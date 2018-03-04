#include "FreeRTOS.h"
#include "os_task.h"
#include "btaStart.h"
#include "btaApp.h"

static const TickType_t k_btaInitTaskDelay = pdMS_TO_TICKS(1000);

static void btaInitTask(void* parameter)
{
    BtaApp btaApp("btaApp");
    btaApp.start();
    while (1) {
        vTaskDelay(k_btaInitTaskDelay);
    }
}

void btaStart()
{
    xTaskCreate(btaInitTask, "btaInit", configMINIMAL_STACK_SIZE, NULL,
                                                        tskIDLE_PRIORITY, NULL);
    vTaskStartScheduler();
    while (1) {
    }
}
