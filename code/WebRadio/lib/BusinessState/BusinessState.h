#ifndef BusinessState_h
#define BusinessState_h

#include <Arduino.h>

class BusinessState {

    private:
        float temperature = 0;
        float pressure = 0;

        float total_free_bytes = 0;

    public:
        BusinessState();

        String access_point_ssid = "";

        void setTemperature(float temperature);
        float getTemperature();

        void setPressure(float pressure);
        float getPressure();

        void setTotalFreeBytes(float total_free_bytes);
        float getTotalFreeBytes();
};

#endif
