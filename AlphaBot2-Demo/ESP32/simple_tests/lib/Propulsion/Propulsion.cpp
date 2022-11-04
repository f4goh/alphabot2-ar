/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Propulsion.cpp
 * Author: ale
 * 
 * Created on 29 octobre 2022, 19:15
 */

#include "Propulsion.h"

Propulsion::Propulsion(byte _speed):
speed(_speed)
{
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    digitalWrite(AIN1, LOW); //CCW
    digitalWrite(AIN2, LOW); //CW
    ledcSetup(CANAL_MOTORA_PWM, FREQUENCY, 8); // canal = 3, frequence = 5000 Hz, resolution = 8 bits
    ledcAttachPin(PWMA, CANAL_MOTORA_PWM);
    ledcWrite(CANAL_MOTORA_PWM, 0); //   modifie le rapport cyclique

    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    digitalWrite(BIN1, LOW); //CCW
    digitalWrite(BIN2, LOW); //CW
    ledcSetup(CANAL_MOTORB_PWM, FREQUENCY, 8); // canal = 4, frequence = 5000 Hz, resolution = 8 bits
    ledcAttachPin(PWMB, CANAL_MOTORB_PWM);
    ledcWrite(CANAL_MOTORB_PWM, 0); //   modifie le rapport cyclique      
}

Propulsion::~Propulsion() {
}


void Propulsion::avance()
{
    Serial.print(speed);
  ledcWrite(CANAL_MOTORA_PWM, speed); 
  ledcWrite(CANAL_MOTORB_PWM, speed); 
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);  
}

void Propulsion::recule()
{
  ledcWrite(CANAL_MOTORA_PWM, speed); 
  ledcWrite(CANAL_MOTORB_PWM, speed); 
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);  

}

void Propulsion::droite()
{
  ledcWrite(CANAL_MOTORA_PWM, speed); 
  ledcWrite(CANAL_MOTORB_PWM, speed); 
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);  
}

void Propulsion::gauche()
{
  ledcWrite(CANAL_MOTORA_PWM, speed); 
  ledcWrite(CANAL_MOTORB_PWM, speed); 
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW);  
  digitalWrite(BIN2,HIGH); 
}

void Propulsion::stop()
{
  ledcWrite(CANAL_MOTORA_PWM, 0); 
  ledcWrite(CANAL_MOTORB_PWM, 0); 
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,LOW);  
}

void Propulsion::setSpeed(byte _speed){
    speed=_speed;
}

void Propulsion::differentiel(int ecart) {
    if ((speed + ecart < 255)&&(speed - ecart > 0)) {
        ledcWrite(CANAL_MOTORA_PWM, speed+ecart);
        ledcWrite(CANAL_MOTORB_PWM, speed-ecart);
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        digitalWrite(BIN1, LOW);
        digitalWrite(BIN2, HIGH);

    }
}