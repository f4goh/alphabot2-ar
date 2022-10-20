/* WaveShare ARPICAR Run Forward/Backward/Left/Right Test
   
   ARPICAR run forward,backward,left right and so on..
   
   Created 25th June 2016
           by Xinwu Lin
           
   CN: http://www.waveshare.net/
   EN: http://www.waveshare.com/
*/

#define ECHO   2
#define TRIG   3

int Distance = 0;

int Distance_test()         // Measure the distance 
{
  digitalWrite(TRIG, LOW);   // set trig pin low 2μs
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);  // set trig pin 10μs , at last 10us 
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);    // set trig pin low
  float Fdistance = pulseIn(ECHO, HIGH);  // Read echo pin high level time(us)
  Fdistance= Fdistance/58;       //Y m=（X s*344）/2
  // X s=（ 2*Y m）/344 ==》X s=0.0058*Y m ==》cm = us /58       
  return (int)Fdistance;
}  

void setup()
{
  Serial.begin(115200);  
  pinMode(ECHO, INPUT);    // Define the ultrasonic echo input pin
  pinMode(TRIG, OUTPUT);   // Define the ultrasonic trigger input pin   
}

void loop()
{
  Distance = Distance_test();                 //display distance 
  if((2 < Distance) && (Distance < 400))      //Ultrasonic range ranging 2cm to 400cm
  {
    Serial.print("Distance = ");            //print distance
    Serial.print(Distance);       
    Serial.println("cm");       
  }
  else
  {
    Serial.println("!!! Out of range");      
  }
  delay(250);
}
