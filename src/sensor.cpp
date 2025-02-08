#include "sensor.h"
#include <heartRate.h>

sensor::sensor() {
    // delay(1000);

    // Wire.begin();

    // if (!max30102.begin()) {
    //     // LCD_TFT.displayMessage("HMPV Guardian", "modul gagal diinisialisasi", "", "NeoTech Dreamers -2025");
    //     while(true);
    // }

    // max30102.setup();
    // max30102.setPulseAmplitudeRed(0x0A);
    // max30102.setPulseAmplitudeGreen(0x0A);

    // LCD_TFT.displayMessage("HMPV Guardian", "modul berhasil diinisialisasi", "", "NeoTech Dreamers -2025");
}

String sensor::initMq135() {
    startMillis = millis();
    highestReadingMq135 = analogRead(mq135Pin);
    if (highestReadingMq135 == 0)
    {
        Serial.println("MQ135 tidak terbaca");
        return "sensor oksigen tidak terbaca";
    }
    
    Serial.println("mq135 terinisialisasi");
    return "";
}

String sensor::initMaxSensor() {
    delay(1000);

    Wire.begin();

    if (!particleSensor.begin()) {
        Serial.println("sensor MAX30102 tidak ditemukan");
        return "sensor MAX30102 tidak ditemukan";
    }

    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x0A);
    particleSensor.setPulseAmplitudeGreen(0x0A);
    startMillis = millis();
    return "";
}

String sensor::initTempSensor() {
    startMillis = millis();
    Wire1.begin(SDA_PIN_GY906, SCL_PIN_GY906);

    if(!mlx.begin()) {
        Serial.println("sensor GY906 tidak terdeteksi");
        return "sensor GY906 tidak terdeteksi";
    }

    highestReadingTemp = mlx.readObjectTempC();
    return "";
}

void sensor::mq135Value() {
    currentMillis = millis();

    int currentReading = analogRead(mq135Pin);

    Serial.println("O2 : " + currentReading);

    if (currentReading > highestReadingMq135) {
        highestReadingMq135 = currentReading;
    }

    if (currentMillis - startMillis >= interval) {
        Serial.print("Highest Gas Concentration in 5 seconds (Analog Reading): ");
        Serial.println(highestReadingMq135);
        stopMq135 = true;
    }
}

void sensor::max30102Value() {
    currentMillis = millis();
    irValue = particleSensor.getIR();

    if (checkForBeat(irValue) == true)
    {
        //We sensed a beat!
        long delta = millis() - lastBeat;
        lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20)
        {
        rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
        rateSpot %= RATE_SIZE; //Wrap variable

        //Take average of readings
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++)
            beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
        }
    }

    Serial.print("IR=");
    Serial.print(irValue);
    Serial.print(", BPM=");
    Serial.print(beatsPerMinute);
    Serial.print(", Avg BPM=");
    Serial.print(beatAvg);

    if (irValue < 50000) {
        startMillis += 1000;
        Serial.print(" No finger?");
    }

    Serial.println();

    if (currentMillis - startMillis >= interval)
    {
        stopBeatPerMinute = true;
    }
    
}

void sensor::tempSensorValue() {
    currentMillis = millis();

    double currentReading = mlx.readObjectTempC();

    Serial.println("temp : " + String(currentReading, 2) + " C");

    if(currentReading > highestReadingTemp) {
        highestReadingTemp = currentReading;
    }

    if(currentMillis - startMillis >= interval) {
        stopTempSensor = true;
    }
}