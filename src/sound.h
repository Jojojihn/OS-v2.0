#ifndef Sound_cpp
#define Sound_cpp

#include <Arduino.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define SpkL 50
#define SpkR 52

#define mp3TX 23
#define mp3RX 25

/**The speaker variables define how much that speaker is used**/
void playSound(bool rSpeaker, bool lSpeaker, float pitch, int length){
  if(rSpeaker){
    tone(SpkR, pitch, length);
  }
  if(lSpeaker){
    tone(SpkL, pitch, length);
  }
  delay(length);
  
}

void playRasputin() {
  SoftwareSerial mySoftwareSerial(mp3RX, mp3TX); // RX, TX
  DFRobotDFPlayerMini myDFPlayer;

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0);
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(10);
  myDFPlayer.play(1);
  

}





#endif