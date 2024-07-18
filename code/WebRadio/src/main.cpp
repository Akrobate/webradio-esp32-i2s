#include <Arduino.h>
#include <WiFi.h>

#include <LittleFS.h>

#include <WebradioServer.h>


WebradioServer * server = new WebradioServer();



#define LED 4

const char * ssid     = "ESP32-Access-Point";
const char * password = "123456789";

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);

  delay(1000);

  WiFi.softAP(ssid);


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


}
