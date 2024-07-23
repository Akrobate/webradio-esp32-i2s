#include "BusinessState.h"

BusinessState::BusinessState() {
    this->temperature = 0;
    this->pressure = 0;
    this->access_point_ssid = "";
}

void BusinessState::setTemperature(float temperature) {
    this->temperature = temperature;
}

float BusinessState::getTemperature() {
    return this->temperature;
}

void BusinessState::setPressure(float pressure) {
    this->pressure = pressure;
}

float BusinessState::getPressure() {
    return this->pressure;
}



