#include "jsonSender.h"

void sendJsonData(const char* serverUrl, JsonDocument& doc) {
  
  String jsonString;
  serializeJson(doc, jsonString);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonString);

    if (httpResponseCode > 0) {

      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println("Response: " + response);

    } else {

      Serial.print("Error on sending POST request: ");
      Serial.println(httpResponseCode);

    }

    http.end();

  } else {

    Serial.println("Error in WiFi connection");

  }
}
