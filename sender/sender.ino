#include <SPI.h>
#include <LoRa.h>

#define SCK     5
#define MISO    19
#define MOSI    27
#define SS      18
#define RST     23
#define DIO0    26
byte data[] = {1, 2, 3, 4, 56, 7, 8, 9, 10};

#define BAND    433E6 
void setup() {
  Serial.begin(115200);
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Optional: for better range and stability
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);

  Serial.println("LoRa Sender Ready");
}

void loop() {
  Serial.print("\nSending packet: ");
  // send packet
  LoRa.beginPacket();
  for (int i = 0; i < sizeof(data);i++) {
    LoRa.write(data[i]);
    Serial.print(data[i]);
    Serial.print(' ');
  }
  LoRa.endPacket();
  data[0] += 1;
  delay(2000);
}