#include "InputInterface.h"


InputInterface::InputInterface() {
}

void InputInterface::injectBusinesState(BusinessState * business_state) {
    this->business_state = business_state;
}


void InputInterface::init() {

    pinMode(this->PIN_BTN_1, INPUT_PULLUP);
    pinMode(this->PIN_BTN_2, INPUT_PULLUP);
    pinMode(this->PIN_BTN_3, INPUT_PULLUP);
    pinMode(this->PIN_BTN_4, INPUT_PULLUP);
    pinMode(this->PIN_BTN_5, INPUT_PULLUP);
    pinMode(this->PIN_BTN_6, INPUT_PULLUP);
    
    this->button_a_up = new Toggle(this->PIN_BTN_1);
    this->button_a_down = new Toggle(this->PIN_BTN_2);
    this->button_a_validate = new Toggle(this->PIN_BTN_3);
    this->button_b_up = new Toggle(this->PIN_BTN_4);
    this->button_b_down = new Toggle(this->PIN_BTN_5);
    this->button_b_validate = new Toggle(this->PIN_BTN_6);
}


void InputInterface::update() {

    this->button_a_up->poll();
    this->button_a_down->poll();
    this->button_a_validate->poll();
    this->button_b_up->poll();
    this->button_b_down->poll();
    this->button_b_validate->poll();


    /*
    if (this->button_a_up->onPress()) {
        Serial.println("Button A UP pressed");
    }
    if (this->button_a_down->onPress()) {
        Serial.println("Button A DOWN pressed");
    }
    if (this->button_a_validate->onPress()) {
        Serial.println("Button A VALIDATE pressed");
    }

    if (this->button_b_up->onPress()) {
        Serial.println("Button B UP pressed");
    }
    if (this->button_b_down->onPress()) {
        Serial.println("Button B DOWN pressed");
    }
    if (this->button_b_validate->onPress()) {
        Serial.println("Button B VALIDATE pressed");
    }
    */

   this->processBusinessRules();
}

bool InputInterface::onVolumeUp() {
    return this->button_a_up->onPress();
}

bool InputInterface::onVolumeDown() {
    return this->button_a_down->onPress();
}

bool InputInterface::onStreamNext() {
    return this->button_b_down->onPress();
}

bool InputInterface::onStreamPrevious() {
    return this->button_b_up->onPress();
}


void InputInterface::processBusinessRules() {

    if (this->onVolumeUp()) {
        this->business_state->lock();
        this->business_state->setPlayingVolume(this->business_state->getPlayingVolume() + 1);
        this->business_state->unlock();
    }

    if (this->onVolumeDown()) {
        this->business_state->lock();
        this->business_state->setPlayingVolume(this->business_state->getPlayingVolume() - 1);
        this->business_state->unlock();
    }

    if (this->onStreamPrevious()) {
        this->business_state->lock();
        this->business_state->setPlayingStream(this->business_state->getPlayingStream() - 1);
        this->business_state->unlock();
    }

    if (this->onStreamNext()) {
        this->business_state->lock();
        this->business_state->setPlayingStream(this->business_state->getPlayingStream() + 1);
        this->business_state->unlock();
    }
}





