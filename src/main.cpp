#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <Joystick.h>
#include <img/sleep_ico.c>
#include <LowPower.h>


//Comment/Uncomment this to switch between Simulide mode and hardware mode. Simulide uses a different Display and is used for debugging
#define Simulide

#ifdef Simulide
#include <Adafruit_ILI9341.h>
#define TFT_CS 8
#define TFT_RST 9
#define TFT_DC 10
#define TFT_MOSI 11
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, 13, TFT_RST);
#else
MCUFRIEND_kbv tft;
#endif

#define VrX A7
#define VrY A6
#define Bttn 22

#define SHUTDOWN_BTTN 18

Joystick sticky = Joystick(VrX, VrY, Bttn);

void renderAnalogStick(Joystick &stick) {
    struct Point {
      int x;
      int y;
    };

    
    tft.fillScreen(TFT_WHITE);
    Point size = {tft.width(), tft.height()};
    Point center = {(size.x / 2), (size.y / 2)};
    
    Joystick::Axes prev = Joystick::Axes{-2, -2};
    Point drawn = {-1,-1};

    while(!sticky.pressed()) {
      if(abs(prev.x - stick.getAxes().x) < 0.02 && abs(prev.y - stick.getAxes().y) < 0.02) {
        delay(5);
        continue;
      }

      if(drawn.x != -1) {
        tft.fillCircle(drawn.x, drawn.y, 10, TFT_WHITE);
      }
      
      //Center dot
      tft.fillCircle(center.x, center.y, 2, TFT_RED);

      //Outer Ring
      int smallestSize = min(size.x, size.y);
      int outerRingRad = (smallestSize / 2) - 10;
      tft.drawCircle(center.x, center.y, outerRingRad, TFT_BLACK);

      //Deadzone Ring
      int deadZoneRingRad = outerRingRad * stick.getDeadzone();
      tft.drawCircle(center.x, center.y, deadZoneRingRad, TFT_RED);

      //Stick
      Joystick::Axes axes = stick.getAxes();
      Point stickPos = {
        center.x + outerRingRad * (-axes.x),
        center.y + outerRingRad * (-axes.y)
      };

      tft.fillCircle(stickPos.x, stickPos.y, 10, TFT_BLUE);
      prev = axes;
      drawn = {stickPos.x, stickPos.y};
      
      //Ticks
      int startRad = outerRingRad - 4;
      int endRad = outerRingRad + 4;
      for(float i = 0; i < 360; i += 90) {
        Point startPoint;
        startPoint.x = center.x + (sin(radians(i)) * startRad);
        startPoint.y = center.y - (cos(radians(i)) * startRad);

        Point endPoint;
        endPoint.x = center.x + (sin(radians(i)) * endRad);
        endPoint.y = center.y - (cos(radians(i)) * endRad);

        tft.drawLine(startPoint.x, startPoint.y, endPoint.x, endPoint.y, TFT_BLACK);
        
      }

      delay(80);
    }

    tft.fillScreen(TFT_BLACK);
}

void wake() {}
void shutdown() {
  int pos[] = {0,0};
  pos[0] = tft.width() / 2 - sleep_ico_width / 2;
  pos[1] = tft.height() / 2 - sleep_ico_height / 2;
  tft.drawXBitmap(pos[0],pos[1], sleep_ico_bits, sleep_ico_width, sleep_ico_height, TFT_LIGHTGREY);

  attachInterrupt(digitalPinToInterrupt(SHUTDOWN_BTTN), wake, FALLING);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  detachInterrupt(digitalPinToInterrupt(SHUTDOWN_BTTN));
  
}

void setup()
{
  Serial.begin(9600);

  pinMode(SHUTDOWN_BTTN, INPUT_PULLUP);

  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
#ifndef Simulide
  // Reading TFT ID:
  uint16_t ID = tft.readID();
  // Initializing TFT display:
  tft.begin(ID);

   // Fill TFT Screen with a color:
  tft.fillScreen(TFT_BLACK);
#else
  tft.begin();
  tft.print("Ready");
#endif

  
 
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

  if(digitalRead(SHUTDOWN_BTTN) == LOW) {
    shutdown();
  }
  
  if (sticky.pressed()) {
    Serial.println(F("Button pressed"));

    sticky.setDeadzone(0.3);
    renderAnalogStick(sticky); 
  }
}



