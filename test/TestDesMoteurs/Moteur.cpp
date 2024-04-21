#include <Arduino.h>
const int en_pin = 26;
const int step_pin = 2;
const int dir_pin = 15;
int pas = 0;
int tour = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello, ESP32!");
  pinMode(en_pin, OUTPUT);
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);

  digitalWrite(en_pin, 0);
  digitalWrite(step_pin, 0);
  digitalWrite(dir_pin, 0);
}

void loop()
{
  digitalWrite(en_pin, LOW);
  digitalWrite(step_pin, HIGH);
  delay(2);
  digitalWrite(step_pin, LOW);
  // put your main code here, to run repeatedly:
  delay(2.67); // this speeds up the simulation
  pas += 1;
  if (pas == 200)
  {
    pas = 0;
    tour += 1;
    Serial.print("Un tour de passee : ");
    Serial.println(tour);
  }
}
