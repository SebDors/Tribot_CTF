#include "Arduino.h"
const int PWM_channel_left = 0;  // On choisit le canal 0 pour le moteur de gauche
const int PWM_channel_right = 2; // On choisit le canal 2 pour le moteur de droite
const int resolution = 8;        // Résolution de 8 bits, dans notre cas ça n'a pas ou peu d'importance

int frequence_left = 1000; // Nous initialisons la fréquence PWM du moteur de gauche à 1 KHz.
                           // Notre moteur fera donc 5 tours par seconde

int frequence_right = 1000; // Nous initialisons la fréquence PWM du moteur de droite à 1 KHz
                            // Notre moteur fera donc 5 tours par seconde

const int dir_pin_left = 21;  // Constante contenant le numéro de broche sur laquelle est branchée l'entrée DIR du driver
const int step_pin_left = 22; // Constante contenant le numéro de broche sur laquelle est branchée l'entrée STEP du driver
const int en_pin_left = 23;   // Constante contenant le numéro de broche sur laquelle est branchée l'entrée ENABLE du driver

const int dir_pin_right = 15; // Constante contenant le numéro de broche sur laquelle est branchée l'entrée DIR du driver
const int step_pin_right = 2; // Constante contenant le numéro de broche sur laquelle est branchée l'entrée STEP du driver
const int en_pin_right = 26;  // Constante contenant le numéro de broche sur laquelle est branchée l'entrée ENABLE du driver

// Idem pour les boutons
const int button_up = 27;
const int button_down = 14;
const int button_right = 12;
const int button_left = 13;

void setup()
{
    // On initialise la communication série (baudrate = 115200)
    Serial.begin(115200);

    // Nous paramétrons le rôle des broches sur lesquelles sont branchées nos boutons :
    // elles seront paramétrées en ENTREE [INPUT] avec une résistance de tirage vers le haut [PULLUP],
    // c'est à dire que digitalRead() renverra HIGH lorsque le bouton ne sera pas pressé (et LOW lorsque il le sera)
    pinMode(button_up, INPUT_PULLUP);
    pinMode(button_down, INPUT_PULLUP);
    pinMode(button_left, INPUT_PULLUP);
    pinMode(button_right, INPUT_PULLUP);

    // Nous paramétrons les broches permettant de piloter le moteur de gauche
    // On paramètre notre broche "DIR" en sortie
    pinMode(dir_pin_left, OUTPUT);
    // On paramètre notre broche "STEP" en sortie
    //    pinMode(step_pin_left,OUTPUT);
    // On paramètre notre broche "EN" en sortie
    pinMode(en_pin_left, OUTPUT);
    // On désactive l'alimentation de notre moteur (souvenez-vous l'entrée Enable du driver est en logique inverse)
    digitalWrite(en_pin_left, HIGH);

    pinMode(dir_pin_right, OUTPUT);
    pinMode(en_pin_right, OUTPUT);
    digitalWrite(en_pin_right, HIGH);

    // On paramètre la sortie PWM qui pilote la broche "STEP" du moteur "left"
    ledcSetup(PWM_channel_left, frequence_left, resolution);
    ledcSetup(PWM_channel_right, frequence_right, resolution);
    // On lie la sortie PWM à la broche "STEP" du moteur "left"
    ledcAttachPin(step_pin_left, PWM_channel_left);
    ledcAttachPin(step_pin_right, PWM_channel_right);
}

