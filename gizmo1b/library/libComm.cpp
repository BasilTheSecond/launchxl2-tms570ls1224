#include "libRaisePrivilege.h"
#include "libComm.h"
#include "sci.h"

bool LibComm::s_isInitialized = false;

LibComm::LibComm()
{
    if (!s_isInitialized) {
        LibRaisePrivilege libRaisePrivilege;
        sciInit();
        s_isInitialized = true;
    }
}

LibComm::~LibComm()
{
}
