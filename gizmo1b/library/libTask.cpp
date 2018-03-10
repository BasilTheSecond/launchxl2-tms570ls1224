#include "libTask.h"

LibTask::LibTask(const char* name, uint16_t stackSize, UBaseType_t priority,
                                                            bool isPrivileged) :
    m_name(name),
    m_stackSize(stackSize),
    m_priority(priority),
    m_isPrivileged(isPrivileged)
{
}

LibTask::~LibTask()
{
    if (!m_taskHandle) {
        vTaskDelete(m_taskHandle);
    }
}

void LibTask::start()
{
    UBaseType_t priority = m_priority;
    priority |= m_isPrivileged ? portPRIVILEGE_BIT : 0;
    xTaskCreate(task, m_name.c_str(), m_stackSize, this, priority,
                                                                 &m_taskHandle);
}

void LibTask::task(void* parameter)
{
    reinterpret_cast<LibTask*>(parameter)->run();
    reinterpret_cast<LibTask*>(parameter)->m_taskHandle = NULL;
    vTaskDelete(NULL);
}

bool LibTask::isPrivileged()
{
    return m_isPrivileged;
}