void Avancer()
{
    // Nous modifions la fréquence de la broche PWM correspondant au moteur "left"
    ledcWriteTone(PWM_channel_left, frequence_left);
    ledcWriteTone(PWM_channel_right, frequence_right);
    // Nous activons l'alimentation électrique du moteur "left"
    digitalWrite(en_pin_left, LOW);
    digitalWrite(en_pin_right, LOW);
    // Conformément à ce qui est indiqué dans le diagramme qui vous est fourni, nous passons l'état de la broche
    // gérant le sens de rotation à "HIGH"
    digitalWrite(dir_pin_left, HIGH);
    digitalWrite(dir_pin_right, LOW);
    // Nous utilisons un délai afin de permettre le bon fonctionnement du programme
    // Sans ce-dernier celui-ci ne fonctionne pas convenablement
    delay(50);
}
void Reculer()
{ // Nous modifions la fréquence de la broche PWM correspondant au moteur "left"
    ledcWriteTone(PWM_channel_left, frequence_left);
    ledcWriteTone(PWM_channel_right, frequence_right);
    // Nous activons l'alimentation électrique du moteur "left"
    digitalWrite(en_pin_left, LOW);
    digitalWrite(en_pin_right, LOW);
    // Conformément à ce qui est indiqué dans le diagramme qui vous est fourni, nous passons l'état de la broche
    // gérant le sens de rotation à "HIGH"
    digitalWrite(dir_pin_left, LOW);
    digitalWrite(dir_pin_right, HIGH);
    // Nous utilisons un délai afin de permettre le bon fonctionnement du programme
    // Sans ce-dernier celui-ci ne fonctionne pas convenablement
    delay(50);
}
void DemiTourGauche()
{
    // Nous modifions la fréquence de la broche PWM correspondant au moteur "left"
    ledcWriteTone(PWM_channel_left, frequence_left);
    ledcWriteTone(PWM_channel_right, frequence_right);
    // Nous activons l'alimentation électrique du moteur "left"
    digitalWrite(en_pin_left, LOW);
    digitalWrite(en_pin_right, LOW);
    // Conformément à ce qui est indiqué dans le diagramme qui vous est fourni, nous passons l'état de la broche
    // gérant le sens de rotation à "HIGH"
    digitalWrite(dir_pin_left, HIGH);
    digitalWrite(dir_pin_right, HIGH);
    // Nous utilisons un délai afin de permettre le bon fonctionnement du programme
    // Sans ce-dernier celui-ci ne fonctionne pas convenablement
    delay(50);
}
void DemiTourDroite()
{
    // Nous modifions la fréquence de la broche PWM correspondant au moteur "left"
    ledcWriteTone(PWM_channel_left, frequence_left);
    ledcWriteTone(PWM_channel_right, frequence_right);
    // Nous activons l'alimentation électrique du moteur "left"
    digitalWrite(en_pin_left, LOW);
    digitalWrite(en_pin_right, LOW);
    // Conformément à ce qui est indiqué dans le diagramme qui vous est fourni, nous passons l'état de la broche
    // gérant le sens de rotation à "HIGH"
    digitalWrite(dir_pin_left, LOW);
    digitalWrite(dir_pin_right, LOW);
    // Nous utilisons un délai afin de permettre le bon fonctionnement du programme
    // Sans ce-dernier celui-ci ne fonctionne pas convenablement
    delay(50);
}

