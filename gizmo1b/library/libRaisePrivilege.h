#ifndef _LIB_RAISE_PRIVILEGE_H_
#define _LIB_RAISE_PRIVILEGE_H_

#include "FreeRTOS.h"

class LibRaisePrivilege
{
public:
    LibRaisePrivilege();
    virtual ~LibRaisePrivilege();
private:
    BaseType_t m_runningPrivileged;
};

#endif /* _LIB_RAISE_PRIVILEGE_H_ */
