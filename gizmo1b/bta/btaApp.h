#ifndef _BTA_APP_H_
#define _BTA_APP_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "sys_common.h"
#include "libTask.h"
#include "libComm.h"
#include "libGpio.h"
#include "sci.h"

class BtaApp: public LibTask {
public:
    BtaApp(const char* name);
    virtual ~BtaApp();
    static void setFlagsFromIsr(uint32 flags);
    void clearFlags(uint32 flags);
private:
    virtual void run();
    LibGpio m_libGpio;
    LibComm m_libComm;
    static const TickType_t k_taskDelay;
    static volatile uint32 s_flags;
    static SemaphoreHandle_t s_sem;
    static bool s_isInitialized;
};

#ifdef __cplusplus
extern "C" {
#endif

void sciNotification(sciBASE_t *sci, uint32 flags);

#ifdef __cplusplus
}
#endif

#endif /* _BTA_APP_H_ */
