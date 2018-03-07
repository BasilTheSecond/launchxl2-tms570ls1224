#ifndef _LIB_TASK_H_
#define _LIB_TASK_H_

#include "FreeRTOS.h"
#include "os_task.h"
#include <string>

class LibTask {
public:
    LibTask(const char* name, uint16_t stackSize = configMINIMAL_STACK_SIZE,
                                   UBaseType_t priority = tskIDLE_PRIORITY + 1);
    virtual ~LibTask();
    virtual void run() = 0;
    void start();
private:
    static void task(void* parameter);
private:
    TaskHandle_t m_taskHandle;
    std::string m_name;
    uint16_t m_stackSize;
    UBaseType_t m_priority;
};

#endif /* _LIB_TASK_H_ */
