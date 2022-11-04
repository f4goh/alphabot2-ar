/*
 * test du robot : commandes élémentaires 
 * reset de l'afficheur SSD1306 (disable au niveau haut) (donc possible de faire une économie de broche en reliant directement au RESET de l'esp32)
 * la broche d/c est utilisée en tant que sélection d'adresse i2c et est laissée en l'air.  -> 0x3c
 * l'afficheur est scindé en 2 couleurs (bleu+jaune) avec une ligne séparatrice non affichable
 * Lors de la mise sous tension avec les 2 batteries, faire un reset esp32 manuel
 * Manque les classes pour
 * gestion remote IR
 * gestion ultrason
 * classe spécifique pour l'aficheur
 * classe de suivi de ligne ou autre suivant projet de piste
 */


#include <Arduino.h>
#include <Led.h>
#include <SSD1306Wire.h>
#include <TRSensors.h>
#include <Propulsion.h>
#include <PCF8574.h>


#define NBLED 4           // Définit le nombre de leds du rubant
#define DELAY 500         // Temps d'attente en ms pour cheniller


#define TRIG 25           //capteur ultra son
#define ECHO 26
#define IR 17             //télécomande IR

#define KEY2 0x18                 //Key:2 
#define KEY8 0x52                 //Key:8 
#define KEY4 0x08                 //Key:4 
#define KEY6 0x5A                 //Key:6 
#define KEY1 0x0C                 //Key:1 
#define KEY3 0x5E                 //Key:3 
#define KEY5 0x1C                 //Key:5
#define KEY7 0x42                 //Key:7
#define KEY9 0x4A                 //Key:9
#define KEY0 0x16                 //Key:0
#define KEY100 0x19                 //Key:100
#define KEY200 0x0D                 //Key:100
#define SpeedDown 0x07            //Key:VOL-
#define SpeedUp 0x15              //Key:VOL+
#define ResetSpeed 0x09           //Key:EQ
#define PREV 0x44           //Key:PREV
#define NEXT 0x40           //Key:NEXT
#define PLAY 0x43           //Key:PLAY
#define CHM 0x45          //Key:CH plus
#define CH 0x46          //Key:CH
#define CHP 0x47           //Key:CH MOINS
#define Repeat 0xFF               //press and hold the key

#define SPEED 40            //vitesse moteur 0-255

unsigned long lasttime = 0;
unsigned char results;
byte flag = 0;

byte speed=SPEED;


Led *led;  //neopixels VRB

SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_64);

Propulsion prop(SPEED);
PCF8574 pcf;

TRSensors trs = TRSensors();

unsigned int sensorValues[NUMSENSORS]; //5 capteurs de ligne



unsigned int ultraSound();
void translateIR();
char IR_decode(unsigned char * code);


void setup() {
    Serial.begin(115200);
    // Création de 4 leds couleurs RVB
    led = new Led(NBLED); // quatre leds;
    led->setDelay(DELAY);


    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(IR, INPUT);

    pcf.begin();


    display.init();
    display.flipScreenVertically();
   

    //led->allumer(ROUGE, 0);
    //led->allumer(VERT, 1);
    //led->allumer(BLEU, 2);
    //led->allumer(ROUGE, 3); //pb ??
}


void loop() {
    char buffer[30];
    display.clear();
     display.setFont(ArialMT_Plain_16);
    //affiche les capteurs de collision avant G et D
     sprintf(buffer, "IrG %d  -  IrD %d", pcf.getIrGauche(),pcf.getIrDroit() ); 
    display.drawString(0, 0, buffer);

    //affiche les capteurs de suivi de ligne
    trs.AnalogReadSPI(sensorValues);
    
    display.setFont(ArialMT_Plain_10);
    sprintf(buffer, "%d %d %d %d %d",sensorValues[0],sensorValues[1],sensorValues[2],sensorValues[3],sensorValues[4]); //

    display.drawString(0, 20, buffer);
   
    //affiche le capteur ultra son avant
    sprintf(buffer, "distance US: %d ",ultraSound());

    display.drawString(0, 30, buffer);
        
    display.display();
    
    
    //test de la telecomande IR
  if(IR_decode(&results))
  {
    flag = 1;
    lasttime = millis();  
    translateIR(); 
  }
  else
  {
    if(flag == 1)
    {
      if(millis() - lasttime >150)
      { 
        flag = 0;
        prop.stop();
        Serial.println("stop");
      }
    }
  }
    
}



unsigned int ultraSound() {
    long duration;
    int distance;
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(ECHO, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    return distance;
    //Serial.print("Distance: ");
    //Serial.println(distance);
}


void translateIR() // takes action based on IR code received
{
    switch (results) {
        case KEY1:
            prop.differentiel(-5);
            break;
        case KEY3:
            prop.differentiel(+5);
            break;
        case KEY2:
            prop.avance();
            break;
        case KEY4:
            prop.gauche();
            break;
        case KEY5:
            prop.stop();
            break;
        case KEY6:
            prop.droite();
            break;
        case KEY8:
            prop.recule();
            break;            
        case KEY7:
            led->allumer(ROUGE, 0);
            break;
        case KEY9:
            led->eteindre(0);
            break;            
        case SpeedUp:
            speed += 10;
            if (speed > 255)speed = 250;
            prop.setSpeed(speed);
            break;
        case SpeedDown:
            speed -= 10;
            if (speed < 0)speed = 0;
            prop.setSpeed(speed);
            break;
        case ResetSpeed:
            speed = SPEED;
            break;
        case Repeat:
            break;
        default:
            prop.stop();
    }// End Case
} //END translateIR



char IR_decode(unsigned char * code)         
{
  char value = 0;
  unsigned int count = 0;
  unsigned char i,index,cnt=0,data[4]={0,0,0,0};
  if(digitalRead(IR) == LOW)
  {
    count = 0;
    while(digitalRead(IR) == LOW && count++ < 200)   //9ms
      delayMicroseconds(60);
      
    count = 0;
    while(digitalRead(IR) == HIGH && count++ < 80)    //4.5ms
      delayMicroseconds(60);

    for(i =0;i<32;i++)
    {   
        count = 0;
        while(digitalRead(IR) == LOW && count++ < 15)  //0.56ms
            delayMicroseconds(60);
        count = 0;
        while(digitalRead(IR) == HIGH && count++ < 40)  //0: 0.56ms; 1: 1.69ms
            delayMicroseconds(60);
        if (count > 20)data[index] |= (1<<cnt);
        if(cnt == 7)
        {
          cnt = 0;
          index++;
        }
        else cnt++;
    }
    if(data[0]+data[1] == 0xFF && data[2]+data[3]==0xFF)  //check 
    {
      code[0] = data[2];
      Serial.println(code[0], HEX);
      value = 1;
    }
    if(data[0] == 0xFF && data[1] == 0xFF && data[2] == 0xFF && data[3]==0xFF)
    {
      code[0] = 0xFF;
     // Serial.println("rep");
      value = 1;
    }
  }
  return value;
}

