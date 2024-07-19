#ifndef WebRadioServer_h
#define WebRadioServer_h

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

// #include "AsyncJson.h"
// #include <LittleFS.h>

class WebRadioServer {

    private:
        int port = 80;

    public:
        AsyncWebServer * server;
        WebRadioServer();
        void init();
        void begin();
};

#endif