void AvancerGauche()
{
    // Nous vérifions si le bouton correspondant à l'action souhaitée est pressé
    // Si c'est le cas nous exécutons les instructions contenues entres les accolades de notre condition
    frequence_left = 600;
    // Nous modifions la fréquence de la broche PWM correspondant au moteur "left"
    ledcWriteTone(PWM_channel_left, frequence_left);
    ledcWriteTone(PWM_channel_right, frequence_right);
    // Nous activons l'alimentation électrique du moteur "left"
    digitalWrite(en_pin_left, LOW);
    digitalWrite(en_pin_right, LOW);
    // Conformément à ce qui est indiqué dans le diagramme qui vous est fourni, nous passons l'état de la broche
    // gérant le sens de rotation à "HIGH"
    digitalWrite(dir_pin_left, HIGH);
    digitalWrite(dir_pin_right, LOW);
    frequence_left = 1000;
    // Nous utilisons un délai afin de permettre le bon fonctionnement du programme
    // Sans ce-dernier celui-ci ne fonctionne pas convenablement
    delay(50);
}
void AvancerDroite()
{
    // Nous vérifions si le bouton correspondant à l'action souhaitée est pressé
    // Si c'est le cas nous exécutons les instructions contenues entres les accolades de notre condition

    frequence_right = 600;
    // Nous modifions la fréquence de la broche PWM correspondant au moteur "left"
    ledcWriteTone(PWM_channel_left, frequence_left);
    ledcWriteTone(PWM_channel_right, frequence_right);
    // Nous activons l'alimentation électrique du moteur "left"
    digitalWrite(en_pin_left, LOW);
    digitalWrite(en_pin_right, LOW);
    // Conformément à ce qui est indiqué dans le diagramme qui vous est fourni, nous passons l'état de la broche
    // gérant le sens de rotation à "HIGH"
    digitalWrite(dir_pin_left, HIGH);
    digitalWrite(dir_pin_right, LOW);
    frequence_right = 1000;
    // Nous utilisons un délai afin de permettre le bon fonctionnement du programme
    // Sans ce-dernier celui-ci ne fonctionne pas convenablement
    delay(50);
}
void ReculerGauche()
{
    // Nous vérifions si le bouton correspondant à l'action souhaitée est pressé
    // Si c'est le cas nous exécutons les instructions contenues entres les accolades de notre condition

    frequence_left = 600;
    // Nous modifions la fréquence de la broche PWM correspondant au moteur "left"
    ledcWriteTone(PWM_channel_left, frequence_left);
    ledcWriteTone(PWM_channel_right, frequence_right);
    // Nous activons l'alimentation électrique du moteur "left"
    digitalWrite(en_pin_left, LOW);
    digitalWrite(en_pin_right, LOW);
    // Conformément à ce qui est indiqué dans le diagramme qui vous est fourni, nous passons l'état de la broche
    // gérant le sens de rotation à "HIGH"
    digitalWrite(dir_pin_left, LOW);
    digitalWrite(dir_pin_right, HIGH);
    frequence_left = 1000;
    // Nous utilisons un délai afin de permettre le bon fonctionnement du programme
    // Sans ce-dernier celui-ci ne fonctionne pas convenablement
    delay(50);
}
void ReculerDroite()
{
    frequence_right = 600;
    // Nous modifions la fréquence de la broche PWM correspondant au moteur "left"
    ledcWriteTone(PWM_channel_left, frequence_left);
    ledcWriteTone(PWM_channel_right, frequence_right);
    // Nous activons l'alimentation électrique du moteur "left"
    digitalWrite(en_pin_left, LOW);
    digitalWrite(en_pin_right, LOW);
    // Conformément à ce qui est indiqué dans le diagramme qui vous est fourni, nous passons l'état de la broche
    // gérant le sens de rotation à "HIGH"
    digitalWrite(dir_pin_left, LOW);
    digitalWrite(dir_pin_right, HIGH);
    frequence_right = 1000;
    // Nous utilisons un délai afin de permettre le bon fonctionnement du programme
    // Sans ce-dernier celui-ci ne fonctionne pas convenablement
    delay(50);
}

void loop()
{
    // Test contenant 2 boutons
    if (digitalRead(button_up) == LOW && digitalRead(button_left) == LOW)
    {
        AvancerGauche();
    }
    if (digitalRead(button_up) == LOW && digitalRead(button_right) == LOW)
    {
        AvancerDroite();
    }
    if (digitalRead(button_down) == LOW && digitalRead(button_right) == LOW)
    {
        ReculerDroite();
    }
    if (digitalRead(button_down) == LOW && digitalRead(button_left) == LOW)
    {
        ReculerGauche();
    }

    // Test contenant qu'un seul bouton
    else if (digitalRead(button_up) == LOW)
    {
        Avancer();
    }
    else if (digitalRead(button_down) == LOW)
    {
        Reculer();
    }
    else if (digitalRead(button_left) == LOW)
    {
        DemiTourGauche();
    }
    else if (digitalRead(button_right) == LOW)
    {
        DemiTourDroite();
    }

    digitalWrite(en_pin_left, HIGH);
    digitalWrite(en_pin_right, HIGH);
    // Nous utilisons un délai afin de permettre le bon fonctionnement du programme
    // Sans ce-dernier celui-ci ne fonctionne pas convenablement
    delay(50);
}