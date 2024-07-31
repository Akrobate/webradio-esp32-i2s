#ifndef BusinessState_h
#define BusinessState_h

#include <Arduino.h>

class BusinessState {

    private:

        SemaphoreHandle_t business_state_mutex;

        // WiFi
        bool is_connected_to_wifi = false;
        bool is_connecting_to_wifi = false;

        String connected_to_ssid = "";
        String local_ip = "";
        String access_point_ssid = "";

        // System
        int total_free_bytes = 0;
        int minimum_free_bytes = 0;

        // weather report
        float temperature = 0;
        float pressure = 0;

        // time
        String date_time = "";
        bool date_time_configured = false;

        // NTP
        long gmt_offset_sec = 0;
        int daylight_offset_sec = 0;
        String ntp_server_host = "";


    public:
        BusinessState();

        bool lock();
        void unlock();

        void setTemperature(float temperature);
        float getTemperature();

        void setPressure(float pressure);
        float getPressure();

        void setTotalFreeBytes(int total_free_bytes);
        int getTotalFreeBytes();

        void setMinimumFreeBytes(int minimum_free_bytes);
        int getMinimumFreeBytes();

        void setIsConnectedToWifi(bool is_connected_to_wifi);
        bool getIsConnectedToWifi();

        void setConnectedToSSID(String connected_to_ssid);
        String getConnectedToSSID();

        void setLocalIP(String local_ip);
        String getLocalIP();

        void setAccessPointSSID(String access_point_ssid);
        String getAccessPointSSID();

        void setIsConnectingToWifi(bool is_connecting_to_wifi);
        bool getIsConnectingToWifi();


        void setGMTOffsetSec(long gmt_offset_sec);
        long getGMTOffsetSec();

        void setDaylightOffsetSec(int daylight_offset_sec);
        int getDaylightOffsetSec();

        void setNTPServerHost(String ntp_server_host);
        String getNTPServerHost();
        const char * getNTPServerHostChar();

        void setDateTime(String date_time);
        String getDateTime();

        void setDateTimeConfigured(bool date_time_configured);
        bool getDateTimeConfigured();

};

#endif
