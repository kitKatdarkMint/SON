#include <Audio.h>
#include "AudioEffectDoppler.h"

// Effets Doppler
AudioEffectDoppler dopplerEffectL;
AudioEffectDoppler dopplerEffectR;
AudioInputUSB usbInput;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;
AudioConnection patchCord0(usbInput, 0, dopplerEffectL, 0);
AudioConnection patchCord1(usbInput, 1, dopplerEffectR, 0);
AudioConnection patchCord2(dopplerEffectL, 0, out, 0);
AudioConnection patchCord3(dopplerEffectR, 0, out, 1);

bool effectEnabled = false;  // État du bouton MIDI

void setup() {
  AudioMemory(10);
  audioShield.enable();
  audioShield.volume(0.8);

  usbMIDI.setHandleControlChange(controlChange);
}

void loop() {
  usbMIDI.read();  

  // Contrôle du volume en fonction de l’entrée USB
  float vol = usbInput.volume();
  audioShield.volume(vol < 0 ? 0.3 + vol * 0.5 : vol);
}

// Fonction pour gérer les messages MIDI reçus
void controlChange(byte channel, byte control, byte value) {
  float mappedValue = (value / 127.0) * 0.4 - 0.2; 

  if (control == 10) {  // Potentiomètre gauche (MIDI CC 10)
    dopplerEffectL.setDopplerFactor(mappedValue);
  }
  else if (control == 11) {  // Potentiomètre droit (MIDI CC 11)
    dopplerEffectR.setDopplerFactor(mappedValue);
  }
  else if (control == 12) {  // Bouton MIDI CC 12
    effectEnabled = value > 64; // Seuil ON/OFF

    if (!effectEnabled) { // Désactiver l'effet si OFF
        dopplerEffectL.setDopplerFactor(0.0);
        dopplerEffectR.setDopplerFactor(0.0);
    }
  }
}


