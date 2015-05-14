#Copyright Phillip Moxley 2015. All rights reserved.

// SET ALL THE INTERFACES
const int Small_Dpin = 2;
const int Small_Apin = A0;
const int Large_Dpin = 3;
const int Large_Apin = A1;
const int Button_Dpin = 4;
const int Temp_Dpin = 5;
const int Light_Apin = A2;
const int Tilt_Apin = A3;

// Temp & Humidity variable
byte Tdat[5];

// The Setup
void setup()
{
  pinMode(Small_Dpin, INPUT);
  pinMode(Large_Dpin, INPUT);
  pinMode(Temp_Dpin, OUTPUT);
  digitalWrite(Temp_Dpin,HIGH);
  Serial.begin(9600);
}

// Main Loop
void loop()
{
  read_temp();
  Serial.print("Small Mic:");
  Serial.print(digitalRead(Small_Dpin));
  Serial.print("-");
  Serial.print(analogRead(Small_Apin));
  Serial.print("   Large Mic:");
  Serial.print(digitalRead(Large_Dpin));
  Serial.print("-");
  Serial.print(analogRead(Large_Apin));
  Serial.print("   Light:");
  Serial.print(analogRead(Light_Apin));
  Serial.print("   Tilt:");
  Serial.print(analogRead(Tilt_Apin));   
  Serial.print("   Button:");
  Serial.print(digitalRead(Button_Dpin));
  Serial.print("    Humdity:");
  Serial.print(Tdat[0]);
  Serial.print("%  ");
  Serial.print("Temp:");
  Serial.print(Tdat[2]*9/5+32);
  Serial.println("*F  ");

  delay(3000);
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
  for (i=0; i<5; i++)
  {
    for(j=0; j<8; j++)
    {
      while(digitalRead(Temp_Dpin)==LOW);
        delayMicroseconds(27);
        if (digitalRead(Temp_Dpin)==HIGH)
          Tdat[i] |=(1<<(7-j));
      while (digitalRead(Temp_Dpin)==HIGH);
    }
  }
  pinMode(Temp_Dpin,OUTPUT);
  digitalWrite(Temp_Dpin,HIGH);
}
