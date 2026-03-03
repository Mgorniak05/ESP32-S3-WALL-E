# ESP32-S3 Custom Rover

Hi! This is the repository for my custom-built mobile robot. I'm building it from scratch to get away from ready-made toys and Arduino IDE "spaghetti code." I've switched to **PlatformIO** and clean, object-oriented **C++**, with the ultimate goal of using this platform to learn ROS 2 (micro-ROS).

## Hardware Setup
The robot uses a standard differential drive setup, but I've put a lot of effort into the electronics:
* **Brain:** ESP32-S3 (16MB Flash, 8MB PSRAM version) 
* **Drive:** 2x small and agile N20 gear motors (300 RPM) controlled by a TB6612FNG driver
* **Eyes:** OV2640 camera on an SG90 servo (pan/tilt) + a VL53L0X laser distance sensor (ToF) via I2C
* **Power:** A custom 2S (7.4V) 18650 battery pack hooked up to a dedicated BMS and a **USB-C charging module**. I can just plug the robot into a phone charger without taking the batteries out.

## Software Architecture
Everything is written in C++. I'm splitting the logic into specific classes to keep things tidy:
* `MotorController` - handles PWM signals and direction logic for the TB6612 driver.
* `Sensors` - manages the laser and other peripherals.
* *(more modules coming soon)*

## Roadmap
- Assemble the chassis and run drive tests.
- Handle WiFi video streaming from the ESP-CAM.
- Integrate the platform with micro-ROS.