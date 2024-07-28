#ifndef DeviceSystem_h
#define DeviceSystem_h

#define seaLevelPressure_hPa 1013.25

#include <Arduino.h>
#include <BusinessState.h>

class DeviceSystem {

    private:

        BusinessState * business_state = nullptr;

    public:
        int min_total_free_bytes = 0;
        int max_total_free_bytes = 0;

        int total_free_bytes = 0;
        int minimum_free_bytes = 0;
        int largest_free_block = 0;

        void injectBusinesState(BusinessState * business_state);

        DeviceSystem();

        void update();

        int getTotalFreeBytes();
        int getMinimumFreeBytes();
        int getLargestFreeBlock();

        void showMemoryUsage();

        void updateBusinessState();

};

#endif