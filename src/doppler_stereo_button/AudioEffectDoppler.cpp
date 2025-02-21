#include "AudioEffectDoppler.h"
#include <string.h>


#define BUFFER_SIZE 1024

AudioEffectDoppler::AudioEffectDoppler() : 
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
