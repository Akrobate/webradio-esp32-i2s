#ifndef ConfigurationRepository_h
#define ConfigurationRepository_h

#include "ArduinoJson.h"
#include <LittleFS.h>
#include <BusinessState.h>

/**
 * {
 *  "ntp_server_host": "pool.ntp.org",
 *  "gmt_offset_sec": 3600,
 *  "daylight_offset_sec": 3600,
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
        String access_point_password = "123456789"; 

        void setNTPServerHost(String ntp_server_host);
        void setGMTOffsetSec(long gmt_offset_sec);
        void setDaylightOffsetSec(int daylight_offset_sec);

        void setAccessPointSSID(String access_point_ssid);
        void setAccessPointPassword(String access_point_password);

        // @todo add access point ssid and password getter
        String getAccessPointSSID();
        String getAccessPointPassword();


        ConfigurationRepository();

        void init();
        bool load();
        bool save();

        void injectBusinessState(BusinessState * business_state);
        void updateBusinessState();

};

#endif
