/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PCF8574.cpp
 * Author: ale
 * 
 * Created on 31 octobre 2022, 19:58
 */

#include "PCF8574.h"

PCF8574::PCF8574() {

}

PCF8574::~PCF8574() {
}

void PCF8574::begin() {
    Wire.begin(SDA, SCL); // SDA SCL
    write(0xFF); //set Pin High
}

void PCF8574::setBeep(int tempo) {
    write(0xDF & read());
    delay(tempo);
    write(0xFF | read());
}

void PCF8574::write(byte data) {
    Wire.beginTransmission(Addr);
    Wire.write(data);
    Wire.endTransmission();
}

byte PCF8574::read() {
    int data = -1;
    Wire.requestFrom(Addr, 1);
    if (Wire.available()) {
        data = Wire.read();
    }
    return data;
}

void PCF8574::update() {
    *(byte*)&data = read();
}

bool PCF8574::getIrDroit() {
    update();
    return data.irDroit;
}

bool PCF8574::getIrGauche() {
    update();
    return data.irGauche;
}

bool PCF8574::getJoyHaut() {
    update();
    return data.haut;
}

bool PCF8574::getJoyBas() {
    update();
    return data.bas;
}

bool PCF8574::getJoyDroit() {
    update();
    return data.droit;
}

bool PCF8574::getJoyGauche() {
    update();
    return data.gauche;
}

bool PCF8574::getJoyCenter() {
    update();
    return data.center;
}
