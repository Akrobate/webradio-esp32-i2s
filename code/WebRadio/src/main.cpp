#include <Arduino.h>
#include <WiFi.h>
#include <LittleFS.h>

#include <WebRadioServer.h>
#include <WifiNetworking.h>
#include <NetworkCredentialRepository.h>
#include <StreamRepository.h>
#include <BusinessState.h>
#include <DisplayScreen.h>
#include <BMP180Probe.h>
#include <DeviceSystem.h>

BusinessState * business_state = new BusinessState();
WebRadioServer * server = new WebRadioServer();
WifiNetworking * wifi_networking = new WifiNetworking();
NetworkCredentialRepository * network_credential_repository = new NetworkCredentialRepository();
StreamRepository * stream_repository = new StreamRepository();
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
    wifi_networking->injectBusinessState(business_state);
    
    network_credential_repository->load();
    stream_repository->load();

    server->init();
    server->begin();
    server->injectWifiNetworking(wifi_networking);
    server->injectNetworkCredentialRepository(network_credential_repository);
    server->injectStreamRepository(stream_repository);
    server->injectBusinessState(business_state);
    

    bmp_180_probe->init();
    bmp_180_probe->injectBusinesState(business_state);

    display_screen->init();
    display_screen->injectBusinesState(business_state);
    display_screen->demoScreen();

    delay(1000);

    // Serial.println("--------------start-------------");
    // serializeJsonPretty(*network_credential_repository->network_credential_repository_list, Serial);
    // Serial.println("--------------end-------------");    
}


void loop() {
    loops++;

    if (loops % 100 == 0) {
        Serial.print("loops " );
        Serial.println(loops);
    }
    
    if (loops % 100 == 0) {
        bmp_180_probe->updateBusinessState();
        // display_screen->infoScreen();
        display_screen->temperatureScreen();
    }

    delay(10);
}
