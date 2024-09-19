#ifndef AudioProcess_h
#define AudioProcess_h

#include <Arduino.h>
#include <BusinessState.h>
#include <Audio.h>

class AudioProcess {

    private:

        BusinessState * business_state = nullptr;
        Audio * audio;
        unsigned int I2S_DOUT = 25;
        unsigned int I2S_LRC =  26;
        unsigned int I2S_BCLK = 27;


        bool is_initialized = false;
        bool is_playing = false;
        int volume = 0;
        String host = "";



    public:
        AudioProcess();

        void injectBusinesState(BusinessState * business_state);
        void init();

};

#endif