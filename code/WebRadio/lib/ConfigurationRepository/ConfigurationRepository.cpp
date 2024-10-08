#include "ConfigurationRepository.h"

ConfigurationRepository::ConfigurationRepository(){}

void ConfigurationRepository::init() {
    if (!this->load()) {
        Serial.println("Failed to load configuration file");
    }
}

bool ConfigurationRepository::load() {
    LittleFS.begin();
    File file = LittleFS.open(CONFIGURATION_FILE, "r");

    if (!file) {
        Serial.println("Failed to open file for reading");
        return false;
    }

    DynamicJsonDocument * doc = new DynamicJsonDocument(1024);
    DeserializationError error = deserializeJson(*doc, file);
    if (error) {
        Serial.println("Failed to parse file");
        return false;
    }

    JsonObject root = doc->as<JsonObject>();
    this->gmt_offset_sec = root["gmt_offset_sec"].as<long>();
    this->daylight_offset_sec = root["daylight_offset_sec"].as<int>();
    this->ntp_server_host = root["ntp_server_host"].as<String>();

    file.close();
    delete doc;
    return true;
}

bool ConfigurationRepository::save() {
    LittleFS.begin();
    File file = LittleFS.open(CONFIGURATION_FILE, "w");

    if (!file) {
        Serial.println("Failed to open configuration file for writing");
        return false;
    }

    DynamicJsonDocument doc(1024);
    JsonObject rootObject = doc.to<JsonObject>();
    rootObject["gmt_offset_sec"] = this->gmt_offset_sec;
    rootObject["daylight_offset_sec"] = this->daylight_offset_sec;
    rootObject["ntp_server_host"] = this->ntp_server_host;

    serializeJson(doc, file);

    file.close();
    return true;
}

void ConfigurationRepository::injectBusinessState(BusinessState * business_state) {
    this->business_state = business_state;
}

void ConfigurationRepository::updateBusinessState() {
    if (this->business_state->lock()) {
        this->business_state->setGMTOffsetSec(this->gmt_offset_sec);
        this->business_state->setDaylightOffsetSec(this->daylight_offset_sec);
        this->business_state->setNTPServerHost(this->ntp_server_host);

        this->business_state->unlock();
    }
}