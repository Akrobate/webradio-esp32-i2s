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

    delete this->network_credential_list;
    this->network_credential_list = new DynamicJsonDocument(1024);
    DeserializationError error = deserializeJson(*this->network_credential_list, file);

    if (error) {
        Serial.println("Failed to parse file");
        return false;
    }
    file.close();

    return true;
}


bool NetworkCredential::save() {
    LittleFS.begin();
    File file = LittleFS.open(NETWORK_CREDENTIAL_FILE, "w");

    if (!file) {
        Serial.println("Failed to open file for writing");
        return false;
    }

    serializeJson(*this->network_credential_list, file);
    file.close();

    return true;
}


JsonObject NetworkCredential::getCredentialByIndex(int index) {
    JsonArray rootArray = this->network_credential_list->as<JsonArray>();
    return rootArray[index].as<JsonObject>();
}


void NetworkCredential::upsertCredential(String ssid, String password) {
    int index = this->getCredentialIndexBySSID(ssid);
    if (index == -1) {
        this->addCredential(ssid, password);
    } else {
        this->setCredentialByIndex(index, ssid, password);
    }
}


void NetworkCredential::addCredential(String ssid, String password) {
    JsonArray rootArray = this->network_credential_list->to<JsonArray>();
    JsonObject obj = rootArray.createNestedObject();
    obj["ssid"] = ssid;
    obj["password"] = password;
}


void NetworkCredential::setCredentialByIndex(int index, String ssid, String password) {
    JsonArray rootArray = this->network_credential_list->to<JsonArray>();
    JsonObject obj = rootArray[index].to<JsonObject>();
    obj["ssid"] = ssid;
    obj["password"] = password;
}


int NetworkCredential::getCredentialIndexBySSID(String ssid) {
    JsonArray rootArray = this->network_credential_list->as<JsonArray>();
    for (int i = 0; i < rootArray.size(); i++) {
        JsonObject obj = rootArray[i].as<JsonObject>();
        if (obj["ssid"] == ssid) {
            return i;
        }
    }
    return -1;
}


void NetworkCredential::removeCredentialByIndex(int index) {
    JsonArray rootArray = this->network_credential_list->to<JsonArray>();
    rootArray.remove(index);
}


void NetworkCredential::removeCredentialBySSID(String ssid) {
    int index = this->getCredentialIndexBySSID(ssid);
    if (index != -1) {
        this->removeCredentialByIndex(index);
    }
}


String NetworkCredential::getPasswordByIndex(int index) {
    JsonArray rootArray = this->network_credential_list->as<JsonArray>();
    JsonObject obj = rootArray[index].as<JsonObject>();
    return obj["password"];
}