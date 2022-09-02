#ifndef Sound_cpp
#define Sound_cpp

#include <Arduino.h>

#define SpkL 50
#define SpkR 52

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

#endif