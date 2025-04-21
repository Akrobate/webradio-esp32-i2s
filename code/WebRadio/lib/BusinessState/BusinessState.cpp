#include "BusinessState.h"

BusinessState::BusinessState() {
    this->business_state_mutex = xSemaphoreCreateMutex();
    if (this->business_state_mutex == NULL) {
        Serial.println("Mutex creation failed");
        while (1);
    }
}

boolean BusinessState::lock() {
    return (xSemaphoreTake(this->business_state_mutex, portMAX_DELAY) == pdTRUE);
}

void BusinessState::unlock() {
    xSemaphoreGive(this->business_state_mutex);
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

void BusinessState::setMinimumFreeBytes(int minimum_free_bytes) {
    this->minimum_free_bytes = minimum_free_bytes;
}

int BusinessState::getMinimumFreeBytes() {
    return this->minimum_free_bytes;
}

void BusinessState::setAccessPointSSID(String access_point_ssid) {
    this->access_point_ssid = access_point_ssid;
}

String BusinessState::getAccessPointSSID() {
    return this->access_point_ssid;
}


bool BusinessState::getIsConnectingToWifi() {
    return this->is_connecting_to_wifi;
}

void BusinessState::setIsConnectingToWifi(bool is_connecting_to_wifi) {
    this->is_connecting_to_wifi = is_connecting_to_wifi;
}


void BusinessState::setGMTOffsetSec(long gmt_offset_sec) {
    this->gmt_offset_sec = gmt_offset_sec;
}

long BusinessState::getGMTOffsetSec() {
    return this->gmt_offset_sec;
}

void BusinessState::setDaylightOffsetSec(int daylight_offset_sec) {
    this->daylight_offset_sec = daylight_offset_sec;
}

int BusinessState::getDaylightOffsetSec() {
    return this->daylight_offset_sec;
}

void BusinessState::setNTPServerHost(String ntp_server_host) {
    this->ntp_server_host = ntp_server_host;
}

String BusinessState::getNTPServerHost() {
    return this->ntp_server_host;
}

const char * BusinessState::getNTPServerHostChar() {
    return this->ntp_server_host.c_str();
}

void BusinessState::setDateTime(String date_time) {
    this->date_time = date_time;
}

String BusinessState::getDateTime() {
    return this->date_time;
}

void BusinessState::setDateTimeConfigured(bool date_time_configured) {
    this->date_time_configured = date_time_configured;
}

bool BusinessState::getDateTimeConfigured() {
    return this->date_time_configured;
}


// Streams
void BusinessState::setPlayingStream(int playing_stream) {
    this->playing_stream = playing_stream;
    this->setStreamChangedAtMillis(millis());
}

int BusinessState::getPlayingStream() {
    return this->playing_stream;
}

void BusinessState::setStreamChangedAtMillis(unsigned long stream_changed_at_millis) {
    this->stream_changed_at_millis = stream_changed_at_millis;
}

unsigned long BusinessState::getStreamChangedAtMillis() {
    return this->stream_changed_at_millis;
}



void BusinessState::setPlayingVolume(int playing_volume) {
    this->playing_volume = playing_volume;
    this->setVolumeChangedAtMillis(millis());
}

int BusinessState::getPlayingVolume() {
    return this->playing_volume;
}

void BusinessState::setFormatedTime(String formated_time) {
    this->formated_time = formated_time;
}

String BusinessState::getFormatedTime() {
    return this->formated_time;
}

void BusinessState::setFormatedDate(String formated_date) {
    this->formated_date = formated_date;
}

String BusinessState::getFormatedDate() {
    return this->formated_date;
}

void BusinessState::setVolumeChangedAtMillis(unsigned long volume_changed_at_millis) {
    this->volume_changed_at_millis = volume_changed_at_millis;
}

unsigned long BusinessState::getVolumeChangedAtMillis() {
    return this->volume_changed_at_millis;
}

void BusinessState::setInitingDevice(bool initing_device) {
    this->initing_device = initing_device;
}

bool BusinessState::getInitingDevice() {
    return this->initing_device;
}

void BusinessState::setDnsHost(String dns_host) {
    this->dns_host = dns_host;
}

String BusinessState::getDnsHost() {
    return this->dns_host;
}
