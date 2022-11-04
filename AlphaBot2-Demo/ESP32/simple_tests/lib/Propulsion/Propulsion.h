/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Propulsion.h
 * Author: ale
 *
 * Created on 29 octobre 2022, 19:15
 */

#ifndef PROPULSION_H
#define PROPULSION_H

#include <Arduino.h>


#define FREQUENCY 5000


#define PWMA 27
#define CANAL_MOTORA_PWM 3
#define AIN1 2
#define AIN2 4

#define PWMB 16
#define CANAL_MOTORB_PWM 4
#define BIN1 12
#define BIN2 13


class Propulsion {
public:
    Propulsion(byte _speed);    
    virtual ~Propulsion();
    void avance();
    void recule();
    void droite();
    void gauche();
    void differentiel(int ecart);
    void stop();
    void setSpeed(byte _speed);
    
    
private:
    byte speed;
    
};

#endif /* PROPULSION_H */

