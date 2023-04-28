#include "tasks.h"

/*--------------- TRACKING TASK ---------------*/

void Tracking(void *pvParameters) {
  int receivedAngle = 0;

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

      vTaskDelay(TRACKING_DELAY / portTICK_PERIOD_MS);
    }
}



/*--------------- DATABASE TASK ---------------*/

void Database(void *pvParameters) {
  int measurment = 1;

    for (;;) {
        if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;

        http.begin(client, url_measure);
        http.addHeader("Content-Type", "text/plain");

        int httpResponseCode = http.POST(String(measurment));

        if (httpResponseCode > 0) {
          Serial.println("POST request successful");
        }
        else {
          Serial.println("Error on POST request");
        }

        http.end();
      }

      measurment++;
      vTaskDelay(DATABASE_DELAY / portTICK_PERIOD_MS);
    }
}

/*--------------- POWER BATTERIES TASK ---------------*/

// void PowerBatteries(void *pvParameters) {

//   for(;;) {

//   vTaskDelay(POWER_BATTERIES_DELAY / portTICK_PERIOD_MS);
//   }
// }


/*--------------- MPTT TASK ---------------*/

// void MPPT(void *pvParameters) {
  
//   for(;;) {

//   vTaskDelay(MPPT_DELAY / portTICK_PERIOD_MS);
//   }
// }