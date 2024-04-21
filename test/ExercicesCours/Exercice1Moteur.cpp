#include "Arduino.h"
const int PWM_channel_left = 0; // On choisit le canal 0 pour le moteur de gauche
const int resolution = 8;       // Résolution de 8 bits, dans notre cas ça n'a pas ou peu d'importance

int frequence_left = 1000; // Nous initialisons la fréquence PWM du moteur de gauche à 1 KHz.
                           // Notre moteur fera donc 5 tours par seconde

const int dir_pin_left = 21;  // Constante contenant le numéro de broche sur laquelle est branchée l'entrée DIR du driver
const int step_pin_left = 22; // Constante contenant le numéro de broche sur laquelle est branchée l'entrée STEP du driver
const int en_pin_left = 23;   // Constante contenant le numéro de broche sur laquelle est branchée l'entrée ENABLE du driver

// Idem pour les boutons
const int button_up = 27;
const int button_down = 14;

bool button_up_state = false;
bool button_down_state = false;

long last_buttons_checking = 0;

int next_speed_percent = 0; // Nous déclarons une variable qui nous permettra de stocker la nouvelle vitesse en pourcent
                            // Nous l'initialisons à 0. Sa valeur sera comprise entre -100 et 100
                            // Si sa valeur est inférieure à 0, nous tournerons dans le sens anti-horaire

int current_speed_percent = 0; // Nous déclarons une variable qui nous permettra de stocker la valeur actuelle (courante)
                               // de notre vitesse en pourcent.
                               // Cette variable nous permettra de comparer la nouvelle vitesse avec l'actuelle.
                               // Si celles-ci sont différentes nous mettrons à jour la vitesse actuelle
                               // sinon nous sauterons les instructions de la condition afin de ne pas exécuter
                               // d'instructions inutiles (économie des ressources).

void setup()
{
    // On initialise la communication série (baudrate = 115200)
    Serial.begin(115200);

    // Nous paramétrons le rôle des broches sur lesquelles sont branchées nos boutons :
    // elles seront paramétrées en ENTREE [INPUT] avec une résistance de tirage vers le haut [PULLUP],
    // c'est à dire que digitalRead() renverra HIGH lorsque le bouton ne sera pas pressé (et LOW lorsque il le sera)
    pinMode(button_up, INPUT_PULLUP);
    pinMode(button_down, INPUT_PULLUP);

    // Nous paramétrons les broches permettant de piloter le moteur de gauche
    // On paramètre notre broche "DIR" en sortie
    pinMode(dir_pin_left, OUTPUT);
    // On paramètre notre broche "STEP" en sortie
    //    pinMode(step_pin_left,OUTPUT);
    // On paramètre notre broche "EN" en sortie
    pinMode(en_pin_left, OUTPUT);
    // On désactive l'alimentation de notre moteur (souvenez-vous l'entrée Enable du driver est en logique inverse)
    digitalWrite(en_pin_left, HIGH);

    // On paramètre la sortie PWM qui pilote la broche "STEP" du moteur "left"
    ledcSetup(PWM_channel_left, frequence_left, resolution);
    // On lie la sortie PWM à la broche "STEP" du moteur "left"
    ledcAttachPin(step_pin_left, PWM_channel_left);
}

void loop()
{

    // Dans un premier temps nous occupons de nos boutons
    // Cette première condition, nous permet de vérifier l'état de nos boutons toutes les 50ms.
    // Gardez cette "technique" sous le coude elle vous reservira ultérieurement car elle permet d'éviter
    // l'utilisation de la fonction delay() et d'avoir un programme "multitâches" (donnant l'illusion d'exécuter plusieurs choses à la fois)

    // Elle fonctionne de la manière suivante :
    // - On soustrait le temps écoulé actuel (renvoyé par millis()) à la dernière valeur connue de la variable last_buttons_checking
    // - Si le résultat est supérieur ou égal à 50 (ms) alors on exécute les instructions

    if (millis() - last_buttons_checking >= 50)
    {
        if (digitalRead(button_up) == LOW && button_up_state == false)
        {
            // Nous stockons le nouvel état du bouton, cela nous servira à n'incrémenter qu'une seule fois
            button_up_state = true;

            // Nous incrémentons la vitesse de 10%
            next_speed_percent += 10;
            if (next_speed_percent > 100)
            {
                next_speed_percent = 100;
            }
            // Nous affichons la nouvelle valeur
            Serial.println(next_speed_percent);
        }
        else if (digitalRead(button_up) == HIGH && button_up_state == true)
        {
            button_up_state = false;
        }

        if (digitalRead(button_down) == LOW && button_down_state == false)
        {
            // Nous stockons le nouvel état du bouton, cela nous servira à descendre qu'une seule fois
            button_up_state = true;

            // Nous déscendons la vitesse de 10%
            next_speed_percent -= 10;
            if (next_speed_percent < -100)
            {
                next_speed_percent = -100;
            }
            // Nous affichons la nouvelle valeur
            Serial.println(next_speed_percent);
        }
        else if (digitalRead(button_down) == HIGH && button_down_state == true)
        {
            button_up_state = false;
        }

        // On met à jour la valeur de la variable last_buttons_checking,
        // au moins notre toute première condition pourra à nouveau être validé au bout de 50ms
        last_buttons_checking = millis();
    }

    // Maintenant nous allons nous occuper de notre moteur

    // Si les valeurs de next_speed_percent et current_speed_percent sont différentes et que la valeur de next_speed_percent
    // est différente de 0 alors nous exécutons les instructions contenues entres les accolades de notre condition.
    // Ainsi nous économisons les ressources de notre microcontrôleur pour exécuter d'autres instructions.
    if (next_speed_percent != current_speed_percent)
    {

        if (next_speed_percent != 0)
        {
            // Nous déclarons la variable freq qui stockera la valeur de notre fréquence.
            // Nous l'initialisons à 0
            int freq = 0;

            // Nous utilisons la fonction map() afin de convertir la valeur absolue de "next_speed_percent" => abs(next_speed_percent)
            // comprise sur la plage de valeur 0 à 100 en sa valeur comprise sur l'intervalle 0.
            // Nous mettons à jour la valeur de freq
            freq = map(abs(next_speed_percent), 0, 100, 0, frequence_left);
            // Maintenant nous allons nous occuper du sens de rotation
            // Si current_speed_percent est négatif nous faisons tourner notre moteur dans le sens anti-horaire
            if (next_speed_percent < 0)
            {
                digitalWrite(dir_pin_left, LOW);
            }
            else
            {
                digitalWrite(dir_pin_left, HIGH);
            }

            // Nous activons l'alimentation électrique du moteur "left"
            digitalWrite(en_pin_left, LOW);

            // Nous modifions la fréquence de la broche PWM correspondant au moteur "left"
            ledcWriteTone(PWM_channel_left, freq);
        }
        else
        {
            // Nous déactivons l'alimentation de notre moteur.
            digitalWrite(en_pin_left, HIGH);
        }

        // Nous mettons à jour la valeur de current_speed_percent avec la valeur de next_speed_percent
        current_speed_percent = next_speed_percent;
    }
}