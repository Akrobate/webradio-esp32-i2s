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
#include <AudioProcess.h>
#include <InputInterface.h>
#include <ConfigurationRepository.h>

#include "soc/soc.h" //disable brownour problems
#include "soc/rtc_cntl_reg.h" //disable brownour problems

BusinessState * business_state = new BusinessState();
WebRadioServer * server = new WebRadioServer();
WifiNetworking * wifi_networking = new WifiNetworking();
NetworkCredentialRepository * network_credential_repository = new NetworkCredentialRepository();
StreamRepository * stream_repository = new StreamRepository();
DisplayScreen * display_screen = new DisplayScreen();
BMP180Probe * bmp_180_probe = new BMP180Probe();
DeviceSystem * device_system = new DeviceSystem();
ConfigurationRepository * configuration_repository = new ConfigurationRepository();
AudioProcess * audio_process = new AudioProcess();
InputInterface * input_interface = new InputInterface();

int loops = 0;

void setup() {
    //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    business_state->setInitingDevice(true);
    Serial.begin(115200);

    display_screen->injectBusinesState(business_state);
    display_screen->injectStreamRepository(stream_repository);
    display_screen->init();

    configuration_repository->injectBusinessState(business_state);
    configuration_repository->load();

    device_system->injectBusinesState(business_state);
    device_system->configureTimeTask();
    device_system->init();

    network_credential_repository->load();
    stream_repository->load();

    wifi_networking->injectNetworkCredentialRepository(network_credential_repository);
    wifi_networking->injectBusinessState(business_state);
    wifi_networking->startAP();
    wifi_networking->scan();
    wifi_networking->init();
    
    server->injectWifiNetworking(wifi_networking);
    server->injectNetworkCredentialRepository(network_credential_repository);
    server->injectStreamRepository(stream_repository);
    server->injectBusinessState(business_state);
    server->init();
    server->begin();
    
    bmp_180_probe->injectBusinesState(business_state);
    bmp_180_probe->init();

    audio_process->injectBusinesState(business_state);
    audio_process->injectStreamRepository(stream_repository);
    audio_process->init();

    input_interface->injectBusinesState(business_state);
    input_interface->init();

    business_state->setInitingDevice(false);
}


void loop() {
    loops++;
    if (loops % 10000 == 0) {
        Serial.println("loops " + String(loops));
    }
}
