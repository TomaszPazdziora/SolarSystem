#include <Arduino.h>
#include <Stepper.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string>

// WIFI 
extern const char* ssid;
extern const char* password;
extern const char* url;
extern const char* url_time;
extern const char* url_measure;


// TRACKING 
// extern const int stepsPerResolution;
// extern const int stepperSpeed;
// extern Stepper azimStepper;
int getAngle();


// DATABASE
void sendToServer(int measurement);


// BATTERIES 
extern const int BatVolt_Pin;
extern const int BMS_enable_Pin;
extern const float abortBound;
extern const int abortDelay;
extern const float lowerBound;
extern const float higherBound;
void abort();
float measureBatsVolt();


// MPPT
extern const int PWM_Pin; 
extern const int PWM_freq;
extern const int PWM_channel;
extern const int PWM_resolution;
void testPWM();