/*
 * baseTask.h
 *
 */

#ifndef SOURCE_BASETASK_H_
#define SOURCE_BASETASK_H_

#include "FreeRTOS.h"
#include "os_task.h"
#include <string>

class BaseTask
{
public:
    BaseTask(const char* name, uint16_t stackSize = configMINIMAL_STACK_SIZE,
                                   UBaseType_t priority = tskIDLE_PRIORITY + 1);
    virtual ~BaseTask();
    virtual void run() = 0;
    void start();
private:
    static void task(void* parameter);
private:
    TaskHandle_t m_handle;
    const std::string m_name;
    const uint16_t m_stackSize;
    const UBaseType_t m_priority;
};

#endif /* SOURCE_BASETASK_H_ */
