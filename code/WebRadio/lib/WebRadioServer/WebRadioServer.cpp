#include "WebRadioServer.h"

WebRadioServer::WebRadioServer() {
}

void WebRadioServer::init() {

  this->server = new AsyncWebServer(port);

  LittleFS.begin();
  

  this->server->onNotFound(
    [](AsyncWebServerRequest *request){
      request->send(404, "text/html", "Not found");
    }
  );

  this->server->on(
    "/api/ping",
    HTTP_GET,
    [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", "Ok");
    }
  );


  this->server->on(
    "/api/available-networks",
    HTTP_GET,
    [&](AsyncWebServerRequest *request) {
      // AsyncResponseStream *response = request->beginResponseStream("application/json");
      
      String response;
      // this->wifi_networking->scan();
      DynamicJsonDocument * available_networks = this->wifi_networking->getAvailableNetworks();
      serializeJson(*available_networks, response);
      
      Serial.println("Response ==== start");
      Serial.println(response);
      Serial.println("Response ==== end");
      // request->send(response);

      delay(5000);

      request->send(200, "application/json", response);
    }
  );

  this->server->serveStatic("/", LittleFS, "/webinterface").setDefaultFile("index.html");
  this->server->serveStatic("/api/streams.json", LittleFS, "/streams.json");

}


void WebRadioServer::begin() {
    this->server->begin();
}


void WebRadioServer::injectWifiNetworking(WifiNetworking * wifi_networking) {
    this->wifi_networking = wifi_networking;
}

void WebRadioServer::injectNetworkCredential(NetworkCredential * network_credential) {
    this->network_credential = network_credential;
}
