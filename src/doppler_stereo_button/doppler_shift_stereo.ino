#include <Audio.h>
#include "AudioEffectDoppler.h"


AudioEffectDoppler dopplerEffectL;
AudioEffectDoppler dopplerEffectR;
AudioInputUSB usbInput;
AudioOutputI2S out;
//AudioOutputUSB outputUsb;
AudioControlSGTL5000 audioShield;
AudioConnection patchCord0(usbInput,0,dopplerEffectL,0);//gauche
AudioConnection patchCord1(usbInput, 1,dopplerEffectR, 0);//droite
AudioConnection patchCord2(dopplerEffectL,0,out,0);
AudioConnection patchCord3(dopplerEffectR,0,out,1);


bool effectEnabled = false;  // État du pitch shift (ON/OFF)
bool lastButtonState = LOW;  // État précédent du bouton

void setup() {
  AudioMemory(10);
  audioShield.enable();
  audioShield.volume(0.8);
  pinMode(0, INPUT);
  dopplerEffectL.setDelay(100);
  dopplerEffectR.setDelay(100);
}

void loop() {

  float vol = usbInput.volume();

  if (vol <0){
    vol = 0.3 + vol*0.5;
  }
  audioShield.volume(vol);

  bool buttonState = digitalRead(0);
  if (buttonState == HIGH && lastButtonState == LOW) {
    effectEnabled = !effectEnabled;  // Inversion de l'état du pitch shift
  }
  lastButtonState = buttonState; 
  
  if (effectEnabled) {
    float newGL = (analogRead(A0) / 1023.0) * 0.4 - 0.2; // Potentiomètre gauche (A0)
    float newGR = (analogRead(A2) / 1023.0) * 0.4 - 0.2; // port A2 (droite) pour le second potentiometre sur le port 16

    // Application des valeurs aux effets Doppler séparés
    dopplerEffectL.setDopplerFactor(newGL);
    dopplerEffectR.setDopplerFactor(newGR);
    }
  else {
    dopplerEffectL.setDopplerFactor(0.0); // Désactive le pitch shift
    dopplerEffectR.setDopplerFactor(0.0);
  }
}
