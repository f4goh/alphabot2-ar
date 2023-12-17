/*
 * test du robot : commandes élémentaires 
 * reset de l'afficheur SSD1306 (disable au niveau haut) (donc possible de faire une économie de broche en reliant directement au RESET de l'esp32)
 * la broche d/c est utilisée en tant que sélection d'adresse i2c et est laissée en l'air.  -> 0x3c
 * l'afficheur est scindé en 2 couleurs (bleu+jaune) avec une ligne séparatrice non affichable
 * Lors de la mise sous tension avec les 2 batteries, faire un reset esp32 manuel
 *
 * suivi de piste
 * blanc 960
 * gris 745
 * noir 250 
 */


#include <Arduino.h>
#include <Led.h>
#include <SSD1306Wire.h>
#include <TRSensors.h>
#include <Propulsion.h>
#include <PCF8574.h>



//GPIO ESP32 ne pas modifier
#define TRIG 25           //capteur ultra son
#define ECHO 26
#define IR 17            //télécomande IR
#define SPEED 40         //vitesse moteur 0-255


#define NBLED 4           // Définit le nombre de leds du ruban
#define DELAY 500         // Temps d'attente en ms pour cheniller
#define SEUIL 800       //seuil du suivi de piste entre noir et couleur marron ou gris




Led led(NBLED); //neopixels VRB

SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_64);

Propulsion prop(SPEED);        //gestion des moteurs
PCF8574 pcf;                   //capteurs de collision, beep et joystick

TRSensors trs = TRSensors();  //capteurs de suivi de ligne

unsigned int sensorValues[NUMSENSORS]; //5 capteurs de ligne indice de 0 gauche  à 4 droite



unsigned int ultraSound();
void afficheSenors();
void piste();
void motor_demo();



void setup() {
    Serial.begin(115200);

    led.setDelay(DELAY);        //Delay pour les leds RGB

    pinMode(TRIG, OUTPUT);      //Pin Mode pour le capteur ultrason
    pinMode(ECHO, INPUT);
    pinMode(IR, INPUT);

    pcf.begin();        //init E/S pour capteurs de collision, beep et joystick

    display.init();     //init affichage OLED
    display.flipScreenVertically();

/*
    led.allumer(ROUGE, 0); //Attention les leds RGB  consomment beaucoup
    led.allumer(VERT, 1);
    led.allumer(BLEU, 2);
    led.allumer(ROUGE, 3);
   */ 
    
    
}

void loop() {
    afficheSenors();
    if (!pcf.getJoyCenter()){  //actif au niveau BAS
        motor_demo();
    }
    if (!pcf.getIrDroit()){  //actif au niveau BAS
        led.cheniller(ROUGE,4);
    }
    if (!pcf.getIrGauche()){  //actif au niveau BAS
        led.cheniller(NOIR,4);
    }
}


/*
 lit et affiche l'état de capteurs suivants :
 * - suivi de piste (grandeur analogique compise entre 0 et 1023)
 * - collision latéral gauche et droit (0 collision , 1 : pas de collision)
 *   la distance de collision est réglable avec les potentiomètres bleus
 * - ultra-son en cm
 * - joystick actif au niveau bas
 */


void afficheSenors() {
    char buffer[30];
    display.clear();
    display.setFont(ArialMT_Plain_16);
    //affiche les capteurs de collision avant G et D
    sprintf(buffer, "IrG %d  -  IrD %d", pcf.getIrGauche(), pcf.getIrDroit());
    display.drawString(0, 0, buffer);

    //affiche les capteurs de suivi de ligne
    trs.AnalogReadSPI(sensorValues);
    display.setFont(ArialMT_Plain_10);
    sprintf(buffer, "%d %d %d %d %d", sensorValues[0], sensorValues[1], sensorValues[2], sensorValues[3], sensorValues[4]); //
    display.drawString(0, 20, buffer);

    //affiche le capteur ultra son avant
    sprintf(buffer, "distance US: %d ", ultraSound());
    display.drawString(0, 30, buffer);
    display.display();

    //affiche l'état des joystick
    sprintf(buffer, "joy: %d %d %d %d %d", pcf.getJoyBas(),pcf.getJoyDroit(),pcf.getJoyGauche(),pcf.getJoyHaut(),pcf.getJoyCenter());
    display.drawString(0, 40, buffer);
    display.display();

}

/*
 Exemple de suivi de piste
 */

void piste() {
    if (sensorValues[1] > SEUIL && sensorValues[2] < SEUIL && sensorValues[3] > SEUIL) {
        prop.avance();
    }
	//A Compléter
}

/*
 Demonstration de la propulsion
 */

void motor_demo() {
    Serial.println("avance");
    prop.avance();
    delay(1000);
    Serial.println("gauche");
    prop.gauche();
    delay(500);
    Serial.println("avance");
    prop.avance();
    delay(1000);
    Serial.println("droite");
    prop.droite();
    delay(500);
    Serial.println("stop");
    prop.stop();
    delay(500);
}

/*
 Mesure la distance en cm d'un obstacle avec le capteur US
 * entrée : rien
 * sortie : distance en cm
 */

unsigned int ultraSound() {
    long duration;
    int distance;
    digitalWrite(TRIG, HIGH);  //déclenche de capteur US
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);    
    duration = pulseIn(ECHO, HIGH); //mesure da durée de l'écho    
    distance = duration * 0.034 / 2; //calcul la distance en cm
    //Serial.print("Distance: ");
    //Serial.println(distance);
    return distance;
}



