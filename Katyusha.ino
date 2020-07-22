//Course Title:  VM250 Design and Manufacture I
//Project Title:  Katyusha
//Date last updated: 7/22/2020
//

/*
  Port connection: D -- digital use
                   P -- PWM use
                   
  Arduino 3.3v -> PS2 3.3v
  Arduino D09 -> PS2 DATA
  Arduino D10 -> PS2 CS
  Arduino D11 -> PS2 COMMAND
  Arduino D12 -> PS2 CLOCK
  
  Arduino D7  -> L298N EN1
  Arduino D8  -> L298N EN2 
  Arduino D6  -> L298N EN3
  Arduino D5  -> L298N EN4
  Arduino P6  -> L298N ENA
  Arduino P3  -> L298N ENB
  Arduino P9  -> Servo 1 PWM
*/

#include<PS2X_lib.h>
#include<Servo.h>

#define PS2_DAT       14
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


#define pressures     false
#define rumble        false

PS2X ps2x; // create PS2 Controller class

Servo trigger1;
Servo trigger2;
Servo loader;

int LstickY=0;
int LstickX=0;
int RstickY=0;
int RstickX=0;




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

  trigger1.attach(servo1);
  trigger2.attach(servo2);
  trigger1.write(90);
  trigger2.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  ps2x.read_gamepad(false, 0);
  
  LstickY=ps2x.Analog(PSS_LY);
  LstickX=ps2x.Analog(PSS_LX);
  RstickY=ps2x.Analog(PSS_RY);
  RstickX=ps2x.Analog(PSS_RX);
  
  
  
  if(ps2x.Button(PSB_TRIANGLE)){
    Serial.println("Triangle just pressed");
    trigger1.write(120);
    trigger2.write(120);
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



  delay(50);
}
