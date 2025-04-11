# SX1276 LoRa Sensor Node (433 MHz EU Region)

This repository contains an example LoRa communication setup using **SX1276** modules operating at **433 MHz (EU868 band)**. The system includes:

- A **sensor node** reading analog soil moisture sensors powered by **Fanso Li-SOCl₂ batteries**
- A **LoRa receiver node** that decodes incoming sensor and battery voltage data
- Communication via **point-to-point (P2P)** LoRa, not LoRaWAN

---

## 📡 Hardware Used

### Transmitter (Sensor Node)
- Generic SX1276-based LoRa board (433 MHz)
- 2x Analog Soil Moisture Sensors (connected to A1 and A2)
- Fanso 3.6V Li-SOCl₂ Battery
- Optional: GPIO-controlled MOSFETs to power sensors for power saving

### Receiver
- Heltec or generic SX1276-based LoRa board (433 MHz)
- USB for serial monitoring via Arduino Serial Monitor

---

## ⚙️ Configuration

Both nodes are configured for **EU433 compliant LoRa P2P** communication:

| Parameter          | Value         |
|-------------------|---------------|
| Frequency         | 433 MHz       |
| Spreading Factor  | 9             |
| Bandwidth         | 62.5 kHz      |
| Sync Word         | 0x22          |
| CRC               | Enabled       |
| Payload Length    | 6 bytes       |

---

## 📦 Payload Format

Each LoRa uplink packet is **6 bytes**, structured as:

| Bytes | Content            |
|-------|---------------------|
| 0-1   | Value 1 (High, Low) |
| 2-3   | Value 2 (High, Low) |
| 4-5   | Node ID (High, Low) |

### Uplink Types

- **Sensor Node (Node ID: 1):**  
  - `Value1`: Soil Moisture Sensor 1 (analog read)
  - `Value2`: Soil Moisture Sensor 2 (analog read)

- **Battery Node (Node ID: 2):**  
  - `Value1`: Battery Voltage × 100 (e.g. `331` = 3.31V)
  - `Value2`: Raw ADC value of battery pin

---

## 📥 Receiver Output Example

```txt
=============================
Received from Node ID: 1
Sensor 1 (SM1) Value: 445
Sensor 2 (SM2) Value: 412
=============================

=============================
Received from Node ID: 2
Battery Voltage: 3.31 V
Raw ADC: 285
=============================
