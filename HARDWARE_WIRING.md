# 🔌 Hardware Connections & Circuit Diagram: BLE Smart Lock

## 1. Bill of Materials (BOM)
| Component | Quantity | Notes |
| :--- | :--- | :--- |
| **ESP32 NodeMCU Development Board** | 1 | Microcontroller with BLE 4.2 / 5.0 |
| **12V Solenoid Door Lock Latch** | 1 | 12V 1.5A Draw when active |
| **5V Single Channel Relay Module** | 1 | Optocoupler Isolated |
| **0.96" I2C SSD1306 OLED Display** | 1 | 128x64 pixels (Address 0x3C) |
| **Active Piezo Buzzer** | 1 | Audio feedback (GPIO 27) |
| **LED Indicators** | 2 | Green (GPIO 14), Red (GPIO 12) |
| **12V 2A DC Power Adapter / LiPo** | 1 | External solenoid power |
| **1N4007 Flyback Diode** | 1 | Solenoid inductive kickback protection |

---

## 2. Pinout Connection Mapping

### A. ESP32 to OLED Display & Relay
| ESP32 Pin | Target Component Pin | Description |
| :--- | :--- | :--- |
| **GPIO 21** | OLED SDA | I2C Data Line |
| **GPIO 22** | OLED SCL | I2C Clock Line |
| **GPIO 26** | Relay Module IN | Solenoid Lock Signal (HIGH = Unlock) |
| **GPIO 27** | Buzzer Positive (+) | Audio alert tone |
| **GPIO 14** | Green LED Anode | Unlock Status LED (220Ω resistor) |
| **GPIO 12** | Red LED Anode | Locked Status LED (220Ω resistor) |
| **VIN (5V)** | Relay VCC & OLED VCC | 5V Power Rail |
| **GND** | Common GND | System Common Ground |

---

## 3. Circuit Diagram
```
              +-------------------+
              |  12V DC Supply    |
              | (+)           (-) |
              +--+-------------+--+
                 |             |
                12V           GND
                 |             |
                 +---+         |
                     |         |
                  [SOLENOID LOCK]
                     |         |
                 +---+---+     |
                 | COM   |     |
            +----+       +-----+
            |    | NO    |
            |    +-------+
            |  5V RELAY  |
            | [IN] [VCC] |
            +--+-----+---+
               |     |
    +----------+-----+---------------------------+
    | GPIO26  VIN   GND    GPIO21 GPIO22 GPIO27  |
    |                                            |
    |                  ESP32                     |
    |                                            |
    | [GPIO14]  [GPIO12]                         |
    +----+---------+-----------------------------+
         |         |
      GREEN LED  RED LED
```
