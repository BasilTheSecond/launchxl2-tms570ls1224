#include "sci.h"
#include "libSci.h"

bool LibSci::s_isInitialized;

LibSci::LibSci()
{
    if (!s_isInitialized) {
        sciInit();
        s_isInitialized = true;
    }
}

LibSci::~LibSci()
{
}

void LibSci::open()
{
    openLowLevel();
    xSemaphoreGive(getSem());
}

void LibSci::close()
{
    closeLowLevel();
    xQueueReset(getRxQueue());
    xQueueReset(getTxQueue());
}

int LibSci::setBaudRate(int baudRate)
{
    switch (baudRate) {
    case BAUD_9600:
    case BAUD_19200:
    case BAUD_38400:
    case BAUD_57600:
    case BAUD_115200:
        m_baudRate = baudRate;
        return OKAY;
    default:
        return ERROR_BAUD_RATE;
    }
}

int LibSci::setParity(int parity)
{
    switch (parity) {
    case BAUD_9600:
    case BAUD_19200:
    case BAUD_38400:
    case BAUD_57600:
    case BAUD_115200:
        m_parity = parity;
        return OKAY;
    default:
        return ERROR_PARITY;
    }
}

int LibSci::setStopBits(int stopBits)
{
    switch (stopBits) {
    case BAUD_9600:
    case BAUD_19200:
    case BAUD_38400:
    case BAUD_57600:
    case BAUD_115200:
        m_stopBits = stopBits;
        return OKAY;
    default:
        return ERROR_STOP_BITS;
    }
}

int LibSci::read(std::vector<uint8>& data)
{
    uint8 dat;
    if (xQueuePeek(getRxQueue(), &dat, 0) == errQUEUE_EMPTY) {
        return ERROR_READ;
    }
    while (xQueueReceive(getRxQueue(), &dat, 0) == pdPASS) {
        data.push_back(dat);
    }
    return OKAY;
}

int LibSci::write(const std::vector<uint8>& data)
{
    xQueueReset(getTxQueue());
    if (uxQueueSpacesAvailable(getTxQueue()) < data.size()) {
        return ERROR_WRITE;
    }
    for (int i = 1; i < data.size(); i++) {
        uint8 dat = data[i];
        xQueueSend(getTxQueue(), &dat, 0);
    }
    writeDataLowLevel(data[0]);
    enableTxIntLowLevel();
    return OKAY;
}

bool LibSci::waitForBytesWritten(int msTimeout)
{
    if (xSemaphoreTake(getSem(), pdMS_TO_TICKS(msTimeout)) == pdFALSE) {
        return false;
    }
    clearErrorsLowLevel();
    return true;
}

bool LibSci::waitForReadyRead(int msTimeout)
{
    uint8 dat;
    if (xQueuePeek(getRxQueue(), &dat, pdMS_TO_TICKS(msTimeout))
                                                            == errQUEUE_EMPTY) {
        return false;
    }
    return true;
}

extern void libSci2Notification(sciBASE_t* sci, uint32 flags);

extern "C" void sciNotification(sciBASE_t* sci, uint32 flags)
{
    if (sci == scilinREG) {
        libSci2Notification(sci, flags);
    }
}
