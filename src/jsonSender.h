#ifndef JSONSENDER_H
#define JSONSENDER_H

#include <Arduino.h>
#include <ArduinoJson.h>  
#include <HTTPClient.h>
#include <WiFi.h>

void sendJsonData(const char* serverUrl, JsonDocument& doc);

#endif
