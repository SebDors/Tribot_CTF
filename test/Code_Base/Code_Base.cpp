int joystick_3_y = 0;

#include "Arduino.h"
#include <Servo.h>

Servo ServoMoteur;
// Choix du pin du servomoteur
static const int servoPin = 14;

int Angle = 0;

void setup()
{
  // On initialise la connexion série
  Serial.begin(115200);
  // Attacher le pin du servomoteur à la broche
  ServoMoteur.attach(servoPin);
}

void loop()
{
  // Ajout d'un delay
  delay(10);
  // Changement de l'angle du servo moteur
  Angle = map(joystick_3_y, -100, 100, 100, 18);
}