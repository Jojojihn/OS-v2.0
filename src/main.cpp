#include <Arduino.h>
#include "UTFT.h"

#define VrX A7
#define VrY A6
#define Bttn 22


extern uint8_t BigFont[];
//Uncomment the next line for Arduino Mega
UTFT tftDisplay(ILI9486,A2,A1,A3,A4);   // Remember to change the model parameter to suit your display module!


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  pinMode(Bttn, INPUT_PULLUP);

  tftDisplay.InitLCD();
  tftDisplay.clrScr();
  tftDisplay.setFont(BigFont);

}

void loop() {
  int vals[2];

  vals[0] = analogRead(VrX);
  vals[1] = analogRead(VrY);

  Serial.print(F("X: "));
  Serial.println(vals[0]);

  Serial.print(F("Y: "));
  Serial.println(vals[1]);

  delay(50);

  bool button = (digitalRead(Bttn) == LOW);

  if(button) {
    Serial.println(F("Button pressed"));
  }


  tftDisplay.print("Text rotation", 0, 0);
  tftDisplay.setColor(0, 0, 255);
  tftDisplay.print("0 degrees", 0, 16, 0);
  tftDisplay.print("90 degrees", 319, 0, 90);
  tftDisplay.print("180 degrees", 319, 239, 180);
  tftDisplay.print("270 degrees", 0, 239, 270);


 
  // put your main code here, to run repeatedly:
}