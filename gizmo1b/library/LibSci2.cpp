#include "LibSci2.h"

bool LibSci2::s_isInitialized;
SemaphoreHandle_t LibSci2::s_sem;
QueueHandle_t LibSci2::s_rxQueue;
QueueHandle_t LibSci2::s_txQueue;
sciBASE_t* LibSci2::s_sci;

LibSci2::LibSci2(UBaseType_t rxQueueLength, UBaseType_t txQueueLength)
{
    if (!s_isInitialized) {
        s_sci = scilinREG;
        s_sem = xSemaphoreCreateBinary();
        s_rxQueue = xQueueCreate(rxQueueLength, sizeof(uint8));
        s_txQueue = xQueueCreate(txQueueLength, sizeof(uint8));
        s_isInitialized = true;
    }
}

LibSci2::~LibSci2()
{
}

void LibSci2::openLowLevel()
{
    setBaudRateLowLevel();
    setParityLowLevel();
    setStopBitsLowLevel();
    sciExitResetState(s_sci);
}

void LibSci2::closeLowLevel()
{
    sciEnterResetState(s_sci);
}

void LibSci2::clearErrorsLowLevel()
{
    sciRxError(s_sci);
}

void LibSci2::writeDataLowLevel(uint8 data)
{
    s_sci->TD = data;
}

uint8 LibSci2::readDataLowLevel()
{
    return (uint8)(s_sci->RD & 0x000000FFU);
}

void LibSci2::enableTxIntLowLevel()
{
    s_sci->SETINT = (uint32)SCI_TX_INT;
}

void LibSci2::setBaudRateLowLevel()
{
    uint32 baud[] = {
        [BAUD_9600]   =   9600,
        [BAUD_19200]  =  19200,
        [BAUD_38400]  =  38400,
        [BAUD_57600]  =  57600,
        [BAUD_115200] = 115200,
    };
    sciSetBaudrate(s_sci, baud[m_baudRate]);
}

void LibSci2::setParityLowLevel()
{
    uint32 parity[] = {
        [NO_PARITY]   = (uint32)((uint32)0U << 3U),
        [EVEN_PARITY] = (uint32)((uint32)3U << 3U),
        [ODD_PARITY]  = (uint32)((uint32)1U << 3U),
    };
    s_sci->GCR1 &= ~(uint32)((uint32)3U << 3U);
    s_sci->GCR1 |= parity[m_parity];
}

void LibSci2::setStopBitsLowLevel()
{
    s_sci->GCR1 &= ~(uint32)((uint32)1U << 4U);
    uint32 stopBits[] = {
        [ONE_STOP] = (uint32)((uint32)0U << 4U),
        [TWO_STOP] = (uint32)((uint32)1U << 4U),
    };
    s_sci->GCR1 |= stopBits[m_stopBits];
}

QueueHandle_t LibSci2::getRxQueue()
{
    return s_rxQueue;
}

QueueHandle_t LibSci2::getTxQueue()
{
    return s_txQueue;
}

SemaphoreHandle_t LibSci2::getSem()
{
    return s_sem;
}

QueueHandle_t LibSci2::sGetRxQueue()
{
    return s_rxQueue;
}

QueueHandle_t LibSci2::sGetTxQueue()
{
    return s_txQueue;
}

SemaphoreHandle_t LibSci2::sGetSem()
{
    return s_sem;
}

void libSci2Notification(sciBASE_t* sci, uint32 flags)
{
    uint8 byte;
    if (flags & SCI_RX_INT) {
        byte = (uint8)(sci->RD & 0x000000FFU);
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(LibSci2::sGetRxQueue(), &byte,
                                                     &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    if  (flags & SCI_TX_INT) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        if (xQueueReceiveFromISR(LibSci2::sGetTxQueue(), &byte,
                                 &xHigherPriorityTaskWoken) != errQUEUE_EMPTY) {
            sci->TD = (uint32)byte;
        }
        else {
            sci->CLEARINT = (uint32)SCI_TX_INT;
            xHigherPriorityTaskWoken = pdFALSE;
            xSemaphoreGiveFromISR(LibSci2::sGetSem(), &xHigherPriorityTaskWoken);
        }
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
