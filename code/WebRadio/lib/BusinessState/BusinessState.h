#ifndef BusinessState_h
#define BusinessState_h

#include <Arduino.h>

class BusinessState {



    public:
        BusinessState();

        float temperature = 0;
        float pressure = 0;

        String access_point_ssid = "";


        void setTemperature(float temperature);
        float getTemperature();

        void setPressure(float pressure);
        float getPressure();

};

#endif
