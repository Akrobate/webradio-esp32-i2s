#ifndef DisplayScreen_h
#define DisplayScreen_h

#include <Arduino.h>
#include <U8g2lib.h>

#include <BusinessState.h>
#include <StreamRepository.h>

class DisplayScreen {

    private:
        U8G2_SH1106_128X64_NONAME_F_HW_I2C * u8g2;
        BusinessState * business_state = nullptr;
        StreamRepository * stream_repository = nullptr;

        int displayed_volume = 0;

    public:
        DisplayScreen();
        void injectBusinesState(BusinessState * business_state);
        void injectStreamRepository(StreamRepository * stream_repository);

        void init();
        void clear();
        void display();

        void displayStack();

        void infoScreen();
        void infoTestScreen();

        void splashScreen();
        void standbyScreen();
        void playingStreamScreen();
        void configuringTimeScreen();
        void volumeScreen();
        void connectingScreen();
        void streamSelectionScreen();

};

#endif
