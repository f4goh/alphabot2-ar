#define PWMA   6           //Left Motor Speed pin (ENA)
#define AIN2   A0          //Motor-L forward (IN2).
#define AIN1   A1          //Motor-L backward (IN1)
#define PWMB   5           //Right Motor Speed pin (ENB)
#define BIN1   A2          //Motor-R forward (IN3)
#define BIN2   A3          //Motor-R backward (IN4)
#define IR  4              //he infrare remote receiver pin 

#define KEY2 0x18                 //Key:2 
#define KEY8 0x52                 //Key:8 
#define KEY4 0x08                 //Key:4 
#define KEY6 0x5A                 //Key:6 
#define KEY1 0x0C                 //Key:1 
#define KEY3 0x5E                 //Key:3 
#define KEY5 0x1C                 //Key:5
#define SpeedDown 0x07            //Key:VOL-
#define SpeedUp 0x15              //Key:VOL+
#define ResetSpeed 0x09           //Key:EQ
#define Repeat 0xFF               //press and hold the key

unsigned long lasttime = 0;
unsigned char results;
byte flag = 0;
int Speed = 150;

char IR_decode(unsigned char code);
void translateIR();
void forward();
void backward();
void right();
void left();
void stop();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(IR,INPUT);
  pinMode(PWMA,OUTPUT);                     
  pinMode(AIN2,OUTPUT);      
  pinMode(AIN1,OUTPUT);
  pinMode(PWMB,OUTPUT);       
  pinMode(AIN1,OUTPUT);     
  pinMode(AIN2,OUTPUT);  
  Serial.println("IR control example");
}

void loop() {
  // put your main code here, to run repeatedly:
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
        stop();
        Serial.println("stop");
      }
    }
  }
}
/*-----( Declare User-written Functions )-----*/
void translateIR() // takes action based on IR code received
// describing KEYES Remote IR codes 
{  
  switch(results)
  {
    case KEY2: 
      forward();break;
    case KEY4: 
      left();break;
    case KEY5: 
      stop();break;
    case KEY6: 
       right();break;
    case KEY8: 
       backward();break; 
    case SpeedUp: 
      Speed += 10;
      if(Speed >255)Speed = 250;
      break;
    case SpeedDown: 
      Speed -= 10;
      if(Speed < 0)Speed = 0;
      break;
    case ResetSpeed: 
       Speed = 150;break;
    case Repeat:
        break; 
    default:
        stop();
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
  analogWrite(PWMA,50);
  analogWrite(PWMB,50);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);  
}

void left()
{
  analogWrite(PWMA,50);
  analogWrite(PWMB,50);
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
