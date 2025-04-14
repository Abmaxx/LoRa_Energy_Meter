#include <SPI.h>
#include "LoRa.h"

int SyncWord = 0x22;

void setup() {
    Serial.begin(9600);
    while (!Serial);

    Serial.println("LoRa Receiver Initializing...");

    if (!LoRa.begin(433E6)) {
        Serial.println("LoRa init failed. Check wiring.");
        while (1);
    }

    LoRa.setSpreadingFactor(9);
    LoRa.setSignalBandwidth(62.5E3);
    LoRa.setSyncWord(SyncWord);
    LoRa.enableCrc();

    Serial.println("LoRa init succeeded. Waiting for packets...");
}

void loop() {
    int packetSize = LoRa.parsePacket();
    if (packetSize == 6) {  
        byte payload[6];
        for (int i = 0; i < 6; i++) {
            payload[i] = LoRa.read();
        }

        int value1 = (payload[0] << 8) | payload[1];
        int value2 = (payload[2] << 8) | payload[3];
        int nodeID = (payload[4] << 8) | payload[5];

        Serial.print("Received from Node ID: ");
        Serial.println(nodeID);

        if (nodeID == 1) {  
            Serial.print("Sensor 1 (SM1) Value: ");
            Serial.println(value1);
            Serial.print("Sensor 2 (SM2) Value: ");
            Serial.println(value2);
        } else if (nodeID == 2) { 
            float batteryVoltage = value1 / 100.0;
            Serial.print("Battery Voltage: ");
            Serial.print(batteryVoltage);
            Serial.println(" V");
            Serial.print("Raw ADC: ");
            Serial.println(value2);
        } else {
            Serial.println("Unknown Node ID");
            Serial.print("Raw1: ");
            Serial.println(value1);
            Serial.print("Raw2: ");
            Serial.println(value2);
        }

    }
}
