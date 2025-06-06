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
    this->wifi->softAP("ESP32-Access-Point");
}


void WifiNetworking::startAP(String ssid, String password) {
    this->wifi->softAP(ssid.c_str(), password.c_str());
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

    IPAddress ip = this->wifi->localIP();
    IPAddress gw = this->wifi->gatewayIP();
    IPAddress sn = this->wifi->subnetMask();
    IPAddress dns(8, 8, 8, 8);  // ou 1.1.1.1 (Cloudflare)
    this->wifi->config(ip, gw, sn, dns);

    this->business_state->setIsConnectedToWifi(true);
    return true;
}

void WifiNetworking::begin(String ssid, String password) {
    if (this->business_state->lock()) {
        this->business_state->setConnectedToSSID(ssid);
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



// Prototyping with memory mapping
// @todo
/*
String WifiNetworking::encryptionTypeToString2(wifi_auth_mode_t encryption_type) {
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
*/



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


void WifiNetworking::networkConnectionTask() {

    if (this->isConnected()) {
        int rssi = this->wifi->RSSI();
        if (business_state->lock()) {
            business_state->setWifiRSSI(rssi);
            business_state->unlock();
        }
        return;
    }

    if (business_state->lock()) {
        business_state->setIsConnectedToWifi(false);
        business_state->setIsConnectingToWifi(true);
        business_state->unlock();
    }

    JsonArray credential_list = this->network_credential_repository->network_credential_list->as<JsonArray>();
    for (JsonObject credential : credential_list) {
        String ssid = credential["ssid"];
        String password = credential["password"];

        if (this->isNetworkAvailable(ssid)) {
            Serial.print("Whould try to connect to ");
            Serial.print(ssid);
            this->begin(ssid, password);
            int tries_count = 0;
            while (!this->isConnected()) {
                vTaskDelay(pdMS_TO_TICKS(500));
                Serial.println("Trying to connect");
                if (tries_count > 20) {
                    Serial.println("MAX Count of connection attempts");
                    break;
                }
                tries_count++;
            }
        }

        IPAddress ip = this->wifi->localIP();
        IPAddress gw = this->wifi->gatewayIP();
        IPAddress sn = this->wifi->subnetMask();
        IPAddress dns(8, 8, 8, 8);  // ou 1.1.1.1 (Cloudflare)
        this->wifi->config(ip, gw, sn, dns);

        if (this->isConnected()) {
            if (business_state->lock()) {
                business_state->setIsConnectedToWifi(true);
                business_state->setLocalIP(this->getLocalIP());
                business_state->setIsConnectingToWifi(false);
                business_state->unlock();
            }
            break;
        }
    }

}


void WifiNetworking::init() {
    
    this->scan();
    
    xTaskCreate(
        [](void *arg) {
            WifiNetworking * wifi_networking = (WifiNetworking *)arg;
            while (1) {
                wifi_networking->networkConnectionTask();
                vTaskDelay(pdMS_TO_TICKS(10000));
            }
        },
        "Task NetworkConnection",
        2000,
        this,
        1,
        NULL
    );
}

void WifiNetworking::injectNetworkCredentialRepository(NetworkCredentialRepository * network_credential_repository) {
    this->network_credential_repository = network_credential_repository;
}