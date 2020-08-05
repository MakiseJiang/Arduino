// Course Title:  VM250 Design and Manufacture I
// Project Title:  Katyusha
// Date last updated: 7/31/2020
//
/*
  Port connection: D -- digital use
                   P -- PWM use
                   
  Arduino 3.3v -> PS2 3.3v
  Arduino D14 -> PS2 DATA
  Arduino D15 -> PS2 CS
  Arduino D16 -> PS2 COMMAND
  Arduino D17 -> PS2 CLOCK
  
  Arduino D7  -> L298N EN1
  Arduino D8  -> L298N EN2 
  Arduino D4  -> L298N EN3
  Arduino D2  -> L298N EN4
  Arduino P6  -> L298N ENA
  Arduino P3  -> L298N ENB
  Arduino P9  -> Servo 1 PWM
  Arduino P10 -> Servo 2 PWM
  
*/

#include<PS2X_lib.h>
#include<Stepper.h>
#include<Servo.h>

#define PS2_DAT       14          //Define the port #
#define PS2_CMD       16
#define PS2_CS        15
#define PS2_CLK       17
#define LMOTOR1       7
#define LMOTOR2       8
#define RMOTOR1       4
#define RMOTOR2       2
#define L_PWM         6
#define R_PWM         3   
#define servo1        9
#define servo2        10
#define pul           5
#define dir           11

#define pressures     false
#define rumble        false

PS2X ps2x; // create PS2 Controller class

Servo trigger1;   //trigger motor that drive the main gear
Servo trigger2;


int LstickY=0;
int LstickX=0;
int RstickY=0;
int RstickX=0;

int delayTime=2000;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600); 
  delay(1000); //added delay to give wireless ps2 module some time to startup, before configuring it
  
  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_CS, PS2_DAT, pressures, rumble);
  pinMode(13,OUTPUT);
  pinMode(LMOTOR1,OUTPUT);
  pinMode(LMOTOR2,OUTPUT);
  pinMode(RMOTOR1,OUTPUT);
  pinMode(RMOTOR2,OUTPUT);
  pinMode(L_PWM,OUTPUT);
  pinMode(pul,OUTPUT);
  pinMode(dir,OUTPUT);

  trigger1.attach(servo1);  //Setup the 360-degree trigger motor
  trigger2.attach(servo2);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  ps2x.read_gamepad(false, 0);
  
  LstickY=ps2x.Analog(PSS_LY);     //Read the analog value of sticks
  LstickX=ps2x.Analog(PSS_LX);
  RstickY=ps2x.Analog(PSS_RY);
  RstickX=ps2x.Analog(PSS_RX);
  
  trigger1.writeMicroseconds(1500);
  trigger2.writeMicroseconds(1500);

  if (ps2x.Button(PSB_CIRCLE)){turn(200);}
  else if(ps2x.Button(PSB_CROSS)){turn(-400);}// Controll the stepper motor for reloading

  
  if(ps2x.Button(PSB_TRIANGLE)){        //Press Triangle to rotate the servo
    Serial.println("Triangle just pressed");
    trigger1.write(120);
    trigger2.write(60);
   }
   else{trigger1.write(90);trigger2.write(90);}
  if(ps2x.Button(PSB_L1)){                    // as long as L1 is pressed
    Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); // Return the analog reading of the stick position  
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC); 
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC); 
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC);
     
  }
  
  //****************Motion Function Part*************************
  //
  if(LstickY<100 ){
    Serial.println("Left foward");  
    digitalWrite(LMOTOR1,HIGH);
    digitalWrite(LMOTOR2,LOW);
    analogWrite(L_PWM,200);
  }
  
  else if (LstickY>160){
         Serial.println("Left backward");
         digitalWrite(LMOTOR1,LOW);
         digitalWrite(LMOTOR2,HIGH);
         analogWrite(L_PWM,200);
  }
  else {
    digitalWrite(LMOTOR1,LOW);
    digitalWrite(LMOTOR2,LOW);
    }
  if(RstickY<100 ){
    Serial.println("Right forward");
    digitalWrite(RMOTOR1,HIGH);
    digitalWrite(RMOTOR2,LOW);
    analogWrite(R_PWM,200);
  }
  else if (RstickY>160){
         Serial.println("Right backward");
         digitalWrite(RMOTOR1,LOW);
         digitalWrite(RMOTOR2,HIGH);
         analogWrite(R_PWM,200);
  }
  else {
    digitalWrite(RMOTOR1,LOW);
    digitalWrite(RMOTOR2,LOW);
    }
//
//************************************************************
delay(50);


}

void turn(int count){
  if (count >0){digitalWrite(dir,LOW);}
  else {digitalWrite(dir,HIGH);
        count=-count;
  }
  for (int i=0;i<count;i++){
    digitalWrite(pul,HIGH);
    delayMicroseconds(800);
    digitalWrite(pul,LOW);
    delayMicroseconds(800);
    }
  }
