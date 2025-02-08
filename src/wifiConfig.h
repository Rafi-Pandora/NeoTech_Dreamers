#ifndef WIFICONFIG_H
#define WIFICONFIG_H

#include <Arduino.h>

class wifiConfig {
public:
    const char* ap_ssid = "ESP32-Access-Point"; 
    const char* ap_password = "123456789";      
    String ssid = "HMPV-Guardian-AP";
    String password = "tidakada";

    static const String titleDisplay;
    static const String footerDisplay;

    wifiConfig() {}

};

#endif 
