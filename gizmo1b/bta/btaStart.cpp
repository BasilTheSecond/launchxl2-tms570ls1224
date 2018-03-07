#include "FreeRTOS.h"
#include "btaStart.h"
#include "btaApp.h"

void btaStart()
{
    BtaApp btaApp("btaApp");
    btaApp.start();
    vTaskStartScheduler();
    while (true) {
    }
}
