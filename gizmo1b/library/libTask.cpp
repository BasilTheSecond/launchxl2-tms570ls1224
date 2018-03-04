#include "libTask.h"

LibTask::LibTask(const char* name, uint16_t stackSize, UBaseType_t priority) :
    m_name(name),
    m_stackSize(stackSize),
    m_priority(priority)
{
}

LibTask::~LibTask()
{
#if INCLUDE_vTaskDelete
    vTaskDelete(m_handle);
#endif
}

void LibTask::start()
{
    xTaskCreate(task, m_name.c_str(), m_stackSize, this, m_priority, &m_handle);
}

void LibTask::task(void* parameter)
{
    reinterpret_cast<LibTask*>(parameter)->run();
}
