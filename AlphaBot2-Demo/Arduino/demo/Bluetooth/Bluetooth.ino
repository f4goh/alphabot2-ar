#include <Adafruit_NeoPixel.h>

#define PWMA   6           //Left Motor Speed pin (ENA)
#define AIN2   A0          //Motor-L forward (IN2).
#define AIN1   A1          //Motor-L backward (IN1)
#define PWMB   5           //Right Motor Speed pin (ENB)
#define BIN1   A2          //Motor-R forward (IN3)
#define BIN2   A3          //Motor-R backward (IN4)
#define PIN 7

String comdata = "";
int Speed = 150;
uint16_t i, j;
unsigned long lasttime = 0;
byte flag = 1;
Adafruit_NeoPixel RGB = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800);

void forward();
void backward();
void right();
void left();
void stop();
uint32_t Wheel(byte WheelPos);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  RGB.begin();
  RGB.show(); // Initialize all pixels to 'off'
  pinMode(PWMA,OUTPUT);                     
  pinMode(AIN2,OUTPUT);      
  pinMode(AIN1,OUTPUT);
  pinMode(PWMB,OUTPUT);       
  pinMode(AIN1,OUTPUT);     
  pinMode(AIN2,OUTPUT);  
  Serial.println("Bluetooth control example");
  stop();
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0)  
  {
    comdata += char(Serial.read());
    delay(2);
  }
  if (comdata.length() > 0)
  {
    Serial.println(comdata);
    const char* command = comdata.c_str();
    if(strcmp(command,"Forward") == 0)          //Forward
    {
      flag = 0;
      RGB.setPixelColor(0, 0xFF0000);
      RGB.setPixelColor(1, 0xFF0000);
      RGB.setPixelColor(2, 0xFF0000);
      RGB.setPixelColor(3, 0xFF0000);
      RGB.show();
      forward();
    }
    else if(strcmp(command,"Backward") == 0)    //Backward
    {
      flag = 0;
      RGB.setPixelColor(0, 0x00FF00);
      RGB.setPixelColor(1, 0x00FF00);
      RGB.setPixelColor(2, 0x00FF00);
      RGB.setPixelColor(3, 0x00FF00);
      RGB.show();  
      backward();
    }
    else if(strcmp(command,"Left") == 0)        //Left
    {
      flag = 0;
      RGB.setPixelColor(0, 0xFFFF00);
      RGB.setPixelColor(1, 0xFFFF00);
      RGB.setPixelColor(2, 0xFFFF00);
      RGB.setPixelColor(3, 0xFFFF00);
      RGB.show();
      left();
    }
    else if(strcmp(command,"Right") == 0)       //Right
    {
      flag = 0;
      RGB.setPixelColor(0, 0x0000FF);
      RGB.setPixelColor(1, 0x0000FF);
      RGB.setPixelColor(2, 0x0000FF);
      RGB.setPixelColor(3, 0x0000FF);
      RGB.show();  
      right();
    }
    else if(strcmp(command,"Stop") == 0)        //Stop
    {
      flag = 1;
      stop();
    }
    else if(strcmp(command,"Low") == 0)         //Low
      Speed = 50;
    else if(strcmp(command,"Medium") == 0)      //Medium
      Speed = 150;
    else if(strcmp(command,"High") == 0)        //High
      Speed = 250;
    comdata = "";
  }
  if(millis() - lasttime >20){  
    lasttime = millis();   
    for(i=0; i< RGB.numPixels(); i++) {
      RGB.setPixelColor(i, Wheel(((i * 256 / RGB.numPixels()) + j) & 255));
    }
    if(flag)RGB.show();
    if(j++ > 256*5) j= 0;
  }
}

void forward()
{
  analogWrite(PWMA,Speed);
  analogWrite(PWMB,Speed);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW);  
  digitalWrite(BIN2,HIGH); 
}

void backward()
{
  analogWrite(PWMA,Speed);
  analogWrite(PWMB,Speed);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);  
}

void right()
{
  analogWrite(PWMA,80);
  analogWrite(PWMB,80);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);  
}

void left()
{
  analogWrite(PWMA,80);
  analogWrite(PWMB,80);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);  
}

void stop()
{
  analogWrite(PWMA,0);
  analogWrite(PWMB,0);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,LOW);  
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return RGB.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return RGB.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return RGB.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
