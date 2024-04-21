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
const int en_pin_B = 26;
const int step_pin_B = 2;
const int dir_pin_B = 15;
const int PWM_channel_B = 2;
float VMotB = 0;

// Etablir les fréquences à 1kHz
float FrequenceA = 1000;
float FrequenceB = 1000;
// Etablir la résolution à 8bits
const int Resolution = 8;

const float Sqrt3Demi = sqrt(3) * 0.5; // Généralisation d'une valeur nous servant plus tard
const float DCentreRoue = 0.13;        // Distance entre le centre du robot et les roues = 13cm

// Initialisation des valeurs en X, Y, rotation Omega et gaz des moteurs
float Vy = 0;
float Vx = 0;
float Om = 0;
float throttle = 0;

// Initialisation des valeurs afin de changer la plage de fréquences
float Avant = 100;
const float Apres = 3000;
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
    throttle = mapFloat(joy_right_y, -100, 100, 0, 100) / 100; // Récupération de la valeur 0 <= throttle <= 1
    Vy = joy_left_y;
    Vx = joy_left_x;

    if (Vx >= 10)
    {
        VMotA = (Vy - Vx);
        VMotB = Vy;
    }
    if (Vx <= -10)
    {
        VMotA = Vy;
        VMotB = (Vy + Vx);
    }

    FrequenceA = mapFloat(VMotA, -Avant, Avant, -Apres, Apres) * throttle;
    FrequenceB = mapFloat(VMotB, -Avant, Avant, -Apres, Apres) * throttle;

    digitalWrite(en_pin_A, FrequenceA == 0 ? HIGH : LOW);
    digitalWrite(en_pin_B, FrequenceB == 0 ? HIGH : LOW);
    // Controle des directions
    digitalWrite(dir_pin_A, FrequenceA >= 0 ? HIGH : LOW);
    digitalWrite(dir_pin_B, FrequenceB >= 0 ? HIGH : LOW);
    // Controle de la vitesse (Garder la frequence en valeur absolue)
    ledcWriteTone(PWM_channel_A, abs(FrequenceA));
    ledcWriteTone(PWM_channel_B, abs(FrequenceB));
    // Changement de l'angle du servo moteurs
    Angle = map(joy_right_x, -100, 100, 180, 18);
    ServoMoteur.write(Angle);
    // Ajout d'un delay
    delay(10);
}