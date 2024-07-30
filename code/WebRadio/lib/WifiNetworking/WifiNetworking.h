#ifndef WifiNetworking_h
#define WifiNetworking_h

#include <WiFi.h>
#include <ArduinoJson.h>

#include <BusinessState.h>
// #include "AsyncJson.h"
// #include "ArduinoJson.h"
// #include <LittleFS.h>

class WifiNetworking {

    public:

        SemaphoreHandle_t scanningNetworksMutex;


        WiFiClass * wifi;
        DynamicJsonDocument * available_networks = nullptr;

        BusinessState * business_state = nullptr;

        WifiNetworking();

        void scanDebug();
        void scan();


        DynamicJsonDocument * getAvailableNetworks();

        void startAP();
        bool connect(String ssid, String password);
        void begin(String ssid, String password);
        bool isConnected();
        String getLocalIP();
        void disconnect();

        void injectBusinessState(BusinessState * business_state);

        String encryptionTypeToString(wifi_auth_mode_t encryptionType);
        bool isNetworkAvailable(String ssid);

};

#endif
