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
    this->stream_count = this->stream_list->size();

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


JsonObject StreamRepository::getStreamByIndex(int index) {
    JsonArray rootArray = this->stream_list->as<JsonArray>();
    return rootArray[index].as<JsonObject>();
}


void StreamRepository::addStream(String name, String host) {
    JsonArray rootArray = this->stream_list->as<JsonArray>();
    JsonObject obj = rootArray.createNestedObject();
    obj["name"] = name;
    obj["host"] = host;
    this->save();
}


void StreamRepository::updateStream(int index, String name, String host) {
    JsonArray rootArray = this->stream_list->as<JsonArray>();
    JsonObject obj = rootArray[index].to<JsonObject>();
    obj["name"] = name;
    obj["host"] = host;
    this->save();
}


void StreamRepository::removeStream(int index) {
    JsonArray rootArray = this->stream_list->as<JsonArray>();
    rootArray.remove(index);
    this->save();
}