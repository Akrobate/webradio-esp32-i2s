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

BusinessState * business_state = new BusinessState();
WebRadioServer * server = new WebRadioServer();
WifiNetworking * wifi_networking = new WifiNetworking();
NetworkCredential * network_credential = new NetworkCredential();
DisplayScreen * display_screen = new DisplayScreen();
BMP180Probe * bmp_180_probe = new BMP180Probe();
DeviceSystem * device_system = new DeviceSystem();

#define LED 4

int loops = 0;

// U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

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

    display_screen->init();
    display_screen->injectBusinesState(business_state);
    display_screen->demoScreen();





    delay(2000);

    // Serial.println("--------------start-------------");
    // serializeJsonPretty(*network_credential->network_credential_list, Serial);
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

