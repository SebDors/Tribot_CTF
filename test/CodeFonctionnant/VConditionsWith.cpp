#include "Remote.h"
#include <Servo.h>

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
int FrequenceA = 1000;
int FrequenceB = 1000;
int FrequenceC = 1000;
// Etablir la résolution à 8bits
const int Resolution = 8;

const float Sqrt3Demi = sqrt(3) * 0.5;
const float DCentreRoue = 13; // Distance entre le centre du robot et les roues = 13cm

int VMotA;
int VMotB;
int VMotC;

Servo ServoMoteur;
static const int servoPin = 14;

int NewAngle = 0;
int OldAngle = 0;

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
    ServoMoteur.attach(servoPin);
}

void loop()
{
    int Vy = joy_left_y;
    int Vx = joy_left_x;
    int Om = joy_right_x;
    int Puissance = map(joy_right_y, -100, 100, 0, 300);

    if (Vy >= 50)
    {
        VMotA = -10;
        VMotB = 10;
        VMotC = 0;
        // Serial.println("J'avance");
    }
    else if (Vy <= -50)
    {
        VMotA = 10;
        VMotB = -10;
        VMotC = 0;
        // Serial.println("Je recule");
    }
    else if (Vx >= 50)
    {
        VMotA = -5;
        VMotB = -5;
        VMotC = 10;
        // Serial.println("Je slide a droite");
    }
    else if (Vx <= -50)
    {
        VMotA = 5;
        VMotB = 5;
        VMotC = -10;
        // Serial.println("Je slide a gauche");
    }
    else if (Om >= 50)
    {
        VMotA = -10;
        VMotB = -10;
        VMotC = -10;
        // Serial.println("Rotation Droite");
    }
    else if (Om <= -50)
    {
        VMotA = 10;
        VMotB = 10;
        VMotC = 10;
        // Serial.println("Rotation Gauche");
    }
    if (joystick_3_y >= 50)
    {
        ServoMoteur.write(18);
    }
    else if (joystick_3_y <= -50)
    {
        ServoMoteur.write(100);
    }
    else
    {
        VMotA = 0;
        VMotB = 0;
        VMotC = 0;
    }

    FrequenceA = VMotA * Puissance;
    FrequenceB = VMotB * Puissance;
    FrequenceC = VMotC * Puissance;
    // Contrôle des freins
    digitalWrite(en_pin_A, FrequenceA == 0 ? HIGH : LOW);
    digitalWrite(en_pin_B, FrequenceB == 0 ? HIGH : LOW);
    digitalWrite(en_pin_C, FrequenceC == 0 ? HIGH : LOW);
    // Contrôle des directions
    digitalWrite(dir_pin_A, FrequenceA > 0 ? HIGH : LOW);
    digitalWrite(dir_pin_B, FrequenceB > 0 ? HIGH : LOW);
    digitalWrite(dir_pin_C, FrequenceC > 0 ? HIGH : LOW);
    // Contrôle de la vitesse (Garder la frequence en valeur absolue)
    ledcWriteTone(PWM_channel_A, abs(FrequenceA));
    ledcWriteTone(PWM_channel_B, abs(FrequenceB));
    ledcWriteTone(PWM_channel_C, abs(FrequenceC));
    // ReadAngle();
    //  Ajout d'un delay
    delay(10);
}

void ReadAngle()
{
    // Changement de l'angle du servo moteur
    NewAngle = map(joystick_3_y, -100, 100, 100, 18);
    if (NewAngle != OldAngle)
    {
        ServoMoteur.write(NewAngle);
        Serial.print("Angle : ");
        Serial.println(NewAngle);
        OldAngle = NewAngle;
    }
}
