#include "Thread.h"
#include "ThreadController.h"
#include "TimerOne.h"

// SET ALL THE INTERFACES
const int Small_Dpin = 2;
const int Small_Apin = A0;
const int Large_Dpin = 3;
const int Large_Apin = A1;
const int Temp_Dpin = 5;
const int Button_Dpin = 4;
const int Light_Apin = A2;
const int Tilt_Apin = A3;
byte Tdat[3];

// SET GLOBAL VARIABLES FOR READING
int Sounds = 0;
int Bump = 0;
int Buttons = 9000;
int Lights = 0;
int Lamp = 0;
int Tilts = 0;
int Move = 0;
int n = 0;

ThreadController controller = ThreadController();
Thread Sound = Thread();
Thread Temp = Thread();
Thread Button = Thread();
Thread Light = Thread();
Thread Tilt = Thread();

//Callbacks
void Soundcallback(){
  int SoundWas = 0;
  int s = 0;
  SoundWas = Sounds;
  if(digitalRead(Small_Dpin)==1){
    Sounds = analogRead(Small_Apin);
  }
  if(digitalRead(Large_Dpin)==1){
    Sounds = Sounds + analogRead(Large_Apin);
  }
  s = SoundWas - Sounds;
  if( s >= 3){
    Bump = 1;
  }
}
void Tempcallback(){
  //noInterrupts();
  //read_temp();
  //interrupts();
}
void Buttoncallback(){
  Buttons = digitalRead(Button_Dpin);
}
void Lightcallback(){
  int LightWas = 0;
  int l = 0;
  LightWas = Lights;
  Lights = analogRead(Light_Apin);
  l = LightWas - Lights;
  if( abs(l) >= 50){
    Lamp = 1;
  }
}
void Tiltcallback(){
  int TiltWas = 0;
  int t = 0;
  TiltWas = Tilts;
  Tilts = analogRead(Tilt_Apin);
  t = TiltWas - Tilts;
  if( t >= 600){
    Move = 1;
  }
}

// This is the callback for the Timer
void timerCallback(){
  controller.run();
}

void setup(){
  pinMode(Small_Dpin, INPUT);
  pinMode(Large_Dpin, INPUT);
  pinMode(Temp_Dpin, OUTPUT);
  digitalWrite(Temp_Dpin,HIGH);

  Serial.begin(9600);
  
  Sound.onRun(Soundcallback);
  Sound.setInterval(10);
  Temp.onRun(Tempcallback);
  Temp.setInterval(300000);
  Button.onRun(Buttoncallback);
  Button.setInterval(100);
  Light.onRun(Lightcallback);
  Light.setInterval(10000);
  Tilt.onRun(Tiltcallback);
  Tilt.setInterval(200);

  controller.add(&Sound);
  controller.add(&Temp);
  controller.add(&Button);
  controller.add(&Light);
  controller.add(&Tilt);

  Timer1.initialize(10000);
  Timer1.attachInterrupt(timerCallback);
}

void loop(){
  if(n >= 30){            //30 is roughtly every minute, 1800 is roughly hourly
    //noInterrupts();
    read_temp();
    //interrupts();
    if(Tdat[2] > 44){
      Tdat[2] = 0;
    }
    if(Tdat[0] > 100){
      Tdat[0] = 0;
    }
    n=0;
  }
  n=n+1;
  
  delay(1000);
 
  //Print the fresh readings
  if(Bump == 1){
    Serial.println("A Bump Was Detected.");
    Bump = 0;
  }
  if(Tdat[2] != 0  && Tdat[2] < 44){
    Serial.print("Temp: ");
    Serial.println(Tdat[2]*9/5+32);
    Tdat[2]=0;
  }
  if(Tdat[0] != 0 &&  Tdat[0] < 100){
    Serial.print("Humidity: ");
    Serial.println(Tdat[0]);
    Tdat[0]=0;
  }
  if(Buttons != 9000){
    //Serial.print("Button: ");
    //Serial.println(Buttons);
  }
  if(Lamp == 1 ){
    Serial.println("A light was turned on/off.");
    Lamp = 0;
  }
  if(Move == 1 ){
    Serial.println("I'm certain something shook me.");
    Move = 0;
  }

  delay(1000);
}

//Function for Reading Temp/Humidity sensor
byte read_temp()
{
  byte i=0;
  byte j=0;
  digitalWrite(Temp_Dpin,LOW);
  delay(50);
  digitalWrite(Temp_Dpin,HIGH);
  delayMicroseconds(100);
  pinMode(Temp_Dpin,INPUT);
  delayMicroseconds(100);
  for (i=0; i<3; i++)
  {
    for(j=0; j<8; j++)
    {
      while(digitalRead(Temp_Dpin)==LOW);{
        delayMicroseconds(27);
        if (digitalRead(Temp_Dpin)==HIGH){
          Tdat[i] |=(1<<(7-j));
          while(digitalRead(Temp_Dpin)==HIGH);
        }
      }
    }
  }
  pinMode(Temp_Dpin,OUTPUT);
  digitalWrite(Temp_Dpin,HIGH);
}

