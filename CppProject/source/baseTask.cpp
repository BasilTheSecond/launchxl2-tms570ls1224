/*
 * baseTask.cpp
 *
 */
#include "baseTask.h"

BaseTask::BaseTask(const char* name, uint16_t stackSize, UBaseType_t priority) :
    m_name(name),
    m_stackSize(stackSize),
    m_priority(priority)
{
}

BaseTask::~BaseTask()
{
#if INCLUDE_vTaskDelete
    vTaskDelete(m_handle);
#endif
}

void BaseTask::start()
{
    xTaskCreate(task, m_name.c_str(), m_stackSize, this, m_priority, &m_handle);
}

void BaseTask::task(void* parameter)
{
    reinterpret_cast<BaseTask*>(parameter)->run();
}
