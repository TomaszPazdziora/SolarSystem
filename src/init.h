#include <Arduino.h>
#include <Stepper.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string>

// WIFI VARIABLES
extern const char* ssid;
extern const char* password;
extern const char* url;
extern const char* url_time;
extern const char* url_measure;

// TRACKING VARIABLES
// extern const int stepsPerResolution;
// extern const int stepperSpeed;
// extern Stepper azimStepper;

// PWM VARIABLES
extern const int PWM_Pin; 
extern const int PWM_freq;
extern const int PWM_channel;
extern const int PWM_resolution;

int getAngle();
void sendToServer(int measurement);
void testPWM();