#include <Arduino.h>

// Moteur A
const int en_pin_A = 23;
const int step_pin_A = 22;
const int dir_pin_A = 21;
const int PWM_channel_A = 0;
float VMotA = 0;
// Moteur B
const int en_pin_B = 19;
const int step_pin_B = 18;
const int dir_pin_B = 5;
const int PWM_channel_B = 2;
float VMotB = 0;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("Hello, ESP32!");
    pinMode(en_pin_A, OUTPUT);
    pinMode(step_pin_A, OUTPUT);
    pinMode(dir_pin_A, OUTPUT);

    pinMode(en_pin_B, OUTPUT);
    pinMode(step_pin_B, OUTPUT);
    pinMode(dir_pin_B, OUTPUT);
}

void loop()
{
    // On fait avancer le robot
    digitalWrite(step_pin_A, HIGH);
    delay(1);
    digitalWrite(step_pin_A, LOW);
    delay(1);
}
