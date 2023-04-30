#include "init.h"

// all delays will be changed
#define TRACKING_DELAY 1000 * 30
#define DATABASE_DELAY 1000 * 15
#define POWER_BATTERIES_DELAY 1000 * 10
#define MPPT_DELAY 1000 * 1

// extern int receivedAngle;

/*--------------- TASKS DECLARATION ---------------*/

void Tracking(void *pvParameters);
void Database(void *pvParameters);
// void PowerBatteries(void *pvParameters);
void MPPT(void *pvParameters);
