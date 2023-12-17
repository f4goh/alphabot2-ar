# alphabot2-ar

AlphaBot2-Ar [wiki](https://www.waveshare.com/wiki/AlphaBot2-Ar) 

le robot AlphaBot2 dispose des capteurs suivants

- 5 capteurs analogiques de suivi de piste utilisant un CAN sur 10 bits;
- 2 capteurs latéraux gauche et droit tout ou rien (la distance est réglable à l'aide de 2 potentiomètres);
- 1 capteur ultra-son.

Le robot AlphaBot2 dispose aussi d'un joystick et d'un afficheur OLED, d'un buzzer et de 4 leds RGB.
Attention les leds RGB consomment. Utiliser les en dernier lieu.

Le robot est propulsé par deux moteurs gauche et droite. la vitesse est réglable entre 0 et 255.
Privilégier au départ une consigne de vitesse de 40.

Le robot dispose de deux connecteurs USB :

- Connecteur USB type B pour la programmation sur un ESP32;
- Connecteur micro USB très fragile pour la recharge des batteries.

Un interrupteur est situé sous le robot pour la mise sous tension de la partie propulsion

Un bouton reset (A coté du connecteur USB type B) sur la carte ESP32 permet de réinitialiser le programme 

Le développement logiciel utilise principalement 3 bibliothèques :

- PCF8584 (joystick, buzzer, les capteurs latéraux)

- Propulsion (2 moteurs gauche et droit)

- TRSensors (capteurs de suivi de piste)

Le logiciel implanté dans le robot est nommé piste :

https://github.com/f4goh/alphabot2-ar/tree/main/AlphaBot2-Demo/ESP32/piste

Ce programme montre toutes les fonctionalités du robot

la fonction void piste() est incomplète volontairement pour le challenge
















