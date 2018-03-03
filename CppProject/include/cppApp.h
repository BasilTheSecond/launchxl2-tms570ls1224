/*
 * cppApp.h
 *
 */

#ifndef _CPPAPP_H_
#define _CPPAPP_H_

#include <string>
#include <queue>
#include "baseTask.h"

class CppApp
{
public:
    CppApp(std::string& name);
    ~CppApp();
    static void cppAppTask(void *parameter);
    CppApp& operator+=(CppApp& cppApp);
private:
    std::string m_name;
};

class CppAppTask: public BaseTask
{
public:
    CppAppTask(const char* name, uint16_t stackSize = configMINIMAL_STACK_SIZE,
                                   UBaseType_t priority = tskIDLE_PRIORITY + 1);
    virtual ~CppAppTask();
    virtual void run();
private:
    std::queue<char> m_queue;
};

#endif /* _CPPAPP_H_ */
