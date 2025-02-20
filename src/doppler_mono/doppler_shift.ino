#include <Audio.h>
#include "AudioEffectDoppler.h"


AudioEffectDoppler dopplerEffect;
AudioInputUSB usbInput;
AudioOutputI2S out;
AudioOutputUSB outputUsb;
AudioControlSGTL5000 audioShield;
AudioConnection patchCord0(usbInput,0,dopplerEffect,0);
AudioConnection patchCord1(dopplerEffect,0,out,1);
AudioConnection patchCord2(dopplerEffect,0,out,0);


void setup() {
  AudioMemory(10);
  audioShield.enable();
  audioShield.volume(0.8);
  dopplerEffect.setDelay(100);
}

void loop() {

  float vol = usbInput.volume();

  if (vol <0){
    vol = 0.3 + vol*0.5;
  }
  audioShield.volume(vol);
  float newG =(analogRead(A0)/1023.0)*0.6-0.3; // Potentiomètre sur A0
  Serial.println(newG);
  dopplerEffect.setDopplerFactor(newG);

}
