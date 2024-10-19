#ifndef WifiNetworking_h
#define WifiNetworking_h

#include <WiFi.h>
#include <ArduinoJson.h>

// #include "AsyncJson.h"
// #include "ArduinoJson.h"
// #include <LittleFS.h>

class WifiNetworking {

    public:
        WiFiClass * wifi;
        DynamicJsonDocument * available_networks = nullptr;

        WifiNetworking();

        void scanDebug();
        void scan();


        DynamicJsonDocument * getAvailableNetworks();

        void startAP();
        void startAP(char * ssid, char * password);
        bool connect(char * ssid, char * password);
        bool isConnected();
        void disconnect();

};

#endif
