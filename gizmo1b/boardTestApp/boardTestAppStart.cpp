#include "FreeRTOS.h"
#include "boardTestAppStart.h"
#include "boardTestApp.h"

void boardTestAppStart()
{
    BoardTestApp boardTestApp("BoardTestApp");
    boardTestApp.start();
    vTaskStartScheduler();
    while (true) {
    }
}
