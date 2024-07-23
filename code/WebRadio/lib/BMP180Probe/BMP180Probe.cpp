#include "BMP180Probe.h"


BMP180Probe::BMP180Probe() {
}


void BMP180Probe::init() {
    this->bmp_180 = new Adafruit_BMP085();
    if (!this->bmp_180->begin()) {
        this->is_initialized = false;
    } else {
        this->is_initialized = true;
    }
}


float BMP180Probe::getPressure() {
    return (float)this->bmp_180->readPressure() / 100;
}


float BMP180Probe::getTemperature() {
    return this->bmp_180->readTemperature();
}


void BMP180Probe::injectBusinesState(BusinessState * business_state) {
    this->business_state = business_state;
}


void BMP180Probe::updateBusinessState() {
    if (this->is_initialized) {
        this->business_state->setTemperature(this->getTemperature());
        this->business_state->setPressure(this->getPressure());
    }
}


bool BMP180Probe::isInitialized() {
    return this->is_initialized;
}
