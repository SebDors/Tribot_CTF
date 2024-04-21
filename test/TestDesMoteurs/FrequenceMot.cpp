#include "Arduino.h"
// Moteur A
const int en_pin_A = 26;
const int step_pin_A = 2;
const int dir_pin_A = 15;
const int PWM_channel_A = 0;
// Moteur B
const int en_pin_B = 23;
const int step_pin_B = 22;
const int dir_pin_B = 21;
const int PWM_channel_B = 2;
// Moteur C
const int en_pin_C = 19;
const int step_pin_C = 18;
const int dir_pin_C = 5;
const int PWM_channel_C = 4;

// Etablir les fréquences à 1kHz
int Frequence = 1000;
// Etablir la résolution à 8bits
const int Resolution = 8;

const float Sqrt3Demi = sqrt(3) * 0.5;
const float DCentreRoue = 13; // Distance entre le centre du robot et les roues = 13cm

int VMotA;
int VMotB;
int VMotC;
void setup()
{
    // On initialise la connexion série
    Serial.begin(115200);
    // Moteur A
    pinMode(en_pin_A, OUTPUT);
    pinMode(step_pin_A, OUTPUT);
    pinMode(dir_pin_A, OUTPUT);
    // Moteur B
    pinMode(en_pin_B, OUTPUT);
    pinMode(step_pin_B, OUTPUT);
    pinMode(dir_pin_B, OUTPUT);
    // Moteur C
    pinMode(en_pin_C, OUTPUT);
    pinMode(step_pin_C, OUTPUT);
    pinMode(dir_pin_C, OUTPUT);
    // Paramétrer les pins PWM avec leurs fréquences et leurs résolutions
    ledcSetup(PWM_channel_A, Frequence, Resolution);
    ledcSetup(PWM_channel_B, Frequence, Resolution);
    ledcSetup(PWM_channel_C, Frequence, Resolution);
    // Rattacher les pins step avec leurs pins PWM
    ledcAttachPin(step_pin_A, PWM_channel_A);
    ledcAttachPin(step_pin_B, PWM_channel_B);
    ledcAttachPin(step_pin_C, PWM_channel_C);
}

void loop()
{
    ledcWriteTone(PWM_channel_B, Frequence);
    digitalWrite(dir_pin_B, LOW);
    Serial.println("Low");
    delay(1000);
}
