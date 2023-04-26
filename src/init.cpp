#include "init.h"

const char* ssid = "ssid";
const char* password =  "password";
const char* url = "http://192.168.69.144:8090/";

//const char* url_time = strcat(const_cast<char*>(url), "time"); // połączenie adresu URL i endpointu
const char* url_time = "http://192.168.69.144:8090/time";
const char* url_measure = "http://192.168.69.144:8090/measure";


bool isConnected() {

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  return true;
}