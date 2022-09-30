#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <Joystick.h>
#include <NoSwitches.h>
#include <img/sleep_ico_rain.c>
#include <LowPower.h>
#include <bootUp.h>
#include <SPI.h>
#include <MFRC522.h>
#include <login.h>

//Comment/Uncomment this to switch between Simulide mode and hardware mode. Simulide uses a different Display and is used for debugging
//#define Simulide

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
#define RandomPin A15
Joystick sticky = Joystick(VrX, VrY, Bttn);

#define SHUTDOWN_BTTN 20

Button shutdownButton = Button(SHUTDOWN_BTTN);

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

    while(!sticky.justPressed()) {
      //Don't redraw when the position didn't change (with some margin)
      if(abs(prev.x - stick.getAxes().x) < 0.02 && abs(prev.y - stick.getAxes().y) < 0.02) {
        delay(5);
        continue;
      }
      //Clear previous stick circle
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

/**
 * Draw an RLE encoded RGB565 Bitmap.
 * 
 * Run length encoding stores a count followed by the color to
 * specify how many times in a row the color needs to be drawn (how long it "runs").
 * In this implementation counts and color are separated into two arrays.
 * 
 * @param posX X position of top left pixel
 * @param posY Y position of top left pixel
 * @param colors Array of RGB565 colors
 * @param counts Array of counts corresponding to the colors
 * @param mask 1 Bit per pixel mask, set bit means opaque
 * @param width width of the image
 * @param height height of the image
 */
void drawRleRGBBitmap(int posX, int posY, const uint16_t *colors, const uint8_t *counts, const uint8_t *mask, uint16_t width, uint16_t height) {
  int count = 0; //The amount of times a color needs to be drawn
  int countIndex = 0; //Index into the count array

  int currentColor = 0; //The color currently being drawn
  int colorIndex = 0; //Index into the color array

  bool doDraw = true; //Used for masking
  uint8_t maskCount = 0; //How many pixels until the mask switches (Starts with unset)
  int maskIndex = 0;

  uint8_t currentMask = 0;
  tft.startWrite();
  for(uint16_t y = 0; y < height; y++) {
    for(uint8_t x = 0; x < width; x++) {
      if(count == 0) { //When count is 0 we need to load the next color and how many times it needs to be drawn
        count = pgm_read_byte(&counts[countIndex]); //Next color for this many pixels
        countIndex++;
        currentColor = pgm_read_word(&colors[colorIndex]); //Next color is this
        colorIndex++;
      }
      count--;//decrement count with every pixel
  
      while(maskCount == 0) {
        doDraw = !doDraw;
        maskCount = pgm_read_byte(&mask[maskIndex]);
        maskIndex++;
      } 
      maskCount--;
      /*
      if(x & 7) //If the current mask byte still has more bits to be read
        currentMask <<= 1; //Move the next bit into the mask position
      else //Otherwise next byte needs to be read from mask array
        currentMask = pgm_read_byte(&mask[y * ((width + 7) / 8) + x / 8]);
      */
      if(doDraw) { //Only draw if not masked away
        tft.writePixel(posX + x, posY + y, currentColor);
      }
     
    }
  }
  tft.endWrite();
}


void wake() {}
void shutdown() {
  int pos[] = {0,0};
  pos[0] = tft.width() / 2 - sleep_ico_rain_width / 2;
  pos[1] = tft.height() / 2 - sleep_ico_rain_height / 2;

  //tft.drawXBitmap(pos[0],pos[1], sleep_ico_bits, sleep_ico_width, sleep_ico_height, TFT_LIGHTGREY);
  //tft.drawRGBBitmap(pos[0], pos[1], sleep_ico_rain_bits, sleep_ico_rain_bits_mask, sleep_ico_rain_width, sleep_ico_rain_height);
  tft.fillScreen(TFT_BLACK);
  drawRleRGBBitmap(pos[0], pos[1], sleep_icon_rain_colors_new, sleep_icon_rain_counts_new, sleep_ico_RLE_mask, sleep_ico_rain_width, sleep_ico_rain_height);
  stopMP3();
  attachInterrupt(digitalPinToInterrupt(SHUTDOWN_BTTN), wake, FALLING);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  detachInterrupt(digitalPinToInterrupt(SHUTDOWN_BTTN));
}



void setup()
{
  //sticky.is
  Serial.begin(9600);
  SPI.begin();
  randomSeed(analogRead(RandomPin));
  pinMode(SHUTDOWN_BTTN, INPUT_PULLUP);
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
#ifndef Simulide
  // Reading TFT ID:
  uint16_t ID = tft.readID();
  // Initializing TFT display:
  tft.begin(ID);
  tft.setRotation(3);
  bootUp(tft); 
#else
  tft.begin();
  tft.print("Ready");
#endif
  waitForLogIn(tft);
}



void loop()
{
  delay(50);
  if(shutdownButton.justPressed()) {
    shutdown();
    while(shutdownButton.isPressed());
  }
  
  if (sticky.justPressed()) {
    Serial.println(F("Button pressed"));
    sticky.setDeadzone(0.3);
    renderAnalogStick(sticky); 
  }
}



