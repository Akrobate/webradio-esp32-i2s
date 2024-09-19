#include "AudioProcess.h"

AudioProcess::AudioProcess() {
}


void AudioProcess::injectBusinesState(BusinessState * business_state) {
    this->business_state = business_state;
}


void AudioProcess::injectStreamRepository(StreamRepository * stream_repository) {
    this->stream_repository = stream_repository;
}

void AudioProcess::init(){
    this->audio = new Audio();
    this->audio->setPinout(this->I2S_BCLK, this->I2S_LRC, this->I2S_DOUT);
    this->audio->setVolume(10);

    disableCore0WDT();
    xTaskCreatePinnedToCore(
        [](void *arg){
            AudioProcess * audio_process = (AudioProcess *)arg;

            while(1) {
                if (audio_process->business_state->getIsConnectedToWifi()) {
                    if (!audio_process->is_playing) {
                        // audio_process->audio->connecttohost("http://broadcast.infomaniak.ch/radioclassique-high.mp3");
                        // audio_process->is_playing = true;
                    }
                    audio_process->audio->loop();
                }
            }
        },
        "audioTask", /* Name of the task */
        10000,      /* Stack size in words */
        this,       /* Task input parameter */
        15,          /* Priority of the task */
        NULL,       /* Task handle. */
        0           /* Core where the task should run */
    );  

}
