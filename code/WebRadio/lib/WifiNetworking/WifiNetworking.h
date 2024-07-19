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

        WifiNetworking();

        void scanDebug();
        DynamicJsonDocument * scan();

        void startAP();
        bool connect(char * ssid, char * password);
        bool isConnected();
        void disconnect();

};

#endif
