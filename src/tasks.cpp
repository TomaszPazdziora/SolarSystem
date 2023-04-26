#include "tasks.h"

int receivedAngle = 0;


/*--------------- FREE-RTOS TASKS ---------------*/


void updateAngle(void *pvParameters) {
    for (;;) {
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

      vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}



void sendData(void *pvParameters) {
    for (;;) {
        if ((WiFi.status() == WL_CONNECTED)) { 

        HTTPClient http;
        http.begin(url);
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

      vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}