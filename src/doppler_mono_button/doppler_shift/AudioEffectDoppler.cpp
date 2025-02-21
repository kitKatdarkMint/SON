#include "AudioEffectDoppler.h"
#include <string.h>

//#define AUDIO_OUTPUTS 1
#define BUFFER_SIZE 1024

AudioEffectDoppler::AudioEffectDoppler() : 
//AudioStream(AUDIO_OUTPUTS, new audio_block_t*[AUDIO_OUTPUTS]),
AudioStream(1, inputQueueArray)
{
  memset(A,0,sizeof(A));
  wptr = 0;
  rptr = 0.0f;
  g = 0.0f;
}

AudioEffectDoppler::~AudioEffectDoppler(){}

void AudioEffectDoppler::setDopplerFactor(float factor){
   g = factor;
}


void AudioEffectDoppler::setDelay(int M)
{
    rptr = float(wptr - M);
    while (rptr < 0)
    {
        rptr += BUFFER_SIZE;
    }
}

float AudioEffectDoppler::delayline(float x){
  A[wptr] = x;
  wptr = (wptr+1) % BUFFER_SIZE;
  
  int idx = (int)rptr;
  float frac = rptr - idx;
  float y1 = A[idx];
  float y2 = A[(idx + 1) % BUFFER_SIZE];
  float y = (1.0f - frac) * y1 + frac * y2;
  // static float g_filtered = 0;
  // g_filtered = 0.9 * g_filtered + 0.1* g;
  rptr += (1.0f - g);
  if(rptr >= BUFFER_SIZE)
    rptr -= BUFFER_SIZE;
  else if(rptr < 0)
    rptr += BUFFER_SIZE;
  // if (wptr >= A+BUFFER_SIZE ) 
  //   wptr -= BUFFER_SIZE; // si le pointeur d'ecriture excede buffer remise a zero
  
  // long rpi = (long)floor(rptr -A);
  // float a = (rptr-A) - (float)rpi;
  // float y1 = A[rpi];
  // float y2 = A[(rpi + 1) % BUFFER_SIZE];
  // float y = (1.0f - a) * y1 + a * y2;

  // rptr = A + ((long)(rptr - A + (1.0f - g)) % BUFFER_SIZE);

  // if (rptr >= A + BUFFER_SIZE) rptr -= BUFFER_SIZE;
  // if (rptr < A) rptr += BUFFER_SIZE;

  return y;
}

void AudioEffectDoppler::update(void){
  audio_block_t *block = receiveWritable(0);
  if (!block) return;  // Si aucun bloc n'est disponible, on arrête

  for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      block->data[i] = delayline(block->data[i]);
  }

  transmit(block);
  release(block);
}
//   audio_block_t* outBlock[AUDIO_OUTPUTS];
//   for (int channel = 0; channel < AUDIO_OUTPUTS; channel++) {
//     outBlock[channel] = allocate();
//     if (outBlock[channel]) {
//       for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
//           block->data[i] = delayline(block->data[i]);
    
//     }
//     transmit(outBlock[channel], channel);
//     release(outBlock[channel]);

//     }
// }