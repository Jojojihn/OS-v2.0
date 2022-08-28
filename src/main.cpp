#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <Joystick.h>

#define VrX A7
#define VrY A6
#define Bttn 22



MCUFRIEND_kbv tft;

Joystick sticky = Joystick(VrX, VrY, Bttn);

void setup()
{
  // put your setup code here, to run once:
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
  Serial.begin(9600);
  pinMode(Bttn, INPUT_PULLUP);
  // Reading TFT ID:
  uint16_t ID = tft.readID();
  // Initializing TFT display:
  tft.begin(ID);
  // Fill TFT Screen with a color:
  tft.fillScreen(TFT_BLACK);
}

void loop()
{
  float vals[2];

  vals[0] = sticky.getAxes().x;
  vals[1] = sticky.getAxes().y;

  Serial.print(F("X: "));
  Serial.println(vals[0]);

  Serial.print(F("Y: "));
  Serial.println(vals[1]);

  delay(50);

  if (sticky.pressed()) {
    Serial.println(F("Button pressed"));

    for(int i = 0; i < 0xFFFF; i++) {
      tft.fillScreen(i);
      delay(5);
    }

  }
}