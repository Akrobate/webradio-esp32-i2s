#include "BusinessState.h"

BusinessState::BusinessState() {
    this->temperature = 0;
    this->pressure = 0;
    this->access_point_ssid = "";
}

void BusinessState::setTemperature(float temperature) {
    this->temperature = temperature;
}

float BusinessState::getTemperature() {
    return this->temperature;
}


void BusinessState::setPressure(float pressure) {
    this->pressure = pressure;
}

float BusinessState::getPressure() {
    return this->pressure;
}


bool BusinessState::getIsConnectedToWifi() {
    return this->is_connected_to_wifi;
}

void BusinessState::setIsConnectedToWifi(bool is_connected_to_wifi) {
    this->is_connected_to_wifi = is_connected_to_wifi;
}


String BusinessState::getConnectedToSSID() {
    return this->connected_to_ssid;
}

void BusinessState::setConnectedToSSID(String connected_to_ssid) {
    this->connected_to_ssid = connected_to_ssid;
}

String BusinessState::getLocalIP() {
    return this->local_ip;
}

void BusinessState::setLocalIP(String local_ip) {
    this->local_ip = local_ip;
}

void BusinessState::setTotalFreeBytes(int total_free_bytes) {
    this->total_free_bytes = total_free_bytes;
}

int BusinessState::getTotalFreeBytes() {
    return this->total_free_bytes;
}

void BusinessState::setAccessPointSSID(String access_point_ssid) {
    this->access_point_ssid = access_point_ssid;
}

String BusinessState::getAccessPointSSID() {
    return this->access_point_ssid;
}