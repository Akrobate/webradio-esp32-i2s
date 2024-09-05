#include "StreamRepository.h"

StreamRepository::StreamRepository() {
}


boolean StreamRepository::load() {
    LittleFS.begin();
    File file = LittleFS.open(STREAMS_FILE, "r");

    if (!file) {
        Serial.println("Failed to open file for reading");
        return false;
    }

    delete this->stream_list;
    this->stream_list = new DynamicJsonDocument(1024);
    DeserializationError error = deserializeJson(*this->stream_list, file);

    if (error) {
        Serial.println("Failed to parse file");
        return false;
    }
    file.close();

    return true;
}



bool StreamRepository::save() {
    LittleFS.begin();
    File file = LittleFS.open(STREAMS_FILE, "w");

    if (!file) {
        Serial.println("Failed to open file for writing");
        return false;
    }

    serializeJson(*this->stream_list, file);
    file.close();

    return true;
}


JsonObject StreamRepository::getCredentialByIndex(int index) {
    JsonArray rootArray = this->stream_list->as<JsonArray>();
    return rootArray[index].as<JsonObject>();
}


int StreamRepository::getCredentialIndexBySSID(String ssid) {
    JsonArray rootArray = this->stream_list->as<JsonArray>();
    for (int i = 0; i < rootArray.size(); i++) {
        JsonObject obj = rootArray[i].as<JsonObject>();
        if (obj["ssid"] == ssid) {
            return i;
        }
    }
    return -1;
}


void StreamRepository::addStream(String name, String host) {
    JsonArray rootArray = this->stream_list->as<JsonArray>();
    JsonObject obj = rootArray.createNestedObject();
    obj["name"] = name;
    obj["host"] = host;
    this->save();
}
