#include "WifiNetworking.h"

WifiNetworking::WifiNetworking() {
    this->wifi = &WiFi;
}


void WifiNetworking::startAP() {
    const char * ssid     = "ESP32-Access-Point";
    const char * password = "123456789";

    this->wifi->softAP(ssid);
}


bool WifiNetworking::connect(char * ssid, char * password) {
    this->wifi->begin(ssid, password);
    int timeout = 0;
    while (this->wifi->status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if (timeout > 20) {
            return false;
        }
        timeout++;
    }
    return true;
}


bool WifiNetworking::isConnected() {
    return this->wifi->status() == WL_CONNECTED;
}


void WifiNetworking::disconnect() {
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
            switch (this->wifi->encryptionType(i))
            {
            case WIFI_AUTH_OPEN:
                Serial.print("open");
                break;
            case WIFI_AUTH_WEP:
                Serial.print("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                Serial.print("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                Serial.print("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                Serial.print("WPA+WPA2");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                Serial.print("WPA2-EAP");
                break;
            case WIFI_AUTH_WPA3_PSK:
                Serial.print("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                Serial.print("WPA2+WPA3");
                break;
            case WIFI_AUTH_WAPI_PSK:
                Serial.print("WAPI");
                break;
            default:
                Serial.print("unknown");
            }
            Serial.println();
            delay(10);
        }
    }
    Serial.println("");
 
    this->wifi->scanDelete();
}


DynamicJsonDocument * WifiNetworking::scan() {
    int n = this->wifi->scanNetworks();
    DynamicJsonDocument * json = new DynamicJsonDocument(1024);
    JsonArray networks = json->to<JsonArray>();
   
    for (int i = 0; i < n; ++i) {

        JsonObject network = networks.createNestedObject();
        network["nr"] = i + 1;
        network["ssid"] = this->wifi->SSID(i);
        network["rssi"] = this->wifi->RSSI(i) * -1;
        network["channel"] = this->wifi->channel(i);
        
        switch (this->wifi->encryptionType(i)) {
            case WIFI_AUTH_OPEN:
                network["encryption"] = "open";
                break;
            case WIFI_AUTH_WEP:
                network["encryption"] = "WEP";
                break;
            case WIFI_AUTH_WPA_PSK:
                network["encryption"] = "WPA";
                break;
            case WIFI_AUTH_WPA2_PSK:
                network["encryption"] = "WPA2";
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                network["encryption"] = "WPA+WPA2";
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                network["encryption"] = "WPA2-EAP";
                break;
            case WIFI_AUTH_WPA3_PSK:
                network["encryption"] = "WPA3";
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                network["encryption"] = "WPA2+WPA3";
                break;
            case WIFI_AUTH_WAPI_PSK:
                network["encryption"] = "WAPI";
                break;
            default:
                network["encryption"] = "unknown";
        }
    }
    this->wifi->scanDelete();
    return json;
}

