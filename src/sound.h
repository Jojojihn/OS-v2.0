#ifndef Sound_cpp
#define Sound_cpp

#include <Arduino.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define mp3TX 52
#define mp3RX 53

DFRobotDFPlayerMini myDFPlayer;

/**
 * @brief Well it sets up the MP player genius, call it once at bootup
 * 
 */
void setupMP3(){
  SoftwareSerial mySoftwareSerial(mp3RX, mp3TX); // RX, TX
  mySoftwareSerial.begin(9600);
  

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
}
/**
 * @brief Plays and MP3 file with the given Idea and volume
 * 
 * @param id The ID of the MP3 file to play
 * @param volume The Volume the MP3 player uses Range:0-1 (Optional)
 */
void playMP3byID(int id, float volume = -1){
  myDFPlayer.play(id);
  if(volume!=-1){
    myDFPlayer.volume(max(volume*30, 30));
  }
}

/**
 * @brief Sets the volume of the MP3 player
 * 
 * @param volume The Volume the MP3 Player is supposed to switch to Range:0-1
 */
void MP3setVolume(float volume){
  myDFPlayer.volume(volume*30);
}

/**
 * @brief Stops the MP3 player
 * 
 */
void stopMP3(){
  myDFPlayer.stop();
}

/**
 * @brief Pauses the MP3 player, in order to resume the MP3 later
 * 
 */
void pauseMP3(){
  myDFPlayer.pause();
}

/**
 * @brief Resumes the paused MP3
 * 
 */
void resumeMP3(){
  myDFPlayer.start();
}

/**
 * @brief Plays a random ad
 * 
 */
void playAnAd(){
  myDFPlayer.advertise(1);
}


#endif