#include "tftLcd.h"

// 'user', 24x24px
const uint8_t epd_bitmap_user [] PROGMEM = {
	0x00, 0x3c, 0x00, 0x00, 0xff, 0x00, 0x01, 0xff, 0x00, 0x01, 0xff, 0x80, 0x03, 0xff, 0x80, 0x03, 
  0xff, 0xc0, 0x03, 0xff, 0x80, 0x03, 0xff, 0x80, 0x01, 0xff, 0x80, 0x01, 0xff, 0x00, 0x00, 0x7e, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0x81, 0xe0, 0x0f, 0xff, 0xf0, 0x1f, 0xff, 0xf8, 0x1f, 0xff, 0xf8, 
  0x3f, 0xff, 0xfc, 0x3f, 0xff, 0xfc, 0x3f, 0xff, 0xfc, 0x3f, 0xff, 0xfc, 0x3f, 0xff, 0xfc, 0x3f, 
  0xff, 0xfc, 0x1f, 0xff, 0xf8, 0x0f, 0xff, 0xf0
};


tftLcd::tftLcd() {
  Serial.begin(115200);

  pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_RST, OUTPUT);
  pinMode(TFT_SCLK, OUTPUT);
  pinMode(TFT_MISO, INPUT); //tidak tepakai
  pinMode(TFT_MOSI, OUTPUT); 
  pinMode(TFT_PIN_LED, OUTPUT);

  digitalWrite(TFT_CS, HIGH);
  digitalWrite(TFT_DC, LOW);
  digitalWrite(TFT_RST, HIGH);
  digitalWrite(TFT_SCLK, LOW);
  digitalWrite(TFT_MISO, LOW); //tidak terpakai
  digitalWrite(TFT_MOSI, LOW); 
  digitalWrite(TFT_PIN_LED, HIGH);
}

void tftLcd::tftInit() {
  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
}

void tftLcd::display(String message, uint8_t textSize) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(textSize);
  tft.setCursor(20, 110);
  tft.println(message);
}

void tftLcd::drawHeader(const String title) {
  tft.fillRect(0, 0, tft.width(), 40, TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print(title);
}

void tftLcd::drawContent(const String line1, const String line2, const String line3, const String line4, const uint8_t textSize) {
  tft.setTextSize(textSize);
  tft.setTextColor(TFT_WHITE);

  tft.setCursor(10, 50);
  tft.print(line1);

  tft.setCursor(10, 70);
  tft.print(line2);

  tft.setCursor(10, 90);
  tft.print(line3);

  tft.setCursor(10, 110);
  tft.print(line4);
}

void tftLcd::drawFooter(const String footerText) {
  tft.fillRect(0, tft.height() - 40, tft.width(), 40, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, tft.height() - 30);
  tft.print(footerText);
}

void tftLcd::drawChangeDisplay(const String &title, const String &line1, const String &line2, const String line3, const String &line4, const String &footerText, const int &textSize) {
  display("", textSize);
  drawHeader(title);
  drawContent(line1, line2, line3, line4, textSize);
  drawFooter(footerText);
}

void tftLcd::drawColumnPage(String k1Text, String k2Text, String k3Text, String k4Text, String k5Text, String k1Desc, String k2Desc, String k3Desc, String k4Desc) {
  int headerHeight = 40;
  int footerHeight = 40;
  int barHeight = (tft.height() - headerHeight - footerHeight) / 3;

  drawHeader("HMPV Guardian");
  drawFooter("NeoTech Dreamers - 2025");

  int colWidth = tft.width() / 2;
  tft.fillRect(0, headerHeight, colWidth, barHeight, TFT_GREEN);
  tft.fillRect(colWidth, headerHeight, colWidth, barHeight, TFT_YELLOW);

  tft.fillRect(0, headerHeight + barHeight, colWidth, barHeight, TFT_ORANGE);
  tft.fillRect(colWidth, headerHeight + barHeight, colWidth, barHeight, TFT_PURPLE);

  tft.fillRect(0, headerHeight + 2 * barHeight, tft.width(), barHeight, TFT_CYAN);

  //title text
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK);

  tft.setCursor(10, headerHeight + 10);
  tft.print(k1Text);

  tft.setCursor(colWidth + 10, headerHeight + 10);
  tft.print(k2Text);

  tft.setCursor(10, headerHeight + barHeight + 10);
  tft.print(k3Text);

  tft.setTextColor(TFT_WHITE);
  tft.setCursor(colWidth + 10, headerHeight + barHeight + 10);
  tft.print(k4Text);
  tft.setTextColor(TFT_BLACK);

  //text
  tft.setTextSize(3);
  int k5X = (tft.width() - tft.textWidth(k5Text)) / 2;
  tft.setCursor(k5X, headerHeight + 2 * barHeight + (barHeight - tft.fontHeight(3)) / 2);
  tft.print(k5Text);

  //value text
  tft.setTextSize(2);
  tft.setCursor(10, (headerHeight + barHeight + 30) - 80);
  tft.print(k1Desc);

  tft.setCursor(colWidth + 10, (headerHeight + barHeight + 30) - 80);
  tft.print(k2Desc);

  tft.setCursor(10, (headerHeight + 2 * barHeight + 30) - 80);
  tft.print(k3Desc);

  tft.setCursor(colWidth + 10, (headerHeight + 2 * barHeight + 30) - 80);
  tft.drawBitmap(colWidth + 10, (headerHeight + 2 * barHeight + 30) - 85, epd_bitmap_user, 24, 24, TFT_WHITE);
  // tft.print(k4Desc);
}

void tftLcd::cleanDisplay() {
  tft.fillScreen(TFT_BLACK);
}
