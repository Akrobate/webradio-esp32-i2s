#include "WifiNetworking.h"

WifiNetworking::WifiNetworking() {
    this->wifi = &WiFi;
    this->scanningNetworksMutex = xSemaphoreCreateMutex();
    if (this->scanningNetworksMutex == NULL) {
        Serial.println("scanningNetworksMutex creation failed");
        while (1);
    }
}


void WifiNetworking::startAP() {
    const char * ssid     = "ESP32-Access-Point";
    const char * password = "123456789";

    this->wifi->softAP(ssid);
}


bool WifiNetworking::connect(String ssid, String password) {
    this->business_state->setConnectedToSSID(ssid);
    this->wifi->begin(ssid, password);
    int timeout = 0;
    Serial.print("Connecting to ");
    Serial.print(ssid);
    while (this->wifi->status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if (timeout > 10) {
            return false;
        }
        timeout++;
    }
    this->business_state->setIsConnectedToWifi(true);
    return true;
}

void WifiNetworking::begin(String ssid, String password) {
    if (this->business_state->lock()) {
        this->business_state->setConnectedToSSID(ssid);
        this->business_state->setIsConnectingToWifi(true);
        this->business_state->unlock();
    }
    this->wifi->begin(ssid, password);
}


bool WifiNetworking::isConnected() {
    return this->wifi->status() == WL_CONNECTED;
}


void WifiNetworking::disconnect() {
    if (this->business_state->lock()) {
        this->business_state->setIsConnectedToWifi(false);
        this->business_state->unlock();
    }
    this->wifi->disconnect();
}


void WifiNetworking::scanDebug() {
    int n = this->wifi->scanNetworks();
    Serial.println("Scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < n; ++i) {
            Serial.printf("%2d",i + 1);
            Serial.print(" | ");
            Serial.printf("%-32.32s", this->wifi->SSID(i).c_str());
            Serial.print(" | ");
            Serial.printf("%4d", this->wifi->RSSI(i) * -1);
            Serial.print(" | ");
            Serial.printf("%2d", this->wifi->channel(i));
            Serial.print(" | ");
            Serial.print(this->encryptionTypeToString(this->wifi->encryptionType(i)));
            Serial.println();
            delay(10);
        }
    }
    Serial.println("");
    this->wifi->scanDelete();
}


void WifiNetworking::scan() {
    if (xSemaphoreTake(this->scanningNetworksMutex, portMAX_DELAY) == pdTRUE) {
        int n = this->wifi->scanNetworks();

        delete this->available_networks;
        this->available_networks = new DynamicJsonDocument(1024);
        JsonArray networks = this->available_networks->to<JsonArray>();
    
        for (int i = 0; i < n; ++i) {
            JsonObject network = networks.createNestedObject();
            network["nr"] = i + 1;
            network["ssid"] = this->wifi->SSID(i);
            network["rssi"] = this->wifi->RSSI(i) * -1;
            network["channel"] = this->wifi->channel(i);
            network["encryption"] = this->encryptionTypeToString(this->wifi->encryptionType(i));
        }
        this->wifi->scanDelete();
        xSemaphoreGive(this->scanningNetworksMutex);
    }
}


DynamicJsonDocument * WifiNetworking::getAvailableNetworks() {
    if (xSemaphoreTake(this->scanningNetworksMutex, portMAX_DELAY) == pdTRUE) {
        xSemaphoreGive(this->scanningNetworksMutex);
        return this->available_networks;
    }
    return this->available_networks;
}


void WifiNetworking::injectBusinessState(BusinessState * business_state) {
    this->business_state = business_state;
}


String WifiNetworking::getLocalIP() {
    return this->wifi->localIP().toString();
}


String WifiNetworking::encryptionTypeToString(wifi_auth_mode_t encryption_type) {
    String encryption_type_string = "";

    switch (encryption_type) {
        case WIFI_AUTH_OPEN:
            encryption_type_string = "open";
            break;
        case WIFI_AUTH_WEP:
            encryption_type_string = "WEP";
            break;
        case WIFI_AUTH_WPA_PSK:
            encryption_type_string = "WPA";
            break;
        case WIFI_AUTH_WPA2_PSK:
            encryption_type_string = "WPA2";
            break;
        case WIFI_AUTH_WPA_WPA2_PSK:
            encryption_type_string = "WPA+WPA2";
            break;
        case WIFI_AUTH_WPA2_ENTERPRISE:
            encryption_type_string = "WPA2-EAP";
            break;
        case WIFI_AUTH_WPA3_PSK:
            encryption_type_string = "WPA3";
            break;
        case WIFI_AUTH_WPA2_WPA3_PSK:
            encryption_type_string = "WPA2+WPA3";
            break;
        case WIFI_AUTH_WAPI_PSK:
            encryption_type_string = "WAPI";
            break;
        default:
            encryption_type_string = "unknown";
    }
    return encryption_type_string;
}

bool WifiNetworking::isNetworkAvailable(String ssid) {
    bool is_available = false;
    JsonArray available_networks_list = this->getAvailableNetworks()->as<JsonArray>();
    for (JsonObject available_network : available_networks_list) {
        if (ssid == available_network["ssid"]) {
            is_available = true;
            break;
        }
    }
    return is_available;
}