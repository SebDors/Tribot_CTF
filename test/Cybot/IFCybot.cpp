#include "Remote.h"
#include <Servo.h>

// Servomoteur
Servo ServoMoteur;
static const int servoPin = 14;
int Angle = 0;
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

// Etablir les fréquences à 1kHz
float FrequenceA = 1000;
float FrequenceB = 1000;
// Etablir la résolution à 8bits
const int Resolution = 8;

// Initialisation des valeurs en X, Y, rotation Omega et gaz des moteurs
float Vy = 0;
float Vx = 0;
float Om = 0;
float throttle = 0;

void setup()
{
    // On initialise la connexion série
    Serial.begin(115200);

    // On lance l'initialisation de tout ce qui est lié à la commande à distance (WiFi, websocket, serveur ...)
    // N'oubliez pas de modifier le nom et le mot de passe de votre réseau dans le fichier "Remote.h"
    initRemote();
    // Moteur A
    pinMode(en_pin_A, OUTPUT);
    pinMode(step_pin_A, OUTPUT);
    pinMode(dir_pin_A, OUTPUT);
    // Moteur B
    pinMode(en_pin_B, OUTPUT);
    pinMode(step_pin_B, OUTPUT);
    pinMode(dir_pin_B, OUTPUT);
    // Paramétrer les pins PWM avec leurs fréquences et leurs résolutions
    ledcSetup(PWM_channel_A, FrequenceA, Resolution);
    ledcSetup(PWM_channel_B, FrequenceB, Resolution);
    // Rattacher les pins step avec leurs pins PWM
    ledcAttachPin(step_pin_A, PWM_channel_A);
    ledcAttachPin(step_pin_B, PWM_channel_B);

    ServoMoteur.attach(servoPin, 6); // Le numéro 6 (ou 7) est le channel du timer non utilisé par les moteurs (Timer 3)
}

// Création d'une fonction map permettant l'utilisation des float
float mapFloat(float value, float fromLow, float fromHigh, float toLow, float toHigh)
{
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

void loop()
{
    digitalWrite(en_pin_A, LOW);
    digitalWrite(en_pin_A, LOW);
    throttle = mapFloat(joy_right_y, -100, 100, 0, 100) / 100; // Récupération de la valeur 0 <= throttle <= 1
    Vy = joy_left_y;
    Vx = joy_left_x;
    const int vitesse = 2000;
    // Update motor speeds based on Vx and Vy values
    if (Vy >= 50)
    {
        // If Vy is 100, wheels A and B move forward at vitesse but with an opposite direction
        FrequenceA = vitesse;
        FrequenceB = vitesse;
    }
    else if (Vy <= -50)
    {
        // If Vy is -100, wheels A and B move to vitesse but with an opposite direction of if Vx = 100
        FrequenceA = -vitesse;
        FrequenceB = -vitesse;
    }
    else if (Vx >= 50)
    {
        // If Vx is 100, wheels A and B advance to vitesse in the same direction
        FrequenceA = vitesse;
        FrequenceB = -vitesse;
    }
    else if (Vx <= -50)
    {
        // If Vx = -100, wheels A and B advance to vitesse in the opposite direction than if Vx = 100;
        FrequenceA = -vitesse;
        FrequenceB = vitesse;
    }
    else
    {
        FrequenceA = 0;
        FrequenceB = 0;
    }

    FrequenceA *= throttle;
    FrequenceB *= throttle;

    /*digitalWrite(en_pin_A, FrequenceA == 0 ? HIGH : LOW);
      digitalWrite(en_pin_B, FrequenceB == 0 ? HIGH : LOW);*/
    // Controle des directions
    digitalWrite(dir_pin_A, VMotA >= 0 ? HIGH : LOW);
    digitalWrite(dir_pin_B, VMotB >= 0 ? HIGH : LOW);
    // Controle de la vitesse (Garder la frequence en valeur absolue)
    ledcWriteTone(PWM_channel_A, abs(FrequenceA));
    ledcWriteTone(PWM_channel_B, abs(FrequenceB));
    // Changement de l'angle du servo moteurs
    Angle = map(joy_right_x, -100, 100, 180, 18);
    ServoMoteur.write(Angle);
    // Ajout d'un delay
    delay(10);
}