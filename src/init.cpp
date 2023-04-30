#include "init.h"

// WIFI VARIABLES
const char* ssid =        "";
const char* password =    "";
const char* url =         "";
const char* url_time =    "";
const char* url_measure = "";


// TRACKING VARIABLES
// const int stepsPerResolution = 360;
// const int stepperSpeed = 10;
// Stepper azimStepper(stepsPerResolution, 16, 18, 17, 19);


// BATTERIES VARIABLES
const int BatVolt_Pin = 35;


// MPPT VARIABLES
const int PWM_Pin = 13; 
const int PWM_freq = 5000;
const int PWM_channel = 0;
const int PWM_resolution = 8; // 8 bit pwm - from 0 to 255


/*--------------- TRACKING FUNCTIONS ---------------*/

int getAngle(){
  int receivedAngle = 0;

  if ((WiFi.status() == WL_CONNECTED)) { 
    HTTPClient http;

    http.begin(url_time);
    int httpCode = http.GET();                                        
  
    if (httpCode > 0) {
        receivedAngle = http.getString().toInt();
        Serial.println(receivedAngle);
      }

    else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  }
  return receivedAngle;
}


/*--------------- DATABASE FUNCTIONS ---------------*/

void sendToServer(int measurement) {
  
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

float measureBatsVolt() {
  return (analogRead(BatVolt_Pin) * 3.3) / 4095;
}


/*--------------- MPPT FUNCTIONS ---------------*/

void testPWM() {
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){ 
      ledcWrite(PWM_channel, dutyCycle);
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}