/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "masterthread.h"

#include <QSerialPort>
#include <QTime>

MasterThread::MasterThread(QObject *parent) :
    QThread(parent)
{
}

MasterThread::~MasterThread()
{
    m_mutex.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait();
}

/* Communication protocol between master (M) and slave (S):
 * Register read:
 * M->S: RD A0 A1 A2 A3 X0 X1 X2 X3
 * S->M: S D0 D1 D2 D3
 * RD: Register read command (1 byte)
 * A0..3: Register address (4 bytes)
 * X0..3: "Don't care" data (4 bytes)
 * S: Command status (1 byte)
 * D0..3: Register value (4 bytes)
 * A0: Register address MSB
 * A3: Register address LSB
 * D0: Register value MSB
 * D3: Register value LSB
 * Register write:
 * M->S: WR A0 A1 A2 A3 D0 D1 D2 D3
 * S->M: S D0 D1 D2 D3
 * WR: Register write command (1 byte)
 * A0..3: Register address (4 bytes)
 * D0..3: Register value (4 bytes)
 * S: Command status (1 byte)
 * A0: Register address MSB
 * A3: Register address LSB
 * D0: Register value MSB
 * D3: Register value LSB
 */
void MasterThread::transaction(const SettingsDialog::Settings settings, int waitTimeout, const QString &request)
{
    const QMutexLocker locker(&m_mutex);
    m_settings = settings;
    m_waitTimeout = waitTimeout;
    m_request = request;
    if (!isRunning())
        start();
    else
        m_cond.wakeOne();
}

void MasterThread::run()
{
    bool currentSettingsChanged = false;

    m_mutex.lock();
    SettingsDialog::Settings currentSettings;
    if (currentSettings != m_settings) {
        currentSettings = m_settings;
        currentSettingsChanged = true;
    }

    int currentWaitTimeout = m_waitTimeout;
    QString currentRequest = m_request;
    m_mutex.unlock();
    QSerialPort serial;

    if (currentSettings.name.isEmpty()) {
        emit error(tr("No port name specified"));
        return;
    }

    while (!m_quit) {
        if (currentSettingsChanged) {
            serial.close();
            serial.setPortName(currentSettings.name);
            serial.setBaudRate(currentSettings.baudRate);
            serial.setDataBits(currentSettings.dataBits);
            serial.setParity(currentSettings.parity);
            serial.setStopBits(currentSettings.stopBits);
            serial.setFlowControl(currentSettings.flowControl);

            if (!serial.open(QIODevice::ReadWrite)) {
                emit error(tr("Can't open %1, error code %2")
                           .arg(currentSettings.name).arg(serial.error()));
                return;
            }
        }
        // parse request into
        QList<QByteArray> qlist = currentRequest.toUtf8().split(' ');
        QByteArray requestData;
        bool parsingError = false;
        if (qlist.size() != 9) {
            parsingError = true;
        }
        for (int i = 0; i < qlist.size() && !parsingError; i++) {
            if (qlist[i].size() != 2) {
                parsingError = true;
                break;
            }
            char byte = 0;
            for (int j = 0; j < 2; j++) {
               char c = toupper(qlist[i][j]);
               if (c >= '0' && c <= '9') {
                   byte |= (c - '0') << (j ? 0 : 4);
               }
               else if (c >= 'A' && c <= 'F') {
                   byte |= (c - 'A' + 0x0a) << (j ? 0 : 4);
               }
               else {
                   parsingError = true;
                   break;
               }
            }
            if (!parsingError) {
                requestData.append(byte);
            }
        }
        if (!parsingError) {
            // write request
            serial.readAll(); // drop any pending data
            serial.write(requestData);
        }
        else {
            emit error(tr("Parsing error"));
        }
        if (!parsingError && serial.waitForBytesWritten(m_waitTimeout)) {
            // read response
            if (serial.waitForReadyRead(currentWaitTimeout)) {
                QByteArray responseData = serial.readAll();
                while (responseData.size() < 5 && serial.waitForReadyRead(10))
                    responseData += serial.readAll();

                if (serial.error() == QSerialPort::ReadError) {
                    currentSettings.name.clear(); // force serial port reset
                    emit error(tr("Read error"));
                } else if (serial.error() == QSerialPort::TimeoutError) {
                    currentSettings.name.clear(); // force serial port reset
                    emit error(tr("Read Timeout Error"));
                }
                else if (responseData.size() != 5) {
                    currentSettings.name.clear(); // force serial port reset
                    emit error(tr("Response is %1, expected 5").arg(responseData.size()));
                }
                else {
                    QString response;
                    for (int i = 0; i < responseData.size(); i++) {
                        QByteArray qByteArray;
                        for (int j = 0; j < 2; j++) {
                            char c = responseData[i] >> (j ? 0 : 4);
                            c &= 0x0f;
                            if (c >= 0 && c <= 9) {
                                qByteArray.append(c + '0');
                            }
                            else { // c >= 0x0a && c <= 0x0f)
                                qByteArray.append(c - 0x0a + 'A');
                            }
                        }
                        qByteArray.append(' ');
                        response.append(qByteArray);
                    }
                    emit this->response(response);
                }
            } else {
                currentSettings.name.clear(); // force serial port reset
                emit timeout(tr("Wait read response timeout %1")
                             .arg(QTime::currentTime().toString()));
            }
        } else if (!parsingError) {
            currentSettings.name.clear(); // force serial port reset
            emit timeout(tr("Wait write request timeout %1")
                         .arg(QTime::currentTime().toString()));
        }
        m_mutex.lock();
        m_cond.wait(&m_mutex);
        if (currentSettings != m_settings) {
            currentSettings = m_settings;
            currentSettingsChanged = true;
        } else {
            currentSettingsChanged = false;
        }
        currentWaitTimeout = m_waitTimeout;
        currentRequest = m_request;
        m_mutex.unlock();
    }
}
