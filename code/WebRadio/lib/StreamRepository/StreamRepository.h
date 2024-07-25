#ifndef StreamRepository_h
#define StreamRepository_h

#include "ArduinoJson.h"
#include <LittleFS.h>

/**
 * {
 *   name: "name",
 *   host: "host"
 * }
 */

#define STREAMS_FILE "/streams.json"

class StreamRepository {

    public:
        StreamRepository();

        DynamicJsonDocument * stream_list = nullptr;

        boolean load();
        boolean save();

        String getPasswordBySSID(String ssid);
        JsonObject getCredentialByIndex(int index);
        int getCredentialIndexBySSID(String ssid);

};

#endif
