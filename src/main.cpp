#include "tasks.h"

void setup() {
  Serial.begin(115200);

  Serial.println(ssid);
  Serial.println(password);

  WiFi.begin(ssid, password);
 
  xTaskCreate(updateAngle, "Update Angle", 2048, NULL, 1, NULL);
  // xTaskCreate(sendData, "Send Data", 1024, NULL, 1, NULL);
}

void loop() {}


// for (;;) {
//         if (WiFi.status() == WL_CONNECTED) {
//           HTTPClient http;
//           http.begin("http://192.168.0.38:8090/measurments");
//           String data = "ESP";
//           int httpResponseCode = http.POST(data.c_str());
//           if (httpResponseCode > 0) {
//             Serial.println("POST request successful");
//           }
//           else {
//             Serial.println("Error on POST request");
//           }
//           http.end();
//         }
//       vTaskDelay(5000 / portTICK_PERIOD_MS);
//     }