#include "FreeRTOS.h"
#include "libOperatorDelete.h"

void operator delete(void* p)
{
    vPortFree(p);
}
