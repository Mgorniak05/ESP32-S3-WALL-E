# ESP32-S3 W.A.L.L.-E (Advanced Robotics Project)

## Hardware Setup
The main board was designed with noise isolation and logic safety in mind:
* **Brain:** ESP32-S3 DevKitC-1.
* **Drive:** JGB37 DC motors controlled by a TB6612FNG H-bridge.
* **Articulations:** 7x MG90S (Metal Gear) servos for head and arm movements, driven by an external PCA9685 controller (I2C).
* **Audio System:** MAX98357A I2S digital amplifier with a 3W 4Ohm speaker, playing MP3 sounds.
* **Power & Safety:** Custom 2S (7.4V) 18650 battery pack with a BMS and USB-C charging module. Logic (5V) and servos are separated using two independent step-down converters (LM2596 and XL4015). Current and voltage are monitored in real-time by an INA219 sensor (I2C).

## ESP32-S3 Pinout

| Module | Signal | Macro Name | ESP32-S3 Pin |
| :--- | :--- | :--- | :--- |
| **I2C Bus** (PCA9685, INA219) | SDA | I2C_SDA | 21 |
| **I2C Bus** (PCA9685, INA219) | SCL | I2C_SCL | 47 |
| **Motor A** (TB6612FNG) | PWM Speed | PWMA_MOTOR | 3 |
| **Motor A** (TB6612FNG) | Direction 1 | AN1_MOTOR | 41 |
| **Motor A** (TB6612FNG) | Direction 2 | AN2_MOTOR | 46 |
| **Motor B** (TB6612FNG) | PWM Speed | PWMB_MOTOR | 1 |
| **Motor B** (TB6612FNG) | Direction 1 | BN1_MOTOR | 42 |
| **Motor B** (TB6612FNG) | Direction 2 | BN2_MOTOR | 2 |
| **Audio I2S** (MAX98357A) | Data In | DIN | 14 |
| **Audio I2S** (MAX98357A) | Bit Clock | BCLK | 19 |
| **Audio I2S** (MAX98357A) | Word Select | LRC | 20 |
| **SD Card** | Data | SD_DATA | 40 |
| **SD Card** | Clock | SD_CLK | 39 |
| **SD Card** | Command | SD_CMD | 38 |
| **Camera OV2640** | Data D0-D3 | pin_d0 - pin_d3 | 11, 9, 8, 10 |
| **Camera OV2640** | Data D4-D7 | pin_d4 - pin_d7 | 12, 18, 17, 16 |
| **Camera OV2640** | Clocks | pin_xclk, pin_pclk | 15, 13 |
| **Camera OV2640** | Sync | pin_vsync, pin_href | 6, 7 |
| **Camera OV2640** | SCCB I2C | sccb_sda, sccb_scl | 4, 5 |

### PCA9685 Servo Channels

| Limb / Joint | Macro Name | PCA9685 Channel |
| :--- | :--- | :--- |
| Left Arm | ARM_L_PORT | 0 |
| Right Arm | ARM_R_PORT | 1 |
| Neck Bottom / Pitch | NECK_1_PORT | 2 |
| Neck Top | NECK_2_PORT | 3 |
| Head Rotation / Yaw | NECK_3_PORT | 4 |
| Left Eye | EYE_L_PORT | 5 |
| Right Eye | EYE_R_PORT | 6 |

## Software Architecture
The firmware is designed with an Object-Oriented Programming (OOP) paradigm, ensuring modularity, non-blocking execution, and hardware safety.

### Core Modules
- **`MotorController` (`MotorController.h` / `.cpp`):** Manages drive logic for dual DC motors via the TB6612FNG bridge (PWM speed control, forward/reverse transitions, and current-safe directional braking).
- **`ServoController` (`ServoController.h` / `.cpp`):** Handles communication with the PCA9685 I2C PWM driver. Implements non-blocking smoothing, angular limits, and centralized calibration for all 7 MG90S articulation servos.
- **`ComboController` (`ComboController.h` / `.cpp`):** A non-blocking, multi-track state machine that coordinates multi-axis limb choreography, track movement, and sound playback without freezing the network loop.
- **`CameraManager` (`CameraManager.h` / `.cpp`):** Operates the OV2640 camera in a power-saving **on-demand** architecture (`esp_camera_init` / `esp_camera_deinit`). Isolates the 10 MHz XCLK bus to eliminate 2.4 GHz Wi-Fi interference and brownout resets during continuous driving.
- **`AudioPlayer` (`AudioPlayer.h` / `.cpp`):** Background I2S stream decoder using LittleFS/SD storage to route MP3 sound effects to the MAX98357A amplifier.
- **Telemetry & Safety:** Reads real-time bus voltage and current via the INA219 sensor to trigger software-side low-voltage warnings and throttle motor peaks.

## Getting Started

### Prerequisites
- [PlatformIO IDE](https://platformio.org/) (VS Code extension).
- ESP32-S3 development board support installed.

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/esp32-s3-walle.git
   ```

## Configuration & Setup

### 1. Wi-Fi Credentials
Set your local network credentials directly in `src/main.cpp` (or inside a local untracked `include/secrets.h` file) before compiling:

```cpp
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

### 2. Audio Files (LittleFS)
Due to copyright restrictions, audio tracks are not included in this repository. Place your own `.mp3` files in the `data/` directory matching these filenames before running **Upload Filesystem Image** in PlatformIO:
- `/1_WALL-E.mp3`
- `/2_EEEEE-VA.mp3`
- `/3_WALL-EEEEE.mp3`
- `/4_Hello Dolly.mp3`

## Roadmap
- [x] Custom baseboard design and soldering.
- [x] I2C bus initialization and power monitoring tests.
- [x] LittleFS filesystem and I2S audio integration.
- [x] 3D printing of the chassis, tracks, and mechanical assembly.
- [x] Code refactoring to clean OOP (Object-Oriented Programming).
- [x] Creating and tuning servo animations.
- [x] On-demand OV2640 camera integration and power optimization.

## Credits & Attribution
- **3D Model:** The mechanical design and 3D printable files are based on the [WALL-E Robot Replica by chillibasket (Simon Bluett)](https://www.thingiverse.com/thing:3703555) on Thingiverse (Author's website: [wired.chillibasket.com](https://wired.chillibasket.com)).
- **Custom Hardware & Electronics:** Custom baseboard, power distribution network, and electronics mounting brackets designed specifically for ESP32-S3 and high-current 2S battery integration.
- **Firmware:** Custom OOP implementation built from scratch for WebSocket control and autonomous combos.

## Disclaimer
*WALL-E is a registered trademark of Disney/Pixar. This repository contains a non-commercial, fan-made educational robotics project.*