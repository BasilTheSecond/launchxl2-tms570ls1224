#include "FreeRTOS.h"
#include "libOperatorNew.h"

void* operator new(size_t size)
{
    void* p = pvPortMalloc(size);
    return p;
}
