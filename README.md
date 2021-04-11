# HackTJ - Hardware

This is the repository for the hardware portion of our HackTJ 8.0 project. The code controls an [Adafruit HUZZAH ESP8266](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266) microcontroller, which has onboard WiFi, and uses an [HC-SR04 Ultrasonic Sensor](https://www.sparkfun.com/products/15569).

## Usage

The code in this repo won't work as-is. It is a PlatformIO project, and needs to be built with that. Additionally, it requires a file called `secrets.h` to be present in the `src/` directory and `#define` values for both `WiFi_SSID` and `WiFi_PASSWORD` (in order for the ESP8266 chip to connect to WiFi). Don't ask for it please, since it has my WiFi SSID and password :)
