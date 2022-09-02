#ifndef Bootup_cpp
#define Bootup_cpp
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <color.h>

void bootUp(MCUFRIEND_kbv & display){
    display.fillScreen(TFT_WHITE);
    display.fillCircle(display.width()/2, display.height()/2, (display.width()/2)-10, TFT_PRIMARY_COLOR);
    display.fillCircle(display.width()/2, display.height()/2, (display.width()/2)-30, TFT_WHITE);
    display.fillRect(0, 0, 50, 50, TFT_BLACK);
}

#endif