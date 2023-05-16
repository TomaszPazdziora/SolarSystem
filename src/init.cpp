#include "init.h"

// CONCURRENT VARIABLES
TaskHandle_t DatabaseHandler;
TaskHandle_t MPPTHandler;
SemaphoreHandle_t PanelPowerMutex;

// WIFI VARIABLES
const char* ssid =        "";
const char* password =    "";
const char* url =         "";
const char* url_angle =   "";
const char* url_measure = "";


// TRACKING VARIABLES
// const int stepsPerResolution = 360;
// const int stepperSpeed = 10;
// Stepper azimStepper(stepsPerResolution, 16, 18, 17, 19);


// BATTERIES VARIABLES
const int BatVolt_Pin =     35;
const int BMS_enable_Pin =  4;
const int abortDelay =      10;
const float abortBound =    5;
const float lowerBound =    7.5;
const float higherBound =   8;


// MPPT VARIABLES
const int PWM_Pin =        13; 
const int PWM_freq =       5000;
const int PWM_channel =    0;
const int PWM_resolution = 8; // 8 bit pwm - from 0 to 255
const int PWM_step =       5;
float panelPower =         0;
int PWM_actualDuty =       0;


/*--------------- TRACKING FUNCTIONS ---------------*/

TrackerPosition getAngle() {
  TrackerPosition newAngle;
  String azimuth;
  String altitude;
  int blancSpace;

  if ((WiFi.status() == WL_CONNECTED)) { 
    HTTPClient http;

    http.begin(url_angle);
    int httpCode = http.GET();                                        
  
    if (httpCode > 0) {
        azimuth = http.getString();
        blancSpace = azimuth.indexOf(' ');
        altitude = azimuth.substring(blancSpace);
        azimuth.remove(blancSpace, azimuth.length());
      }

    else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  }

  newAngle.azimuth = azimuth.toInt();
  newAngle.altitude = altitude.toInt();
  Serial.println(newAngle.azimuth);
  Serial.println(newAngle.altitude);
  return newAngle;
}


/*--------------- DATABASE FUNCTIONS ---------------*/

void sendToServer(float measurement) {
  
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, url_measure);
    http.addHeader("Content-Type", "text/plain");

    int httpResponseCode = http.POST(String(measurement));

    if (httpResponseCode > 0) {
      Serial.println("POST request successful");
    }
    else {
      Serial.println("Error on POST request");
    }

    http.end();
  }
}


/*--------------- BATTERIES FUNCTIONS ---------------*/

void abort() {
  Serial.println("Voltage is to low! Abort");
  esp_sleep_enable_timer_wakeup(abortDelay * 1000000); // sleep for minute TO CHANGE
  esp_deep_sleep_start();
}


float measureBatsVolt() {
  return (analogRead(BatVolt_Pin) * 3.3) / 4095;
}


/*--------------- MPPT FUNCTIONS ---------------*/

void testPWM() {
  

  for(int PWM_actualDuty = 0; PWM_actualDuty <= 255; PWM_actualDuty++) { 
    xSemaphoreTake(PanelPowerMutex, portMAX_DELAY);
    Serial.println("Test PWM lock mutex");

    panelPower = PWM_actualDuty;
    ledcWrite(PWM_channel, PWM_actualDuty);

    Serial.println("Test PWM unlock mutex");
    xSemaphoreGive(PanelPowerMutex);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


float measurePower() {
  float measurement;
  // TO IMPLEMENT
  return measurement;
}


void calibratePP() {
  PWM_actualDuty = 255;
  float lowerDutyPower = 0;

  while (PWM_actualDuty > 0) {
    // measure actual power
    ledcWrite(PWM_channel, PWM_actualDuty);
    panelPower = measurePower();

    // set lower PWM and measure
    PWM_actualDuty -= PWM_step;
    ledcWrite(PWM_channel, PWM_actualDuty);
    lowerDutyPower = measurePower();

    if (panelPower > lowerDutyPower) {
      // calibrated = true;
      break;
    }
  }
}