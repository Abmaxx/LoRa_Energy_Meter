#include <Arduino.h>
#include <SPI.h>
#include "LoRa.h"
#include "Sensor.h"
#include "rawAnalog.h"

#define Node_Board_ID 1
#define Sensor_Node_ID   1
#define Battery_Node_ID  (1 + Sensor_Node_ID)

const int number_of_sensors = 2;
int SyncWord = 0x22;
int counter = 0;
Sensor* sensor_ptrs[number_of_sensors];

// Serial print definitions for different boards
#if defined __SAMD21G18A__ && not defined ARDUINO_SAMD_FEATHER_M0
#define PRINTLN                   SerialUSB.println("")
#define PRINT_CSTSTR(fmt,param)   SerialUSB.print(F(param))
#define PRINT_STR(fmt,param)      SerialUSB.print(param)
#define PRINT_VALUE(fmt,param)    SerialUSB.print(param)
#define PRINT_HEX(fmt,param)      SerialUSB.print(param,HEX)
#define FLUSHOUTPUT               SerialUSB.flush();
#else
#define PRINTLN                   Serial.println("")
#define PRINT_CSTSTR(fmt,param)   Serial.print(F(param))
#define PRINT_STR(fmt,param)      Serial.print(param)
#define PRINT_VALUE(fmt,param)    Serial.print(param)
#define PRINT_HEX(fmt,param)      Serial.print(param,HEX)
#define FLUSHOUTPUT               Serial.flush();
#endif

void setup() {
    #ifdef LOW_POWER
        bool low_power_status = IS_LOWPOWER;
    #else
        bool low_power_status = IS_NOT_LOWPOWER;
    #endif

    sensor_ptrs[0] = new rawAnalog("SM2", IS_ANALOG, IS_CONNECTED, low_power_status, (uint8_t) A2, (uint8_t) 8);
    sensor_ptrs[1] = new rawAnalog("SM1", IS_ANALOG, IS_CONNECTED, low_power_status, (uint8_t) A1, (uint8_t) 9);

    sensor_ptrs[0]->set_n_sample(10);
    sensor_ptrs[1]->set_n_sample(10);

    delay(1500);

    Serial.begin(9600);
    while (!Serial);

    Serial.println("LoRa Sender");

    if (!LoRa.begin(433E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    LoRa.setSpreadingFactor(9);
    LoRa.setSignalBandwidth(62.5E3);
    LoRa.setSyncWord(SyncWord);

    Serial.print("Current spreading factor: ");
    Serial.println(LoRa.getSpreadingFactor());
    Serial.print("Current bandwidth: ");
    Serial.println(LoRa.getSignalBandwidth());
    Serial.println("LoRa init succeeded.");

    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
}

void loop() {
    int packet[3];

    Serial.println("\n\n\n");
    Serial.print("Node Board ID Is : SM");
    Serial.println(Node_Board_ID);

    Serial.print("Packet Counter : ");
    Serial.println(counter);

    Serial.println("Sending Sensor Data...");
    Serial.print("Sensor Node ID is : ");
    Serial.println(Sensor_Node_ID);

    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    analogReference(DEFAULT);
    delay(1000);
    int sensorValue1 = analogRead(A2); // Label as A1 on PCB
    Serial.print("Discrete A1 is : ");
    Serial.println(sensorValue1);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    delay(300);

    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    delay(1000);
    int sensorValue2 = analogRead(A1); // Label as A2 on PCB
    Serial.print("Discrete A2 is : ");
    Serial.println(sensorValue2);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    delay(300);

    packet[0] = sensorValue1;
    packet[1] = sensorValue2;
    packet[2] = Sensor_Node_ID;

    PRINT_CSTSTR("%s", "SM1  ");
    PRINT_VALUE("%d", packet[0]);
    PRINTLN;

    PRINT_CSTSTR("%s", "SM2  ");
    PRINT_VALUE("%d", packet[1]);
    PRINTLN;

    byte payload[6];
    payload[0] = highByte(packet[0]);
    payload[1] = lowByte(packet[0]);
    payload[2] = highByte(packet[1]);
    payload[3] = lowByte(packet[1]);
    payload[4] = highByte(packet[2]);
    payload[5] = lowByte(packet[2]);

    LoRa.beginPacket();
    LoRa.write(payload, 6);
    LoRa.endPacket();

    LoRa.end();
    LoRa.enableCrc();

    Serial.println("Sending Node Voltage...");
    Serial.print("Battery Node ID : ");
    Serial.println(Battery_Node_ID);

    analogReference(INTERNAL);
    delay(1000);
    int sensorValueBat = analogRead(A3);
    Serial.print("Discrete Batt_V is : ");
    Serial.println(sensorValueBat);

    float Bat_V = ((sensorValueBat / 1023.0) * 1.1) * 4.3 * 100.0;
    Serial.print("Battery Voltage is : ");
    Serial.println(Bat_V / 100.0);

    packet[0] = (int)Bat_V;
    packet[1] = sensorValueBat;
    packet[2] = Battery_Node_ID;

    payload[0] = highByte(packet[0]);
    payload[1] = lowByte(packet[0]);
    payload[2] = highByte(packet[1]);
    payload[3] = lowByte(packet[1]);
    payload[4] = highByte(packet[2]);
    payload[5] = lowByte(packet[2]);

    LoRa.beginPacket();
    LoRa.write(payload, 6);
    LoRa.endPacket();

    LoRa.end();
    LoRa.enableCrc();

    Serial.println("\n\n\nNode will wait 30 seconds...\n\n\n");

    Serial.flush();
    counter++;

    delay(30000); 
}
