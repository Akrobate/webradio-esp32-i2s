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
    this->audio->setVolume(this->volume);

    disableCore0WDT();
    xTaskCreatePinnedToCore(
        [](void *arg){
            AudioProcess * audio_process = (AudioProcess *)arg;

            while(1) {
                if (audio_process->business_state->getIsConnectedToWifi()) {
                        if (audio_process->business_state->getPlayingVolume() != audio_process->volume) {
                            audio_process->volume = audio_process->business_state->getPlayingVolume();
                            audio_process->audio->setVolume(audio_process->volume);
                        }
                        if (audio_process->business_state->getPlayingStream() != audio_process->playing_stream_index) {
                            audio_process->playing_stream_index = audio_process->business_state->getPlayingStream();
                            if (audio_process->playing_stream_index >= 0) {
                                // @todo: check index exists
                                JsonObject stream = audio_process->stream_repository->getStreamByIndex(audio_process->playing_stream_index);
                                Serial.println("Playing stream: " + (String)audio_process->playing_stream_index);
                                Serial.println((const char *)stream["host"]);
                                audio_process->audio->connecttohost((const char *)stream["host"]);
                            } else {
                                audio_process->audio->stopSong();
                            }
                        }
                }

                // if (audio_process->playing_stream_index >= 0) {
                    audio_process->audio->loop();
                //}
                //vTaskDelay(pdMS_TO_TICKS(1));
                vTaskDelay(1);

            }
        },
        "audioTask", /* Name of the task */
        10000,      /* Stack size in words */
        this,       /* Task input parameter */
        15,          /* Priority of the task */ //15
        NULL,       /* Task handle. */
        0           /* Core where the task should run */
    );  

}
