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

void deviceSystemTask(void *pvParameters);
void networkConnectionTask(void *pvParameters);
void audioTask(void *pvParameters);

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

#define LED 4
int loops = 0;

void setup() {
    //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

    business_state->setInitingDevice(true);
    Serial.begin(115200);

    display_screen->injectBusinesState(business_state);
    display_screen->injectStreamRepository(stream_repository);
    display_screen->init();

    // Why configuration repository do need busuness state?? 
    configuration_repository->injectBusinessState(business_state);
    configuration_repository->load();

    device_system->injectBusinesState(business_state);
    device_system->configureTimeTask();

    wifi_networking->injectBusinessState(business_state);
    wifi_networking->startAP();
    wifi_networking->scan();
    
    network_credential_repository->load();
    stream_repository->load();

    server->injectWifiNetworking(wifi_networking);
    server->injectNetworkCredentialRepository(network_credential_repository);
    server->injectStreamRepository(stream_repository);
    server->injectBusinessState(business_state);
    server->init();
    server->begin();
    
    bmp_180_probe->injectBusinesState(business_state);
    bmp_180_probe->init();

    xTaskCreate(deviceSystemTask, "Task deviceSystem", 2000, NULL, 1, NULL); // 2000 -
    xTaskCreate(networkConnectionTask, "Task NetworkConnection", 2000, NULL, 1, NULL); // 5000
    
    audio_process->injectBusinesState(business_state);
    audio_process->injectStreamRepository(stream_repository);
    audio_process->init();

    input_interface->init();


    business_state->setInitingDevice(false);
}


void loop() {
    loops++;
    if (loops % 1000 == 0) {
        Serial.print("loops " );
        Serial.println(loops);

        Serial.print("analog 1 value:  " );
        Serial.println(input_interface->analog_1_value);
    }
    input_interface->update();
}


// @todo: move to a task in the class
void deviceSystemTask(void *pvParameters) {
    (void) pvParameters;
    while (1) {
        device_system->update();
        device_system->updateBusinessState();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


void networkConnectionTask(void *pvParameters) {
    (void) pvParameters;

    while (1) {
        if (!wifi_networking->isConnected()) {

            if (business_state->lock()) {
                business_state->setIsConnectedToWifi(false);
                business_state->setIsConnectingToWifi(true);
                business_state->unlock();
            }

            JsonArray credential_list = network_credential_repository->network_credential_list->as<JsonArray>();
            for (JsonObject credential : credential_list) {
                String ssid = credential["ssid"];
                String password = credential["password"];

                if (wifi_networking->isNetworkAvailable(ssid)) {
                    Serial.print("Whould try to connect to ");
                    Serial.print(ssid);
                    wifi_networking->begin(ssid, password);
                    int tries_count = 0;
                    while (!wifi_networking->isConnected()) {
                        vTaskDelay(pdMS_TO_TICKS(500));
                        if (tries_count > 10) {
                            break;
                        }
                        tries_count++;
                    }
                }

                if (wifi_networking->isConnected()) {
                    if (business_state->lock()) {
                        business_state->setIsConnectedToWifi(true);
                        business_state->setLocalIP(wifi_networking->getLocalIP());
                        business_state->setIsConnectingToWifi(false);
                        business_state->unlock();
                    }
                    break;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

