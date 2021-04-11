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

// NYC time zone
const long utc_offset = -18000;

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
    time_client.update();

    // dynamic daylight savings
    int epoch = time_client.getEpochTime();
    bool daylight_savings = false;
    if (month(epoch) > 3 && month(epoch) < 11) {
        daylight_savings = true;
    } else if (month(epoch) == 3) {
        // DST starts on March 14 at 2am
        if (day(epoch) > 14 || (day(epoch) == 14 && hour(epoch) >= 2))
            daylight_savings = true;
    } else if (month(epoch) == 11) {
        // DST ends on November 7 at 2am
        if (day(epoch) > 7 || (day(epoch) == 7 && hour(epoch) >= 2))
            daylight_savings = true;
    }

    if (daylight_savings) time_client.setTimeOffset(utc_offset + SECS_PER_HOUR);

    Serial.println(String("") + day(epoch) + ":" + month(epoch) + ":" +
                   year(epoch));
    // time_client.setTimeOffset();
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
