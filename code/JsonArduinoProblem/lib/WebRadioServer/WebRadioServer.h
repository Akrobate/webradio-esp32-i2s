#ifndef WebRadioServer_h
#define WebRadioServer_h

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include <WifiNetworking.h>
#include <NetworkCredential.h>

// #include "AsyncJson.h"

class WebRadioServer {

    private:
        int port = 80;
        WifiNetworking * wifi_networking = nullptr;
        NetworkCredential * network_credential = nullptr;

    public:
        AsyncWebServer * server;
        WebRadioServer();
        void injectWifiNetworking(WifiNetworking * wifi_networking);
        void injectNetworkCredential(NetworkCredential * network_credential);

        void init();
        void begin();

};

#endif
