#include "tasks.h"

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  xTaskCreate(Tracking, "Update Angle", 2048, NULL, 1, NULL);
  xTaskCreate(Database, "Send Data", 2048, NULL, 1, NULL);
}

void loop() {}
