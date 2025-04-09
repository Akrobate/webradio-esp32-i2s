#ifndef WifiNetworking_h
#define WifiNetworking_h

#include <WiFi.h>
#include <ArduinoJson.h>

#include <BusinessState.h>
#include <NetworkCredentialRepository.h>

// #include "AsyncJson.h"
// #include "ArduinoJson.h"
// #include <LittleFS.h>

class WifiNetworking {

    public:
        SemaphoreHandle_t scanningNetworksMutex;
        WiFiClass * wifi;
        DynamicJsonDocument * available_networks = nullptr;
        
        BusinessState * business_state = nullptr;
        NetworkCredentialRepository * network_credential_repository = nullptr;

        WifiNetworking();

        void injectBusinessState(BusinessState * business_state);
        void injectNetworkCredentialRepository(NetworkCredentialRepository * network_credential_repository);

        void init();
        void scan();

        void startAP();
        void startAP(String ssid, String password);
        bool connect(String ssid, String password);
        void begin(String ssid, String password);
        bool isConnected();
        String getLocalIP();
        void disconnect();

        bool isNetworkAvailable(String ssid);
        void networkConnectionTask();
        String encryptionTypeToString(wifi_auth_mode_t encryptionType);
        DynamicJsonDocument * getAvailableNetworks();

};

#endif
