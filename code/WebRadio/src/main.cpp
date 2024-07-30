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

#include "soc/soc.h" //disable brownour problems
#include "soc/rtc_cntl_reg.h" //disable brownour problems

#include "time.h"


void temperatureTask(void *pvParameters);
void deviceSystemTask(void *pvParameters);
void networkConnectionTask(void *pvParameters);


BusinessState * business_state = new BusinessState();
WebRadioServer * server = new WebRadioServer();
WifiNetworking * wifi_networking = new WifiNetworking();
NetworkCredentialRepository * network_credential_repository = new NetworkCredentialRepository();
StreamRepository * stream_repository = new StreamRepository();
DisplayScreen * display_screen = new DisplayScreen();
BMP180Probe * bmp_180_probe = new BMP180Probe();
DeviceSystem * device_system = new DeviceSystem();


// ntp
// const char* ntpServer = "pool.ntp.org";
// const long  gmtOffset_sec = 3600 * 1;
// const int   daylightOffset_sec = 3600 * 1;


#define LED 4

int loops = 0;
SemaphoreHandle_t xMutex;

void setup() {
    //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

    Serial.begin(115200);
    delay(100);

    device_system->injectBusinesState(business_state);
    device_system->configureTimeTask();

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

    xMutex = xSemaphoreCreateMutex();
    if (xMutex == NULL) {
        Serial.println("Mutex creation failed");
        while (1);
    }

    xTaskCreate(temperatureTask, "Task Temperature", 2000, NULL, 1, NULL);
    xTaskCreate(deviceSystemTask, "Task deviceSystem", 2000, NULL, 1, NULL);
    xTaskCreate(networkConnectionTask, "Task NetworkConnection", 5000, NULL, 1, NULL);
    
}


void loop() {
    loops++;

    if (loops % 100 == 0) {
        Serial.print("loops " );
        Serial.println(loops);
    }
    
    if (loops % 100 == 0) {
        display_screen->temperatureScreen();

        /*
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("Failed to obtain time");
        } else {
            Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
        }
        */
    }

    delay(10);
}


void temperatureTask(void *pvParameters) {
    (void) pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xDelay = pdMS_TO_TICKS(5000); // 5 s

    while (1) {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
            bmp_180_probe->updateBusinessState();
            xSemaphoreGive(xMutex);
        }
        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}


void deviceSystemTask(void *pvParameters) {
    (void) pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xDelay = pdMS_TO_TICKS(1000); // 1 s

    while (1) {
        device_system->update();
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
            device_system->updateBusinessState();
            xSemaphoreGive(xMutex);
        }
        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}



void networkConnectionTask(void *pvParameters) {
    (void) pvParameters;

    while (1) {
        if (!wifi_networking->isConnected() && xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
            business_state->setIsConnectedToWifi(false);
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
                    business_state->setIsConnectedToWifi(true);
                    business_state->setIsConnectingToWifi(false);
                    business_state->setLocalIP(wifi_networking->getLocalIP());
                    


                    // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);



                    break;
                }
            }

            xSemaphoreGive(xMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

