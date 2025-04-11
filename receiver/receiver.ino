#include <SPI.h>
#include <LoRa.h>

// Pin configuration for SX1278 module
#define SCK     5
#define MISO    19
#define MOSI    27
#define SS      18
#define RST     23
#define DIO0    26

#define BAND    433E6 
byte data[50];


void setup() {
  Serial.begin(115200);
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Optional: Match these to sender for better performance
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);

  Serial.println("LoRa Receiver Ready");
}

void loop (){
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet size ");
    Serial.print(packetSize);
    Serial.print(" data ");
    // read packet
    while (LoRa.available())
    for(int i=0;i<packetSize;i++){
      data[i]= LoRa.read();
      Serial.print(' ');
      Serial.print(data[i]);
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}