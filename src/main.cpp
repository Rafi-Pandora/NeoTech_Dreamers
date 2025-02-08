#include <Arduino.h>
#include "WifiTFT.h"
#include "sensor.h"

WifiTFT wifi32;
sensor Sensor;

unsigned long currentMillis;
unsigned long previousMillis;
unsigned long interval = 5000;
bool runOnceOnly = false;
bool runOnceOnlyBeatTest = false;
bool runOnceOnlyMQ135 = false;
bool runOnceOnlyTempSensor = false;

bool statusTempSensor = true;
bool statusMAX30102 = true;
bool statusMQ135 = true;
bool statusCheckEnd = false;


void setup() {
    Serial.begin(115200);
    wifi32.begin();
}

void loop() {
    if (wifi32.wifiResetCheck && wifi32.wifiStatusConnection) {
        
        wifi32.wifiResetCheck = false;

        Sensor.stopBeatPerMinute = false;
        Sensor.stopMq135 =  false;
        Sensor.stopTempSensor = false;

        runOnceOnly = false;
        runOnceOnlyBeatTest = false;
        runOnceOnlyMQ135 = false;
        runOnceOnlyTempSensor = false;

        statusTempSensor = true;
        statusMAX30102 = true;
        statusMQ135 = true;
        statusCheckEnd = false;
    }

    if (!runOnceOnly && wifi32.wifiStatusConnection) {
        Sensor.beatsPerMinute = 0;
        Sensor.beatAvg = 0;
        Sensor.highestReadingMq135 = 0;
        Sensor.highestReadingTemp = 0;

        wifi32.displayMessage( "mengecek sensor...");

        Serial.println("init sensor...");
        runOnceOnly = true;
    }

    if (wifi32.wifiStatusConnection) {
        if (!Sensor.stopBeatPerMinute && statusMAX30102) {
            if (!runOnceOnlyBeatTest) {

                wifi32.displayMessage( "pemeriksaan detak", "jantung, silakan ", "letakan jari pada ", "sensor");
                delay(5000);
                
                if (Sensor.initMaxSensor() != "") {

                    wifi32.displayMessage1( Sensor.initMaxSensor());
                    while (true); 

                }

                runOnceOnlyBeatTest = true;
            }

            Sensor.max30102Value();

            if (Sensor.irValue < 50000) {

                wifi32.displayMessage( "jari tidak ", "terdeteksi");
                delay(1000);  

            } else {

                wifi32.displayContent("\n       " + String(Sensor.irValue) + "\n   Est BPM = " + String(Sensor.beatsPerMinute), 2);
                delay(1); 

            }
        }
        else if (Sensor.stopBeatPerMinute && statusMAX30102) {
            wifi32.displayMessage( "detak jantung(BPM)", "         " + String(Sensor.beatAvg));
            delay(5000);  
            statusMAX30102 = false;
        } else if (!Sensor.stopMq135 && statusMQ135) {

            if (!runOnceOnlyMQ135) {
                wifi32.displayMessage( "pemeriksaan kadar ", "oksigen silakan ", "tiupkan kantung", "pada sensor");
                
                if (Sensor.initMq135() != "") {
                    wifi32.displayMessage1( Sensor.initMq135());
                    while(true);
                }

                delay(5000);
                runOnceOnlyMQ135 = true;
            }

            wifi32.displayMessage( "   kadar oksigen", "       " + String(Sensor.highestReadingMq135));
            delay(5000);  
            statusMQ135 = false;

        } else if (!Sensor.stopTempSensor && statusTempSensor) {
            if (!runOnceOnlyTempSensor) {

                Wire.end();
                wifi32.displayMessage( "pemeriksaan suhu", "dekatkan anggota ", "tubuh ke sensor");
                delay(5000);
                
                if (Sensor.initTempSensor() != ""){
                    wifi32.displayMessage1( Sensor.initTempSensor()); 
                    while(true);
                }
                
                runOnceOnlyTempSensor = true;
            }
            
            wifi32.displayMessage("       suhu", "       " + String(Sensor.highestReadingTemp));
            delay(5000);  
            
            statusTempSensor = false;
            statusCheckEnd = true;
        }

        if(statusCheckEnd) {

            if(Sensor.beatAvg == 0) {
                wifi32.displayPage("Detak Jantung(BPM)", "Kadar Oksigen", "Suhu Badan (C)", "Hallo, User!", "Failed", 
                                String(Sensor.beatAvg), String(Sensor.highestReadingMq135), String(Sensor.highestReadingTemp, 2), String(Sensor.beatsPerMinute, 2));
                delay(3000);
                
                Sensor.stopBeatPerMinute = false;
                Sensor.stopMq135 =  false;
                Sensor.stopTempSensor = false;

                runOnceOnly = false;
                runOnceOnlyBeatTest = false;
                runOnceOnlyMQ135 = false;
                runOnceOnlyTempSensor = false;

                statusTempSensor = true;
                statusMAX30102 = true;
                statusMQ135 = true;
                statusCheckEnd = false;

            } else {
                wifi32.displayPage("Detak Jantung(BPM)", "Kadar Oksigen", "Suhu Badan (C)", "Hallo, User!", "Success", 
                                    String(Sensor.beatAvg), String(Sensor.highestReadingMq135), String(Sensor.highestReadingTemp, 2), String(Sensor.beatsPerMinute, 2));
                delay(5000);
            }
        }
    }

    wifi32.handleClient();  
}



// #include <EEPROM.h>
// void setup() {
//     Serial.begin(115200);
//     EEPROM.begin(512); 

//     EEPROM.put(0, "");  
//     EEPROM.put(50, "");

//     EEPROM.commit();  // Komit perubahan ke EEPROM
//     Serial.println("Data disimpan ke EEPROM!");
// }

// void loop() {}




// #include <Arduino.h>
// #include <Wire.h>

// void setup() {
//   Serial.begin(115200);
//   Wire.begin(21, 22); // SDA SCL pin

//   Serial.println("Scanning I2C bus...");
//   byte error, address;
//   int nDevices = 0;

//   for (address = 1; address < 127; address++) {
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();

//     if (error == 0) {
//       Serial.print("I2C device found at address 0x");
//       if (address < 16) {
//         Serial.print("0");
//       }
//       Serial.print(address, HEX);
//       Serial.println("  !");
//       nDevices++;
//       delay(500);
//     } 
//   }
//   if (nDevices == 0) {
//     Serial.println("No I2C devices found.");
//   } else {
//     Serial.println("done.");
//   }
// }

// void loop() {}
