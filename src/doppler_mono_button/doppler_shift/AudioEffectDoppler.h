#ifndef AUDIO_EFFECT_DOPPLER
#define AUDIO_EFFECT_DOPPLER
#include <Audio.h>

#define BUFFER_SIZE 1024
class AudioEffectDoppler: public AudioStream
{
  public:
    AudioEffectDoppler();
    ~AudioEffectDoppler();

    void setDopplerFactor(float factor);
    virtual void update(void);
    void setDelay(int M);
  private:
    audio_block_t *inputQueueArray[1];
    float A[BUFFER_SIZE];
    float rptr;
    int wptr;
    float g;
    float delayline(float x); // on met dans private pour que ce ne soit que la classe qui utilise
    // surtout avec la manip des pointeurs et du buffer circulaire
    
};


#endif