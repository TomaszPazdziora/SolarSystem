#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string>

extern const char* ssid;
extern const char* password;
extern const char* url;
extern const char* url_time;
extern const char* url_measure;

bool isConnected();