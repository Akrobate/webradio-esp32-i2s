#ifndef BusinessState_h
#define BusinessState_h

#include <Arduino.h>

class BusinessState {

    private:
        float temperature = 0;
        float pressure = 0;

        int total_free_bytes = 0;

        bool is_connected_to_wifi = false;
        String connected_to_ssid = "";
        String local_ip = "";

        String access_point_ssid = "";

    public:
        BusinessState();

        void setTemperature(float temperature);
        float getTemperature();

        void setPressure(float pressure);
        float getPressure();

        void setTotalFreeBytes(int total_free_bytes);
        int getTotalFreeBytes();

        void setIsConnectedToWifi(bool is_connected_to_wifi);
        bool getIsConnectedToWifi();

        void setConnectedToSSID(String connected_to_ssid);
        String getConnectedToSSID();

        void setLocalIP(String local_ip);
        String getLocalIP();

        void setAccessPointSSID(String access_point_ssid);
        String getAccessPointSSID();

};

#endif
