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

    xTaskCreate(
        [](void *arg){
            BMP180Probe * bmp_180_probe = (BMP180Probe *)arg;
            while (1) {
                bmp_180_probe->update();
                bmp_180_probe->updateBusinessState();
                vTaskDelay(pdMS_TO_TICKS(10000));
            }
        },
        "Task Temperature",
        2000,
        this,
        1,
        NULL
    );
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

void BMP180Probe::update() {
    if (this->is_initialized) {
        this->temperature = this->getTemperature();
        this->pressure = this->getPressure();
    }
}


void BMP180Probe::updateBusinessState() {
    if (this->is_initialized) {
        if (this->business_state->lock()) {
            this->business_state->setTemperature(this->temperature);
            this->business_state->setPressure(this->pressure);
            this->business_state->unlock();
        }
    }
}


bool BMP180Probe::isInitialized() {
    return this->is_initialized;
}
