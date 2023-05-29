#include "init.h"

/*==============================================*/
/*                   CONCURRENT                 */
/*==============================================*/

TaskHandle_t DatabaseHandler;
TaskHandle_t MPPTHandler;
SemaphoreHandle_t PanelPowerMutex;


/*==============================================*/
/*                    WI-FI                     */
/*==============================================*/

const char* ssid =        "";
const char* password =    "";
const char* url =         "";
const char* url_angle =   "/angle";
const char* url_measure = "/measurements";


/*==============================================*/
/*               TIME MEASUREMENT               */
/*==============================================*/

#define SLEEP_TIME 22
#define SLEEP_HOUR_DELAY 6

const char* ntp_server = "pool.ntp.org";
const long  gmt_offset_sec = 3600;
const int   daylight_offset_sec = 3600;


void printLocalTime() {
  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  if (timeinfo.tm_hour > SLEEP_TIME) {
    esp_sleep_enable_timer_wakeup(SLEEP_HOUR_DELAY * 60 * 1000000);
    esp_deep_sleep_start();
  }
}


/*==============================================*/
/*                    TRACKING                  */
/*==============================================*/

const int stepsPerResolution = 360;
const int stepperSpeed = 10;
Stepper azimuthStepper(stepsPerResolution,  StepperPoz_Pin1, StepperPoz_Pin3,
                                            StepperPoz_Pin2, StepperPoz_Pin4);   

Stepper altitudeStepper(stepsPerResolution, StepperPion1_Pin1, StepperPion1_Pin3,
                                            StepperPion1_Pin2, StepperPion1_Pin4);

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

  // test move 
  Serial.println("Move altitude motor");
  altitudeStepper.setSpeed(stepperSpeed);
  altitudeStepper.step(stepsPerResolution);
  vTaskDelay(100);
  Serial.println("Move altitude motor");
  altitudeStepper.setSpeed(stepperSpeed);
  altitudeStepper.step(stepsPerResolution);
  vTaskDelay(100);

  return newAngle;
}


/*==============================================*/
/*                    DATABASE                  */
/*==============================================*/

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


/*==============================================*/
/*                    BATTERIES                 */
/*==============================================*/

Adafruit_INA219 ina219;
const int abortDelay =      10;
const float abortBound =    5;
const float lowerBound =    7.5;
const float higherBound =   8;


void abort() {
  Serial.println("Voltage is to low! Abort");
  esp_sleep_enable_timer_wakeup(abortDelay * 1000000); // sleep for minute TO CHANGE
  esp_deep_sleep_start();
}


float measureBatsVolt() {
  return (analogRead(BatVolt_Pin) * 3.3) / 4095;
}


/*==============================================*/
/*                      MPPT                    */
/*==============================================*/

const int PWM_freq =       75000; // 100k 
const int PWM_channel =    0;
const int PWM_resolution = 8; // 8 bit pwm - from 0 to 255
const int PWM_step =       5;
float panelPower =         0;
int PWM_actualDuty =       0;


float measurePower() {
  xSemaphoreTake(PanelPowerMutex, portMAX_DELAY);
  float power = ina219.getPower_mW();
  xSemaphoreGive(PanelPowerMutex);

  return power;
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


void findPP() {
  Serial.println("PP");
  float powerBuffer = 0;

  ledcWrite(PWM_channel, PWM_actualDuty);
  vTaskDelay(MEASURE_DELAY / portTICK_PERIOD_MS);
  panelPower = measurePower();
  
  ledcWrite(PWM_channel, PWM_actualDuty - PWM_step);
  vTaskDelay(MEASURE_DELAY / portTICK_PERIOD_MS);
  powerBuffer = measurePower();
  
  if (powerBuffer > panelPower) {
    PWM_actualDuty -= PWM_step;
    return;
  }

  ledcWrite(PWM_channel, PWM_actualDuty + PWM_step);
  vTaskDelay(MEASURE_DELAY / portTICK_PERIOD_MS);
  powerBuffer = measurePower();

  if (powerBuffer > panelPower) PWM_actualDuty += PWM_step;
  
}
