#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <Joystick.h>

#define VrX A7
#define VrY A6
#define Bttn 22



MCUFRIEND_kbv tft;

Joystick sticky = Joystick(VrX, VrY, Bttn);


void renderAnalogStick(Joystick &stick) {
    struct Point {
      int x;
      int y;
    };
    
    tft.fillScreen(TFT_BLACK);
    Point size = {tft.width(), tft.height()};
    Point center = {(size.x / 2), (size.y / 2)};
    
    //Center dot
    tft.fillCircle(center.x, center.y, 2, TFT_BLUE);

    //Outer Ring
    int smallestSize = min(size.x, size.y);
    int outerRingRad = (smallestSize / 2) - 10;
    tft.drawCircle(center.x, center.y, outerRingRad, TFT_WHITE);

    //Deadzone Ring
    int deadZoneRingRad = outerRingRad * stick.getDeadzone();
    tft.drawCircle(center.x, center.y, deadZoneRingRad, TFT_RED);

    //Stick
    Point stickPos = {
      center.x + outerRingRad * (-stick.getAxes().x),
      center.y + outerRingRad * (-stick.getAxes().y)
    };
    tft.fillCircle(stickPos.x, stickPos.y, 10, TFT_BLUE);
    
}

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

    Joystick::Axes prev = sticky.getAxes();
    renderAnalogStick(sticky);
    while(!sticky.pressed()) {

      if(prev.x == sticky.getAxes().x && prev.y == sticky.getAxes().y) {
        continue;
      }

      renderAnalogStick(sticky);
      
    }
  }
}



