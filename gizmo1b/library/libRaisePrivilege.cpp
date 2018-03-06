#include "libRaisePrivilege.h"

#define portRESET_PRIVILEGE(xRunningPrivileged) \
                         if( xRunningPrivileged == 0 ) portSWITCH_TO_USER_MODE()
#ifdef __cplusplus
#pragma SWI_ALIAS(1)
#else
#pragma SWI_ALIAS(prvRaisePrivilege, 1)
#endif
extern "C" BaseType_t prvRaisePrivilege(void);

LibRaisePrivilege::LibRaisePrivilege() :
    m_runningPrivileged(prvRaisePrivilege())
{
}

LibRaisePrivilege::~LibRaisePrivilege()
{
    portRESET_PRIVILEGE(m_runningPrivileged);
}
