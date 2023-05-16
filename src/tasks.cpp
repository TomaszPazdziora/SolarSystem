#include "tasks.h"


/*--------------- TRACKING TASK ---------------*/

void Tracking(void *pvParameters) {
  int receivedAngle = 0;
  for (;;) {
    getAngle();
    vTaskDelay(TRACKING_DELAY / portTICK_PERIOD_MS);
  }
}

/*--------------- DATABASE TASK ---------------*/

void Database(void *pvParameters) {
  for (;;) {

    xSemaphoreTake(PanelPowerMutex, portMAX_DELAY);
    Serial.println("Send lock mutex");

    sendToServer(panelPower);

    Serial.println("Send unlock mutex");
    xSemaphoreGive(PanelPowerMutex);
    
    vTaskDelay(DATABASE_DELAY / portTICK_PERIOD_MS);
  }
}

/*--------------- POWER BATTERIES TASK ---------------*/

void PowerBatteries(void *pvParameters) {
  float voltage = 0;
  for(;;) {
    voltage = measureBatsVolt();

    if (voltage < abortBound) abort();
    else {
      if (voltage < lowerBound) digitalWrite(BMS_enable_Pin, HIGH);
      else {
        if (voltage > higherBound) digitalWrite(BMS_enable_Pin, LOW);
      }
    }

    Serial.println(voltage);
    vTaskDelay(POWER_BATTERIES_DELAY / portTICK_PERIOD_MS);
  }
}

/*--------------- MPTT TASK ---------------*/

void MPPT(void *pvParameters) {
  
  for(;;) {
    testPWM();
    vTaskDelay(MPPT_DELAY / portTICK_PERIOD_MS);
  }
}