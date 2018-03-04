/*
 * init.cpp
 *
 */
#include "FreeRTOS.h"
#include "init.h"
#include "cppApp.h"

static void initTask(void* parameter)
{
    // Start the application
    CppAppTask cppAppTask("CppApp");
    cppAppTask.start();
#if INCLUDE_vTaskDelete
    vTaskDelete(NULL);
#endif
}

void init()
{
    xTaskCreate(initTask, "INIT", configMINIMAL_STACK_SIZE, NULL,
                                                configMAX_PRIORITIES - 1, NULL);
    // Start the scheduler
    vTaskStartScheduler();
    while (1) {
    }
}
