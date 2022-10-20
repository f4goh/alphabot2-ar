/* WaveShare ARPICAR Playing with Infrared Remote Control
   
   Created 24th December 2016
           by MyMX
           
   CN: http://www.waveshare.net/
   EN: http://www.waveshare.com/
*/
#include <aJSON.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

//JSON
aJsonStream serial_stream(&Serial);

#define PWMA   6           //Left Motor Speed pin (ENA)
#define AIN2   A0          //Motor-L forward (IN2).
#define AIN1   A1          //Motor-L backward (IN1)
#define PWMB   5           //Right Motor Speed pin (ENB)
#define BIN1   A2          //Motor-R forward (IN3)
#define BIN2   A3          //Motor-R backward (IN4)


//Speed
#define LOW_SPEED      100
#define MEDIUM_SPEED   200
#define HIGH_SPEED     255

//buzzer
#define Addr  0x20
#define beep_on  PCF8574Write(0xDF & PCF8574Read())
#define beep_off PCF8574Write(0x20 | PCF8574Read())


#define RGB_PIN 7
Adafruit_NeoPixel RGB = Adafruit_NeoPixel(4, RGB_PIN, NEO_GRB + NEO_KHZ800);

int Speed = MEDIUM_SPEED;   

void PCF8574Write(byte data);
byte PCF8574Read();
void forward();
void backward();
void right();
void left();
void stop();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  RGB.begin();
  RGB.show(); // Initialize all pixels to 'off'
  pinMode(PWMA,OUTPUT);                     
  pinMode(AIN2,OUTPUT);      
  pinMode(AIN1,OUTPUT);
  pinMode(PWMB,OUTPUT);       
  pinMode(AIN1,OUTPUT);     
  pinMode(AIN2,OUTPUT);  
  Serial.println("{\"State\":\"Waveshare\"}");
}

//{"Forward":"Down"}
//{"Forward":"Up"}
//{"Backward":"Down"}
//{"Backward":"Up"}
//{"Left":"Down"}
//{"Left":"Up"}
//{"Right":"Down"}
//{"Right":"Up"}
//{"Low":"Down"}
//{"Medium":"Down"}
//{"High":"Down"}
void ComExecution(aJsonObject *msg)
{
  aJsonObject *Forward = aJson.getObjectItem(msg, "Forward");
  if (Forward)
  {
     String str = Forward->valuestring;
     if(str.equals("Down"))
     {
       forward();
       Serial.println("{\"State\":\"Forward\"}");
     }
     else if (str.equals("Up"))
     {
      stop();
      Serial.println("{\"State\":\"Stop\"}");
     }
  }

  aJsonObject *Backward = aJson.getObjectItem(msg, "Backward");
  if (Backward)
  {
     String str = Backward->valuestring;
     if(str.equals("Down"))
     {
      backward();
      Serial.println("{\"State\":\"Backward\"}");
     }
     else if (str.equals("Up"))
     {
      stop();
      Serial.println("{\"State\":\"Stop\"}");
     }
  }

  aJsonObject *Left = aJson.getObjectItem(msg, "Left");
  if (Left)
  {
     String str = Left->valuestring;
     if(str.equals("Down"))
     {
       left();
       Serial.println("{\"State\":\"Left\"}");
     }
     else if (str.equals("Up"))
     {
      stop();
      Serial.println("{\"State\":\"Stop\"}");
     }
  }

  aJsonObject *Right = aJson.getObjectItem(msg, "Right");
  if (Right)
  {
     String str = Right->valuestring;
     if(str.equals("Down"))
     {
      right();
      Serial.println("{\"State\":\"Right\"}");
     }
     else if (str.equals("Up"))
     {
      stop();
      Serial.println("{\"State\":\"Stop\"}");
     }
  }

  aJsonObject *Low = aJson.getObjectItem(msg, "Low");
  if (Low)
  {
     String str = Low->valuestring;
     if(str.equals("Down"))
     {
        Speed = LOW_SPEED;
        Serial.println("{\"State\":\"Low\"}");
     }
  }
  
  aJsonObject *Medium = aJson.getObjectItem(msg, "Medium");
  if (Medium)
  {
     String str = Medium->valuestring;
     if(str.equals("Down"))
     {
      Speed = MEDIUM_SPEED;
      Serial.println("{\"State\":\"Medium\"}");
     }
  }

  aJsonObject *High = aJson.getObjectItem(msg, "High");
  if (High)
  {
     String str = High->valuestring;
     if(str.equals("Down"))
     {
      Speed = HIGH_SPEED;
      Serial.println("{\"State\":\"High\"}");
     }
  }

  aJsonObject *buzzer = aJson.getObjectItem(msg, "BZ");
  if (buzzer)
  {
     String str = buzzer->valuestring;
     if(str.equals("on"))           //{"BZ":"on"}
     {
        beep_on;      
        Serial.println("{\"BZ\":\"on\"}");
     }
     else if (str.equals("off"))//{"BZ":"off"} 
     {
        beep_off;         
        Serial.println("{\"BZ\":\"off\"}");
     }
  }

  aJsonObject *rgb = aJson.getObjectItem(msg, "RGB");
  if (rgb)
  {
    byte R,G,B;
    String str = rgb->valuestring;
    int temp = str.indexOf(',');
    int temp2 = str.lastIndexOf(',');
    R = str.substring(0, temp).toInt(); 
    G = str.substring(temp+1,temp2).toInt();
    B = str.substring(temp2+1,str.length()).toInt();

    Serial.print(" R:");
    Serial.print(R);
    Serial.print(" G:");
    Serial.print(G);
    Serial.print(" B:");
    Serial.println(B);

    RGB.setPixelColor(0, RGB.Color(R, G, B));
    RGB.setPixelColor(1, RGB.Color(R, G, B));
    RGB.setPixelColor(2, RGB.Color(R, G, B));
    RGB.setPixelColor(3, RGB.Color(R, G, B));
    RGB.show();
  }

}

void loop() {
  if (serial_stream.available())
  {
    serial_stream.skip();
  }
  
  if (serial_stream.available())
  {
    aJsonObject *msg = aJson.parse(&serial_stream);
    ComExecution(msg);
    aJson.deleteItem(msg);
  }
}

void PCF8574Write(byte data)
{
  Wire.beginTransmission(Addr);
  Wire.write(data);
  Wire.endTransmission(); 
}

byte PCF8574Read()
{
  int data = -1;
  Wire.requestFrom(Addr, 1);
  if(Wire.available()) {
    data = Wire.read();
  }
  return data;
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

