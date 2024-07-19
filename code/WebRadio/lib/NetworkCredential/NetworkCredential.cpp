#include "NetworkCredential.h"

NetworkCredential::NetworkCredential() {
}



boolean NetworkCredential::load() {
    LittleFS.begin();
    File file = LittleFS.open(NETWORK_CREDENTIAL_FILE, "r");

    if (!file) {
        Serial.println("Failed to open file for reading");
        return false;
    }
    this->network_credential_list = new DynamicJsonDocument(1024);
    DeserializationError error = deserializeJson(*this->network_credential_list, file);

    if (error) {
        Serial.println("Failed to parse file");
        return false;
    }
    file.close();

    return true;
}


JsonObject NetworkCredential::getCredentialByIndex(int index) {
    JsonArray rootArray = this->network_credential_list->as<JsonArray>();
    return rootArray[index].as<JsonObject>();
}


int NetworkCredential::getCredentialIndexBySSID(String ssid) {
    JsonArray rootArray = this->network_credential_list->as<JsonArray>();
    for (int i = 0; i < rootArray.size(); i++) {
        JsonObject obj = rootArray[i].as<JsonObject>();
        if (obj["name"] == ssid) {
            return i;
        }
    }
    return -1;
}


