#ifndef DeviceSystem_h
#define DeviceSystem_h

#define seaLevelPressure_hPa 1013.25

#include <Arduino.h>
#include <BusinessState.h>
#include "time.h"

class DeviceSystem {

    private:

        BusinessState * business_state = nullptr;

    public:
        int total_free_bytes = 0;
        int minimum_free_bytes = 0;
        int largest_free_block = 0;

        String date_time = "";
        String device_started_at_date_time = "";
        String formated_date = "";
        String formated_time = "";
        bool date_time_configured = false;

        long gmt_offset_sec = 3600 * 1;
        int daylight_offset_sec = 3600 * 1;
        String ntp_server_host = "pool.ntp.org";

        DeviceSystem();

        void injectBusinesState(BusinessState * business_state);

        void init();
        void update();
        void configureTimeTask();

        int getTotalFreeBytes();
        int getMinimumFreeBytes();
        int getLargestFreeBlock();
        void showMemoryUsage();

        void updateBusinessState();

};

#endif