#include "tasks.h"

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wifi");
  
   
  ledcSetup(PWM_channel, PWM_freq, PWM_resolution);
  ledcAttachPin(PWM_Pin, PWM_channel);
  ina219.begin();
  
  PanelPowerMutex = xSemaphoreCreateMutex();
  xTaskCreate(Tracking, "Update Angle", 2048, NULL, 1, NULL);
  xTaskCreate(Database, "Send Data", 4096, NULL, 1, &DatabaseHandler);
  // xTaskCreate(PowerBatteries, "Control batteries power", 2048, NULL, 1, NULL);
  xTaskCreate(MPPT, "MPPT Algorithm", 2048, NULL, 1, &MPPTHandler);
}

void loop() {}
