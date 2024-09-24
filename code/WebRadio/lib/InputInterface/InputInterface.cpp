#include "InputInterface.h"


InputInterface::InputInterface() {
}

void InputInterface::injectBusinesState(BusinessState * business_state) {
    this->business_state = business_state;
}


void InputInterface::init() {
}


void InputInterface::update() {
    this->analog_1_value = analogRead(this->PIN_ANALOG_1);
}

