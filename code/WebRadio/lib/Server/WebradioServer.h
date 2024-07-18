#ifndef WebradioServer_h
#define WebradioServer_h
#include <LittleFS.h>


#include <ESPAsyncWebServer.h>

// #include "AsyncJson.h"
// #include "ArduinoJson.h"
// #include <LittleFS.h>

class WebradioServer {

    public:
        AsyncWebServer * server;
        WebradioServer();
        void init();
        void begin();
};

#endif
