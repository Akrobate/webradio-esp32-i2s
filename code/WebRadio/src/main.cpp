#include <Arduino.h>
#include <WiFi.h>
#include <LittleFS.h>
#include <WebRadioServer.h>
#include <WifiNetworking.h>
#include <NetworkCredential.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 ecranOLED(128, 64, &Wire, -1);

void showMemoryUsage();

WebRadioServer * server = new WebRadioServer();
WifiNetworking * wifi_networking = new WifiNetworking();
NetworkCredential * network_credential = new NetworkCredential();

#define LED 4

int loops = 0;


void setup() {

    pinMode(LED, OUTPUT);
    Serial.begin(115200);

    delay(1000);

    wifi_networking->startAP();

    server->init();
    server->begin();
    
    network_credential->load();
  

    server->injectWifiNetworking(wifi_networking);
    server->injectNetworkCredential(network_credential);


    Serial.println("--------------start-------------");
    serializeJsonPretty(*network_credential->network_credential_list, Serial);

    Serial.println("          ");
    Serial.println("--------------end-------------");


    Serial.println("-------------- ITEM -------------");

    Serial.println("Network 2");
    int index = network_credential->getCredentialIndexBySSID("Network 2");
    if (index == -1) {
        Serial.println("SSID not found");
    } else {
        JsonObject obj = network_credential->getCredentialByIndex(index);
        serializeJsonPretty(obj, Serial);

    }
    Serial.println("-------------- / ITEM -------------");







    Wire.begin();

    Serial.println(F("!!!!!!!!!!!!!!!!!!!!!!!!!!BEFORE BEGIN"));
    Serial.print("Wire.getClock()  ");
    Serial.println(Wire.getClock());
    Wire.setClock(10000);
    
    Serial.print("Wire.getClock()  ");
    Serial.println(Wire.getClock());

    if (!ecranOLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C    0x3D
        Serial.println(F("!!!!!!!!!!!!!!!!!!!!!!!!!!SSD1306 initialization failed"));
        for (;;);
    }
    Serial.println(F("!!!!!!!!!!!!!!!!!!!!!!!!!!After begin"));

    delay(2000);

    // ecranOLED.clearDisplay(); 
    // int valeurTemperature = 25;
    // ecranOLED.print("Temperature = ");
    // ecranOLED.print(valeurTemperature);
    // ecranOLED.print((char)247);
    // ecranOLED.println("C");
    ecranOLED.display();


    wifi_networking->scan();

    delay(2000);
    

}




void loop() {
    showMemoryUsage();

    // network_credential->load();

    // delay(1);
    // digitalWrite(LED, HIGH);
    // delay(100);
    // digitalWrite(LED, LOW);

    Serial.println("Scan start");
 
    // wifi_networking->scan();
    // DynamicJsonDocument * scan_list = wifi_networking->getAvailableNetworks();
    // serializeJsonPretty(*scan_list, Serial);
    // delete scan_list;

    // wifi_networking->scanDebug();

    // Wait a bit before scanning again.
    

    /*
    bool status = wifi_networking->connect("dqdqsd", "dqfsqfqsdf");
    if(status){
        Serial.println("Connected to wifi");
    } else {
        Serial.println("Not connected to wifi");
    }
    delay(5000);
    wifi_networking->disconnect();
    */
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