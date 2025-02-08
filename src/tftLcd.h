#ifndef TFTLCD_H
#define TFTLCD_H

#include <TFT_eSPI.h>  

class tftLcd {
private:
    TFT_eSPI tft;  

    // const uint8_t TFT_PIN_CS = 15;    // Chip Select pin (CS)
    // const uint8_t TFT_PIN_DC = 2;     // Data/Command pin (D/C)
    // const uint8_t TFT_PIN_RST = 4;  // Reset pin (RESET)
    // const uint8_t TFT_PIN_SCLK = 18;   // SPI Clock pin (SCK)
    // const uint8_t TFT_PIN_MISO = 12;  // SPI MISO pin (SDO) (not required if only output)
    // const uint8_t TFT_PIN_MOSI = 23;  // SPI MOSI pin (SDI)
    const uint8_t TFT_PIN_LED = 32;   // LED pin (for backlight control)

public:
    tftLcd();  
    void display(String message, uint8_t textSize);  
    void drawHeader(const String title);
    void drawColumnPage(String k1Text, String k2Text, String k3Text, String k4Text, String k5Text, String k1Desc, String k2Desc, String k3Desc, String k4Desc);
    void drawContent(const String line1, const String line2, const String line3, const String line4, const uint8_t sizeText);
    void drawFooter(const String footerText);
    void drawChangeDisplay(const String &title, const String &line1, const String &line2, const String line3, const String &line4, const String &footerText, const int &textSize);
    void cleanDisplay();
    void tftInit();
};

#endif 
