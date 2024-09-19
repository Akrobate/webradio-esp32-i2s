#include "AudioProcess.h"

AudioProcess::AudioProcess() {
}

void AudioProcess::injectBusinesState(BusinessState * business_state) {
    this->business_state = business_state;
}

void AudioProcess::init(){
    this->audio = new Audio();
    this->audio->setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    this->audio->setVolume(10);
}



//    // Audio
//    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
//    audio.setVolume(10);
//
//    disableCore0WDT(); // Disable watchdog on CORE0
//    xTaskCreatePinnedToCore(
//                  audioTask,   /* Function to implement the task */
//                  "audioTask", /* Name of the task */
//                  10000,      /* Stack size in words */
//                  NULL,       /* Task input parameter */
//                  15,          /* Priority of the task */
//                  NULL,       /* Task handle. */
//                  0);  /* Core where the task should run */