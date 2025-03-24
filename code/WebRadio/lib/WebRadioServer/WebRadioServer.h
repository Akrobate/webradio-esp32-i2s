#ifndef WebRadioServer_h
#define WebRadioServer_h

// #define ASYNCWEBSERVER_REGEX

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include <WifiNetworking.h>
#include <NetworkCredentialRepository.h>
#include <StreamRepository.h>
#include <BusinessState.h>

// #include "AsyncJson.h"
// #define HTTP_CODE_OK 200
// #define HTTP_CODE_CREATED 201
// #define HTTP_CODE_BAD_REQUEST 400
// #define HTTP_CODE_NOT_FOUND 404
// #define HTTP_CODE_ERROR 500
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

        static constexpr int HTTP_CODE_OK = 200;
        static constexpr int HTTP_CODE_CREATED = 201;
        static constexpr int HTTP_CODE_BAD_REQUEST = 400;
        static constexpr int HTTP_CODE_NOT_FOUND = 404;
        static constexpr int HTTP_CODE_ERROR = 500;

        void injectWifiNetworking(WifiNetworking * wifi_networking);
        void injectNetworkCredentialRepository(NetworkCredentialRepository * network_credential);
        void injectStreamRepository(StreamRepository * stream_repository);
        void injectBusinessState(BusinessState * business_state);

        void init();
        void begin();

};

#endif
