#ifndef Bootup_cpp
#define Bootup_cpp
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <color.h>
#include <sound.h>

void bootUp(Adafruit_GFX & display){
    display.fillScreen(TFT_DARKGREY);
    display.fillCircle(display.width()/2, display.height()/2, (display.height()/2)-10, TFT_PRIMARY_COLOR);
    display.fillCircle(display.width()/2, display.height()/2, (display.height()/2)-30, TFT_DARKGREY);
    display.fillRect(display.width()/2, (display.height()/2)-((display.height()/3)/2), display.width()/2, display.height()/3, TFT_DARKGREY);
    setupMP3();
}

#endif