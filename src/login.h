#ifndef Login_h
#define Login_h

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <sound.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <color.h>

#define SS_PIN 53
#define RST_PIN 49
MFRC522 mfrc522(SS_PIN, RST_PIN);
String currentUID;

boolean loggedIn = false;
void waitForLogIn(Adafruit_GFX & display){
    mfrc522.PCD_Init();
    display.fillScreen(TFT_PRIMARY_COLOR);
    display.setTextSize(2);
    display.print("Please log in using RFID Card");
    while (!loggedIn){
        if (mfrc522.PICC_IsNewCardPresent()){
            String id;
            if (mfrc522.PICC_ReadCardSerial()){
                for (byte i = 0; i < mfrc522.uid.size; i++){
                    id += mfrc522.uid.uidByte[i];
                }
            }
            if(currentUID!=id){
                currentUID=id;
            }
            loggedIn=true;
            display.fillScreen(TFT_WHITE);
        }
    }
}

#endif