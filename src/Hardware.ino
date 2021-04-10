#include <Arduino.h>
#include <HCSR04.h>

const int trigger_pin = 14;
const int echo_pin = 12;
const int max_distance = 400;

UltraSonicDistanceSensor sonar(trigger_pin, echo_pin);

void setup() { Serial.begin(115200); }

void loop() {
    float distance_cm = sonar.measureDistanceCm();
    // black magic to get distance in feet
    // float distance_cm = (duration / 2) * 0.0343;
    float distance_ft = distance_cm / 2.54 / 12;

    if (distance_ft <= 2 && distance_cm > 2) {
        // 2 ft < distance < 2 cm
        Serial.println("Within 2 ft!");
    }

    delay(500);
}
