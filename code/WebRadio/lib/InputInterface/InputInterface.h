#ifndef InputInterface_h
#define InputInterface_h

#include <Arduino.h>
#include <Toggle.h>

#include <BusinessState.h>

class InputInterface {

    private:
        BusinessState * business_state = nullptr;

        // pins to aviod: 0, 2, 12, 15, 6-11
        unsigned int PIN_BTN_1 = 16; // L-UP
        unsigned int PIN_BTN_2 = 23; // L-DOWN
        unsigned int PIN_BTN_3 = 17; // L-Validate

        unsigned int PIN_BTN_4 = 5; // R-UP
        unsigned int PIN_BTN_5 = 19; // R-DWON
        unsigned int PIN_BTN_6 = 18; // R-Validate

        bool onVolumeUp();
        bool onVolumeDown();
        bool onStreamNext();
        bool onStreamPrevious();

    public:

        Toggle * button_a_up = nullptr;
        Toggle * button_a_down = nullptr;
        Toggle * button_a_validate = nullptr;

        Toggle * button_b_up = nullptr;
        Toggle * button_b_down = nullptr;
        Toggle * button_b_validate = nullptr;


        InputInterface();
        void injectBusinesState(BusinessState * business_state);
        

        void init();
        void update();

        void processBusinessRules();

};

#endif