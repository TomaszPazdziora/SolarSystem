#include "init.h"

const char* ssid =        "ssid";
const char* password =    "password";
const char* url =         "url";
const char* url_time =    "url/time";
const char* url_measure = "url/measurments";


bool isConnected() {

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  return true;
}