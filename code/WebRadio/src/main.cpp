#include <Arduino.h>
#include <WiFi.h>


#define LED 4

const char * ssid     = "ESP32-Access-Point";
const char * password = "123456789";

void setup() {
 
  pinMode(LED, OUTPUT);
  Serial.begin(115200);

  delay(1000);

  WiFi.softAP(ssid);

}


void loop() {
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);


}
