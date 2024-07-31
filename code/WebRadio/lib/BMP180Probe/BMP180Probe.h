#ifndef BMP180Probe_h
#define BMP180Probe_h

#define seaLevelPressure_hPa 1013.25

#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <BusinessState.h>

class BMP180Probe {

    private:
        Adafruit_BMP085 * bmp_180 = nullptr;
        bool is_initialized = false;

        BusinessState * business_state = nullptr;

        float temperature = 0;
        float pressure = 0;

    public:
        BMP180Probe();

        void injectBusinesState(BusinessState * business_state);
        void update();
        void updateBusinessState();

        void init();

        float getTemperature();
        float getPressure();

        bool isInitialized();

};

#endif