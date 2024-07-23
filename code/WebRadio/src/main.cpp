#include <Arduino.h>
#include <WiFi.h>
#include <LittleFS.h>

#include <WebRadioServer.h>
#include <WifiNetworking.h>
#include <NetworkCredential.h>
#include <BusinessState.h>
#include <DisplayScreen.h>
#include <BMP180Probe.h>
#include <DeviceSystem.h>


#include <Wire.h>

void showMemoryUsage();

BusinessState * business_state = new BusinessState();
WebRadioServer * server = new WebRadioServer();
WifiNetworking * wifi_networking = new WifiNetworking();
NetworkCredential * network_credential = new NetworkCredential();
DisplayScreen * display_screen = new DisplayScreen();
BMP180Probe * bmp_180_probe = new BMP180Probe();
DeviceSystem * device_system = new DeviceSystem();

#define LED 4

int loops = 0;


void setup() {

    Serial.begin(115200);
    delay(100);
    wifi_networking->startAP();
    wifi_networking->scan();

    server->init();
    server->begin();
    server->injectWifiNetworking(wifi_networking);
    server->injectNetworkCredential(network_credential);


    bmp_180_probe->init();
    bmp_180_probe->injectBusinesState(business_state);

    network_credential->load();

    // Serial.println("--------------start-------------");
    // serializeJsonPretty(*network_credential->network_credential_list, Serial);
    // Serial.println("--------------end-------------");    
}


void loop() {
    showMemoryUsage();

    loops++;
    Serial.print("loops " );
    Serial.println(loops);
    delay(5000);
}


void showMemoryUsage() {
    multi_heap_info_t info;

    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT); // internal RAM, memory capable to store data or to create new task

    Serial.println("========================================================");
    Serial.print("Total Free: ");
    Serial.println(info.total_free_bytes); // total currently free in all non-continues blocks

    Serial.print("Minimum free: ");
    Serial.println(info.minimum_free_bytes); // minimum free ever

    Serial.print("Largest free block: ");
    Serial.println(info.largest_free_block); // largest continues block to allocate big array
    Serial.println("========================================================");
}
