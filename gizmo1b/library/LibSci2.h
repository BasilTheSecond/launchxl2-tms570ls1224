#ifndef _LIB_SCI2_H_
#define _LIB_SCI2_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "os_queue.h"
#include "sys_common.h"
#include "libSci.h"
#include "sci.h"

class LibSci2: public LibSci
{
public:
    LibSci2(UBaseType_t rxQueueLength, UBaseType_t txQueueLength);
    virtual ~LibSci2();
protected:
    virtual void openLowLevel();
    virtual void closeLowLevel();
    virtual void clearErrorsLowLevel();
    virtual void writeDataLowLevel(uint8 data);
    virtual uint8 readDataLowLevel();
    virtual void enableTxIntLowLevel();
    virtual QueueHandle_t getRxQueue();
    virtual QueueHandle_t getTxQueue();
    virtual SemaphoreHandle_t getSem();
    friend void libSci2Notification(sciBASE_t* sci, uint32 flags);
private:
    void setBaudRateLowLevel();
    void setParityLowLevel();
    void setStopBitsLowLevel();
    static QueueHandle_t sGetRxQueue();
    static QueueHandle_t sGetTxQueue();
    static SemaphoreHandle_t sGetSem();
private:
    static sciBASE_t* s_sci;
    static bool s_isInitialized;
    static QueueHandle_t s_rxQueue;
    static QueueHandle_t s_txQueue;
    static SemaphoreHandle_t s_sem;
};

#endif /* _LIB_SCI2_H_ */
