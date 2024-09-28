#ifndef InputInterface_h
#define InputInterface_h

#include <BusinessState.h>
#include <Arduino.h>

class InputInterface {

    private:
        BusinessState * business_state = nullptr;

    public:

        InputInterface();
        void injectBusinesState(BusinessState * business_state);
        
        void init();
        void update();

};

#endif