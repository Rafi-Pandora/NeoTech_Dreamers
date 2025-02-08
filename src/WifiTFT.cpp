#include "WifiTFT.h"

WifiTFT::WifiTFT() : server(80) {TFTLcd.tftInit();}

// String WifiTFT::readEEPROM(int* addr) {
//     String data = "";
//     char character;
//     for (int i = *addr; i < *addr + 50; i++) {
//         character = EEPROM.read(i);
//         if (character == '\0') break; 
//         data += character;
//     }
//     return data;
// }

void WifiTFT::displayContent(const String &context, const uint8_t &textSize) {
    TFTLcd.display(context, textSize);
}

void WifiTFT::displayMessage1( const String &line1, const String &line2, const String &line3, const String line4, const String &title, const String &footerText) {
    TFTLcd.drawChangeDisplay(title, line1, line2, line3, line4, footerText, 1);
}

void WifiTFT::displayMessage( const String &line1, const String &line2, const String &line3, const String &line4,const String &title, const String &footerText) {
    TFTLcd.drawChangeDisplay(title, line1, line2, line3, line4, footerText, 2);
}
void WifiTFT::displayPage(String k1Text, String k2Text, String k3Text, String k4Text, String k5Text, String k1Desc, String k2Desc, String k3Desc, String k4Desc) {
    TFTLcd.drawColumnPage( k1Text,  k2Text,  k3Text,  k4Text,  k5Text,  k1Desc,  k2Desc,  k3Desc,  k4Desc);
}


void WifiTFT::begin() {
    TFTLcd.display("\n\n      Memuat...", 2);

    // EEPROM.begin(512);  

    // ssid = readEEPROM(&SSID_ADDR);
    // password = readEEPROM(&PASSWORD_ADDR);

    WiFi.softAP(WiFIConfig.ap_ssid, WiFIConfig.ap_password);
    delay(3000);

    displayMessage( "koneksikan ke ", WiFIConfig.ap_ssid, "\n masukan ip dibawah\n ini:","\n\n "+ WiFi.softAPIP().toString() + "\n ke browser Anda");
    
    if (WiFIConfig.ssid == "" && WiFIConfig.password == "")
    {
        startWebServer();
    } else {
        startWebServer();
        handleConnect();
    }

    server.on("/", HTTP_GET, std::bind(&WifiTFT::handleRoot, this));
    server.on("/connect", HTTP_POST, std::bind(&WifiTFT::handleConnect, this));
    server.on("/reset", HTTP_GET, std::bind(&WifiTFT::handleReset, this));
    server.on("/skip", HTTP_GET, std::bind(&WifiTFT::handleSkip, this));
    
}

void WifiTFT::startWebServer() {
    server.begin();
}

void WifiTFT::handleSkip() {
    wifiStatusConnection = true;
    server.send(200, "text/html", "<p>skipped</p>");
    return;
}

void WifiTFT::handleClient() {
    server.handleClient();
}

void WifiTFT::handleRoot() {
    String html = "<html><head>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; background-color: #f4f4f9; margin: 0; padding: 0; text-align: center;}";
    html += "h1 { color: #4CAF50; }";
    html += "form { background-color: #ffffff; padding: 20px; border-radius: 10px; box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1); display: inline-block; margin-top: 20px; }";
    html += "select, input[type=password], input[type=submit] { width: 100%; padding: 10px; margin: 10px 0; border: 1px solid #ccc; border-radius: 5px; box-sizing: border-box; font-size: 16px;}";
    html += "input[type=submit] { background-color: #4CAF50; color: white; cursor: pointer; border: none;}";
    html += "input[type=submit]:hover { background-color: #45a049; }";
    html += "p { color: #555; font-size: 18px; }";
    html += "footer { margin-top: 30px; font-size: 14px; color: #888; }";
    html += "</style>";
    html += "</head><body>";
    html += "<h1>ESP32 Wi-Fi Scan</h1>";
    html += "<p>Pilih jaringan Wi-Fi yang ingin dihubungkan:</p>";

    int n = WiFi.scanNetworks();
    if (n == 0) {
        html += "<p>Tidak ada jaringan Wi-Fi ditemukan.</p>";
        html += "<a href='/skip' style='display: inline-block; margin-top: 10px; padding: 10px 20px; background-color: #f44336; color: white; text-decoration: none; border-radius: 5px;'>Skip</a>";
    } else {
        html += "<form action='/connect' method='POST'>";
        html += "<select name='ssid'>";
        for (int i = 0; i < n; i++) {
            html += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
        }
        html += "</select>";
        html += "<br><br>";
        html += "Password: <input type='password' name='password'><br><br>";
        html += "<input type='submit' value='Connect'>";
        html += "<a href='/skip' style='display: inline-block; margin-top: 10px; padding: 10px 20px; background-color: #f44336; color: white; text-decoration: none; border-radius: 5px;'>Skip</a>";
        html += "<a href='/reset' style='display: inline-block; margin-top: 10px; padding: 10px 20px; background-color: #f44336; color: white; text-decoration: none; border-radius: 5px;'>ulangi pengecekan</a>";
        html += "</form>";
    }
    html += "<footer>ESP32 Web Server - HMPV Guardian</footer>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}
 
void WifiTFT::handleReset() {
    wifiResetCheck = true;
    server.send(200, "text/html", "<p>reseting...</p>");
    server.sendHeader("redirect", "/", true);
    return;
}

void WifiTFT::handleConnect() {

    if (WiFIConfig.ssid == "" && WiFIConfig.password == "")
    {
        WiFIConfig.ssid = server.arg("ssid");
        WiFIConfig.password = server.arg("password");

        // EEPROM.put(SSID_ADDR, ssid);
        // EEPROM.put(PASSWORD_ADDR, ssid);

        // if (EEPROM.commit())
        // {
        //     Serial.println("data disimpan pada EEPROM");
        // } else {
        //     Serial.println("data gagal tersimpan pada EEPROM");
        // }
    }
    
    displayMessage( "menghubungkan ke", WiFIConfig.ssid);

    WiFi.begin(WiFIConfig.ssid.c_str(), WiFIConfig.password.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nTerhubung ke Wi-Fi!");
        Serial.print("Alamat IP: ");
        Serial.println(WiFi.localIP());

        displayMessage( "wifi tersambung ke", WiFIConfig.ssid);

        // WiFi.softAPdisconnect(true);

        delay(5000);
        wifiStatusConnection = true;
        server.send(200, "text/html", "<p>Wifi berhasil terhubung ke " + WiFIConfig.ssid);
        return;

    } else {
        Serial.println("\nGagal menghubungkan ke Wi-Fi");
        displayMessage( "wifi tidak dapat", "tersambung");

        WiFIConfig.ssid = "";
        WiFIConfig.password = "";
       
        WiFi.disconnect();

        server.send(200, "text/html", "<p>Wifi gagal terhubung ke " + WiFIConfig.ssid);

        begin();

        // EEPROM.put(SSID_ADDR, "");
        // EEPROM.put(PASSWORD_ADDR, "");

        // if (EEPROM.commit())
        // {
        //     Serial.println("data disimpan pada EEPROM");
        // } else {
        //     Serial.println("data gagal tersimpan pada EEPROM");
        // }
    }
}