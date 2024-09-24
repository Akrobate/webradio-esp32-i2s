#ifndef InputInterface_h
#define InputInterface_h

#include <BusinessState.h>
#include <Arduino.h>

class InputInterface {

    private:
        BusinessState * business_state = nullptr;
        unsigned int PIN_ANALOG_1 = 34;

    public:

        int analog_1_value = 0;

        InputInterface();
        void injectBusinesState(BusinessState * business_state);
        
        void init();
        void update();

};

#endif