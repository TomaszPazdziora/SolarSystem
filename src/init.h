#include <Arduino.h>
#include <Stepper.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string>
#include <mutex>
#include <Adafruit_INA219.h>


// CONCURRENT
extern TaskHandle_t DatabaseHandler;
extern TaskHandle_t MPPTHandler;
extern SemaphoreHandle_t PanelPowerMutex;


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
struct TrackerPosition {
  int azimuth;
  int altitude;
};
TrackerPosition getAngle();


// DATABASE
void sendToServer(float measurement);


// BATTERIES 
extern Adafruit_INA219 ina219;
extern const int BatVolt_Pin;
extern const int BMS_enable_Pin;
extern const float abortBound;
extern const int abortDelay;
extern const float lowerBound;
extern const float higherBound;
void abort();
float measureBatsVolt();


// MPPT
#define MEASURE_DELAY 500
extern const int PWM_Pin; 
extern const int PWM_freq;
extern const int PWM_channel;
extern const int PWM_resolution;

extern int PWM_actualDuty;
extern float panelPower;
extern const int PWM_step;

void testPWM();
float measurePower();
void calibratePP();
void findPP();
