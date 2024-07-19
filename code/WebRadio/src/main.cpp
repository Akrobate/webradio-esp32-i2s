#include <Arduino.h>
#include <WiFi.h>
#include <LittleFS.h>
#include <WebRadioServer.h>
#include <WifiNetworking.h>
#include <NetworkCredential.h>


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
  


    Serial.println("--------------start-------------");
    serializeJsonPretty(*network_credential->network_credential_list, Serial);

    (*network_credential->network_credential_list).printTo(Serial);


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
}


void loop() {



    showMemoryUsage();

    network_credential->load();



    delay(1);
    digitalWrite(LED, HIGH);
    delay(1);
    digitalWrite(LED, LOW);

    Serial.println("Scan start");
 
    DynamicJsonDocument * scan_list = wifi_networking->scan();
    serializeJsonPretty(*scan_list, Serial);
    delete scan_list;

    // wifi_networking->scanDebug();


    // Wait a bit before scanning again.
    delay(1);

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
}




void showMemoryUsage() {
    multi_heap_info_t info;

    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT); // internal RAM, memory capable to store data or to create new task
    // info.total_free_bytes;   // total currently free in all non-continues blocks
    // info.minimum_free_bytes;  // minimum free ever
    // info.largest_free_block;   // largest continues block to allocate big array
    // 153

    Serial.println("========================================================");
    Serial.print("Total Free: ");
    Serial.println(info.total_free_bytes);

    Serial.print("Minimum free: ");
    Serial.println(info.minimum_free_bytes);

    Serial.print("Largest free block: ");
    Serial.println(info.largest_free_block);
    Serial.println("========================================================");
}