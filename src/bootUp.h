#ifndef Bootup_cpp
#define Bootup_cpp
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <color.h>
#include <sound.h>

void bootUp(MCUFRIEND_kbv & display){
    display.fillScreen(TFT_DARKGREY);
    display.fillCircle(display.width()/2, display.height()/2, (display.height()/2)-10, TFT_PRIMARY_COLOR);
    display.fillCircle(display.width()/2, display.height()/2, (display.height()/2)-30, TFT_DARKGREY);
    display.fillRect(display.width()/2, (display.height()/2)-((display.height()/3)/2), display.width()/2, display.height()/3, TFT_DARKGREY);
    playSound(true, false, 1244.51, 222);
    playSound(false, true, 622.254, 222);
    playSound(true, false, 932.328, 333);
    playSound(false, true, 830.609, 444);
    playSound(true, false, 1244.51, 333);
    playSound(false, true, 932.328, 888);
}

#endif