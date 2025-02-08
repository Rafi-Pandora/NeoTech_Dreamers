#ifndef WIFITFT_H
#define WIFITFT_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "tftLcd.h"
#include "wifiConfig.h"
// #include <EEPROM.h>

class WifiTFT {
public:
    WifiTFT(); 

    bool wifiStatusConnection = false;
    bool wifiResetCheck = false;

    void displayContent( const String &context, const uint8_t &textSize);
    void displayMessage1(
                         const String &line1 = "", const String &line2 = "",
                         const String &line3 = "", const String line4 = "",
                         const String &title = "HMPV Guardian", 
                         const String &footerText = "NeoTech Dreamers - 2025");
    void displayMessage( 
                        const String &line1 = "", const String &line2 = "", 
                        const String &line3 = "", const String &line4 = "",
                        const String &title = "HMPV Guardian", 
                        const String &footerText = "NeoTech Dreamers - 2025");
    void displayPage(String k1Text = "", 
                     String k2Text = "", 
                     String k3Text = "", 
                     String k4Text = "", 
                     String k5Text = "", 
                     String k1Desc = "", 
                     String k2Desc = "", 
                     String k3Desc = "", 
                     String k4Desc = "");
    void begin();
    void handleClient();
    void handleRoot(); 
    void handleConnect(); 
    void startWebServer();
    void handleReset(); 
    void handleSkip();

    // String readEEPROM(int* addr);

private:
    WebServer server;  
    tftLcd TFTLcd;  
    wifiConfig WiFIConfig;

    // int SSID_ADDR = 0;      
    // int PASSWORD_ADDR = 50;   
};

#endif
