/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PCF8574.h
 * Author: ale
 *
 * Created on 31 octobre 2022, 19:58
 */

#ifndef PCF8574_H
#define PCF8574_H

#include <Arduino.h>
#include <Wire.h>

#define SDA 21
#define SCL 22

#define Addr  0x20  //PCF8574

typedef struct
{
    bool haut : 1;  //lsb
    bool droit : 1; 
    bool gauche : 1; 
    bool bas : 1; 
    bool center : 1; 
    bool buzzer : 1; 
    bool irDroit : 1; 
    bool irGauche : 1; //msb
} pcf_struct;



class PCF8574 {
public:
    PCF8574();
    virtual ~PCF8574();
    
    void begin();
    
    
    void setBeep(int tempo);
    
    bool getIrDroit();
    bool getIrGauche();
    bool getJoyHaut();
    bool getJoyBas();
    bool getJoyDroit();
    bool getJoyGauche();
    bool getJoyCenter();
    
    
private:
    void update();
    void write(byte data);
    byte read();
    pcf_struct data;
    
    
};

#endif /* PCF8574_H */

