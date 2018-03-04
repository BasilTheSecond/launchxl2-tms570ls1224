
#include "FreeRTOS.h"
#include "libComm.h"
#include "sci.h"

bool LibComm::s_isInitialized = false;

#define portRESET_PRIVILEGE(xRunningPrivileged) \
                         if( xRunningPrivileged == 0 ) portSWITCH_TO_USER_MODE()
#ifdef __cplusplus
#pragma SWI_ALIAS(1)
#else
#pragma SWI_ALIAS(prvRaisePrivilege, 1)
#endif
extern "C" BaseType_t prvRaisePrivilege(void);

LibComm::LibComm()
{
    if (!s_isInitialized) {
        BaseType_t xRunningPrivileged = prvRaisePrivilege();
        sciInit();
        portRESET_PRIVILEGE( xRunningPrivileged );
        s_isInitialized = true;
    }
}

LibComm::~LibComm()
{
}
