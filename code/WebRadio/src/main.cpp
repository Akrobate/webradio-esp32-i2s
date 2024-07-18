#include <Arduino.h>
#include <WiFi.h>
#include <LittleFS.h>
#include <WebRadioServer.h>
#include <WifiNetworking.h>

WebRadioServer * server = new WebRadioServer();
WifiNetworking * wifi_networking = new WifiNetworking();

#define LED 4

void setup() {

    pinMode(LED, OUTPUT);
    Serial.begin(115200);

    delay(1000);

    wifi_networking->startAP();

    server->init();
    server->begin();
  
  /*
  LittleFS.begin();

  // Print files in data LittleFS
  File dir = LittleFS.open("/");

  if(!dir){
      Serial.println("- failed to open directory");
      return;
  }
  if(!dir.isDirectory()){
      Serial.println(" - not a directory");
      return;
  }

  File file = dir.openNextFile();

  while (file) {
      if(file.isDirectory()){
          Serial.print("  DIR : ");
          Serial.println(file.name());
      } else {
          Serial.print("  FILE: ");
          Serial.print(file.name());
          Serial.print("\tSIZE: ");
          Serial.println(file.size());
      }
      file = dir.openNextFile();
  }

  */
}


void loop() {

    delay(500);
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);

    Serial.println("Scan start");
 
    wifi_networking->scan();

    // Wait a bit before scanning again.
    delay(5000);

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
}
