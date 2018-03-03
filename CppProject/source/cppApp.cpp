/*
 * cppApp.cpp
 *
 */
#include "cppApp.h"

CppApp::CppApp(std::string& name) :
    m_name(name)
{
}

CppApp::~CppApp()
{
}

CppApp& CppApp::operator+=(CppApp& cppApp)
{
    m_name += cppApp.m_name;
    return *this;
}

CppAppTask::CppAppTask(const char* name, uint16_t stackSize,
                                                         UBaseType_t priority) :
    BaseTask(name, stackSize, priority)
{
}

CppAppTask::~CppAppTask()
{
}

void CppAppTask::run()
{
    std::string cppApp1Name("cppApp1");
    CppApp cppApp1(cppApp1Name);
    std::string cppApp2Name("cppApp2");
    CppApp cppApp2(cppApp2Name);
    cppApp1 += cppApp2;
    m_queue.push('a');
    m_queue.push('b');
    m_queue.push('c');
    char out[3];
    out[0] = m_queue.front(); m_queue.pop();
    out[1] = m_queue.front(); m_queue.pop();
    out[2] = m_queue.front(); m_queue.pop();
    while (1) {
    }
}

