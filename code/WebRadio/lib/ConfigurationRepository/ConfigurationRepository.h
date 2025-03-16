#ifndef ConfigurationRepository_h
#define ConfigurationRepository_h

#include "ArduinoJson.h"
#include <LittleFS.h>
#include <BusinessState.h>

/**
 * {
 * }
 */


#define CONFIGURATION_FILE "/configurations.json"

class ConfigurationRepository {

    private:
        BusinessState * business_state = nullptr;

    public:

        long gmt_offset_sec = 3600 * 1;
        int daylight_offset_sec = 3600 * 1;
        String ntp_server_host = "pool.ntp.org";
        String access_point_ssid = "ESP32-Access-Point";
        String access_poinrt_password = "123456789"; 


        ConfigurationRepository();

        void init();
        bool load();
        bool save();

        void injectBusinessState(BusinessState * business_state);
        void updateBusinessState();

};

#endif
