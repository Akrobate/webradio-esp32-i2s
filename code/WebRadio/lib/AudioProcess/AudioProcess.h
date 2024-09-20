#ifndef AudioProcess_h
#define AudioProcess_h

#include <Arduino.h>
#include "ArduinoJson.h"

#include <BusinessState.h>
#include <StreamRepository.h>
#include <Audio.h>

class AudioProcess {

    public:
        BusinessState * business_state = nullptr;
        StreamRepository * stream_repository = nullptr;

        Audio * audio;
        unsigned int I2S_DOUT = 25;
        unsigned int I2S_LRC =  26;
        unsigned int I2S_BCLK = 27;


        bool is_initialized = false;
        bool is_playing = false;

        int playing_stream_index = -1;
        int volume = 0;

        AudioProcess();

        void injectBusinesState(BusinessState * business_state);
        void injectStreamRepository(StreamRepository * stream_repository);

        void init();

};

#endif