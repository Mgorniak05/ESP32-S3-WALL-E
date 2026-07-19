# ESP32-S3 W.A.L.L.-E (Advanced Robotics Project)

## Hardware Setup
The main board was designed with noise isolation and logic safety in mind:
* **Brain:** ESP32-S3 DevKitC-1.
* **Drive:** JGB37 DC motors controlled by a TB6612FNG H-bridge.
* **Articulations:** 7x MG90S (Metal Gear) servos for head and arm movements, driven by an external PCA9685 controller (I2C).
* **Audio System:** MAX98357A I2S digital amplifier with a 3W 4Ohm speaker, playing MP3 sounds.
* **Power & Safety:** Custom 2S (7.4V) 18650 battery pack with a BMS and USB-C charging module. Logic (5V) and servos are separated using two independent step-down converters (LM2596 and XL4015). Current and voltage are monitored in real-time by an INA219 sensor (I2C).

## ESP32-S3 Pinout

| Module | Signal | ESP32-S3 Pin |
| :--- | :--- | :--- |
| **I2C Bus** (PCA9685, INA219) | SDA <br> SCL | 21 <br> 47 |
| **Audio I2S** (MAX98357A) | DIN <br> BCLK <br> LRC | 14 <br> 19 <br> 20 |
| **Motor A** (TB6612FNG) | PWMA <br> AN1 <br> AN2 | 3 <br> 41 <br> 46 |
| **Motor B** (TB6612FNG) | PWMB <br> BN1 <br> BN2 | 1 <br> 42 <br> 2 |
| **SD Card** | SD_DATA <br> SD_CLK <br> SD_CMD | 40 <br> 39 <br> 38 |

## Software Architecture
*(Work in progress - pending Object-Oriented Programming refactoring)*

## Roadmap
- [x] Custom baseboard design and soldering.
- [x] I2C bus initialization and power monitoring tests.
- [x] LittleFS filesystem and I2S audio integration.
- [x] 3D printing of the chassis, tracks, and mechanical assembly.
- [ ] Code refactoring to clean OOP (Object-Oriented Programming).
- [ ] Creating and tuning servo animations.