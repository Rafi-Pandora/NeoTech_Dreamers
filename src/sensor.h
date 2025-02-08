#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <MAX30105.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

class sensor {
private:
    
    const uint8_t mq135Pin = 34; 
    const uint8_t SDA_PIN_GY906 = 21;
    const uint8_t SCL_PIN_GY906 = 22;  
    const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
    Adafruit_MLX90614 mlx = Adafruit_MLX90614();
    MAX30105 particleSensor;
    byte rates[4]; //Array of heart rates
    byte rateSpot = 0;
    long lastBeat = 0; //Time at which the last beat occurred
    long delta;
    const unsigned long interval = 65000;
    const unsigned long interval2 = 5000;
    const unsigned long interval3 = 2000;
    unsigned long currentMillis;
    unsigned long startMillis;

public:

    long irValue;
    float beatsPerMinute;
    int beatAvg;
    uint highestReadingMq135;
    double highestReadingTemp;
    
    bool stopBeatPerMinute = false;
    bool stopMq135 =  false;
    bool stopTempSensor = false;

    sensor();  
    String initTempSensor();
    void tempSensorValue();
    String initMq135();
    void mq135Value();
    String initMaxSensor();
    void max30102Value();
};

#endif  
