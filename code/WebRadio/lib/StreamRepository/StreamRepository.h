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

        void init();

        void addStream(String name, String host);
        void updateStream(int index, String name, String host);
        void removeStream(int index);
        int countStream();

        JsonObject getStreamByIndex(int index);


};

#endif
