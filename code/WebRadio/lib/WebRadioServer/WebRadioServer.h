#ifndef WebRadioServer_h
#define WebRadioServer_h

#define ASYNCWEBSERVER_REGEX

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include <WifiNetworking.h>
#include <NetworkCredentialRepository.h>
#include <StreamRepository.h>
#include <BusinessState.h>

// #include "AsyncJson.h"

class WebRadioServer {

    private:
        int port = 80;
        WifiNetworking * wifi_networking = nullptr;
        BusinessState * business_state = nullptr;
        NetworkCredentialRepository * network_credential = nullptr;
        StreamRepository * stream_repository = nullptr;

    public:
        AsyncWebServer * server;
        WebRadioServer();
        void injectWifiNetworking(WifiNetworking * wifi_networking);
        void injectNetworkCredentialRepository(NetworkCredentialRepository * network_credential);
        void injectStreamRepository(StreamRepository * stream_repository);
        void injectBusinessState(BusinessState * business_state);

        void init();
        void begin();

};

#endif
