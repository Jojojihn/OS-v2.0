#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

#define VrX A7
#define VrY A6
#define Bttn 22
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

MCUFRIEND_kbv tft;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Bttn, INPUT_PULLUP);
  // Reading TFT ID:
  uint16_t ID = tft.readID();
  // Initializing TFT display:
  tft.begin(ID);
  // Fill TFT Screen with a color:
  tft.fillScreen(BLACK);
}

void loop()
{
  int vals[2];

  vals[0] = analogRead(VrX);
  vals[1] = analogRead(VrY);

  Serial.print(F("X: "));
  Serial.println(vals[0]);

  Serial.print(F("Y: "));
  Serial.println(vals[1]);

  delay(50);

  bool button = (digitalRead(Bttn) == LOW);

  if (button)
  {
    Serial.println(F("Button pressed"));
  }
}