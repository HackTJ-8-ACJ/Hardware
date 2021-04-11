#include <Arduino.h>
// ultrasonic
#include <HCSR04.h>
// WiFi/NTP
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <SPI.h>
#include <WiFiUdp.h>

#include "secrets.h"

const int trigger_pin = 14;
const int echo_pin = 12;
const int cooldown = 1000;

int last_cooldown;

// TODO: account for DST dynamically
const long utc_offset = -18000 + 3600;

UltraSonicDistanceSensor sonar(trigger_pin, echo_pin);

WiFiUDP udp_ntp_connection;
NTPClient time_client(udp_ntp_connection, "pool.ntp.org", utc_offset);

void setup() {
    Serial.begin(115200);
    Serial.println("trying...");
    delay(10);

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WiFi_ssid);
    WiFi.begin((char*)WiFi_ssid, WiFi_password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    time_client.begin();
    Serial.println(WiFi_ssid);
    Serial.println(WiFi_password);

    // initialize timers
    last_cooldown = millis();
}

void loop() {
    if (millis() - last_cooldown > cooldown) {
        float distance_cm = sonar.measureDistanceCm();
        float distance_ft = distance_cm / 2.54 / 12;

        if (distance_ft <= 2 && distance_cm > 2) {
            // 2 ft < distance < 2 cm
            time_client.update();
            // epoch has been tested using <Time.h> and works properly
            unsigned long epoch = time_client.getEpochTime();
            Serial.println(String("Within 2 ft at ") + epoch);
        }

        // put on cooldown
        last_cooldown = millis();
    }
}
