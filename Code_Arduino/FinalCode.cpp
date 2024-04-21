#include "Remote.h"
#include <Servo.h>

// Servomoteur
Servo ServoMoteur;
static const int servoPin = 12;
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
// Moteur C
const int en_pin_C = 19;
const int step_pin_C = 18;
const int dir_pin_C = 5;
const int PWM_channel_C = 4;
float VMotC = 0;

// Etablir les fréquences à 1kHz
float FrequenceA = 1000;
float FrequenceB = 1000;
float FrequenceC = 1000;
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
float Avant = 0;
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
    // Moteur C
    pinMode(en_pin_C, OUTPUT);
    pinMode(step_pin_C, OUTPUT);
    pinMode(dir_pin_C, OUTPUT);
    // Paramétrer les pins PWM avec leurs fréquences et leurs résolutions
    ledcSetup(PWM_channel_A, FrequenceA, Resolution);
    ledcSetup(PWM_channel_B, FrequenceB, Resolution);
    ledcSetup(PWM_channel_C, FrequenceC, Resolution);
    // Rattacher les pins step avec leurs pins PWM
    ledcAttachPin(step_pin_A, PWM_channel_A);
    ledcAttachPin(step_pin_B, PWM_channel_B);
    ledcAttachPin(step_pin_C, PWM_channel_C);

    ServoMoteur.attach(servoPin, 6);
}

// Création d'une fonction map permettant l'utilisation des float
float mapFloat(float value, float fromLow, float fromHigh, float toLow, float toHigh)
{
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

void loop()
{
    Vy = joy_left_y; // récupération de la valeur entre -100 <= Vy <= 100
    Vx = joy_left_x; // récupération de la valeur entre -100 <= Vx <= 100
    if (Vx >= -20 && Vx <= 20)
    {
        Vx = 0;
    }
    Om = map(joy_right_x, -100, 100, 100, -100); // récupération de la valeur entre -100 <= Om <= 100
    if (Om >= -30 && Om <= 30)
    {
        Om = 0;
    }
    throttle = mapFloat(joy_right_y, -100, 100, 0, 100) / 100;   // Récupération de la valeur 0 <= throttle <= 1
    VMotA = (-0.5 * Vx) + (Sqrt3Demi * Vy) + (DCentreRoue * Om); // Utilisation de la formule pour % du MotA
    VMotB = (-0.5 * Vx) - (Sqrt3Demi * Vy) + (DCentreRoue * Om); // Utilisation de la formule pour % du MotB
    VMotC = Vx + (DCentreRoue * Om);                             // Utilisation de la formule pour % du MotC

    // Trouver la valeur la plus haute
    Avant = max(max(abs(VMotA), abs(VMotB)), abs(VMotC));

    if (Avant == 0)
    {
        FrequenceA = 0;
        FrequenceB = 0;
        FrequenceC = 0;
    }
    else
    {
        FrequenceA = mapFloat(VMotA, -Avant, Avant, -Apres, Apres) * throttle;
        FrequenceB = mapFloat(VMotB, -Avant, Avant, -Apres, Apres) * throttle;
        FrequenceC = mapFloat(VMotC, -Avant, Avant, -Apres, Apres) * throttle;
    }
    // Controle des freins
    digitalWrite(en_pin_A, FrequenceA >= -30 && FrequenceA <= 30 ? HIGH : LOW);
    digitalWrite(en_pin_B, FrequenceB >= -30 && FrequenceB <= 30 ? HIGH : LOW);
    digitalWrite(en_pin_C, FrequenceC >= -30 && FrequenceC <= 30 ? HIGH : LOW);
    // Controle des directions
    digitalWrite(dir_pin_A, VMotA >= 0 ? HIGH : LOW);
    digitalWrite(dir_pin_B, VMotB >= 0 ? HIGH : LOW);
    digitalWrite(dir_pin_C, VMotC >= 0 ? HIGH : LOW);
    // Controle de la vitesse (Garder la frequence en valeur absolue)
    ledcWriteTone(PWM_channel_A, abs(FrequenceA));
    ledcWriteTone(PWM_channel_B, abs(FrequenceB));
    ledcWriteTone(PWM_channel_C, abs(FrequenceC));
    // Changement de l'angle du servo moteurs
    Angle = map(joystick_3_y, -100, 100, 180, 18);
    ServoMoteur.write(Angle);
    // Ajout d'un delay
    delay(10);
}