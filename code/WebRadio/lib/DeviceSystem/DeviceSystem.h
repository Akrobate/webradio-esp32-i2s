#ifndef DeviceSystem_h
#define DeviceSystem_h

#define seaLevelPressure_hPa 1013.25

#include <Arduino.h>
#include <Adafruit_BMP085.h>


class DeviceSystem {

    private:

        

    public:
        int total_free_bytes = 0;
        int minimum_free_bytes = 0;
        int largest_free_block = 0;

        DeviceSystem();

        void update();

        int getTotalFreeBytes();
        int getMinimumFreeBytes();
        int getLargestFreeBlock();

};

#endif