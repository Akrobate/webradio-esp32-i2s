#include "WebRadioServer.h"

WebRadioServer::WebRadioServer() {
}

void WebRadioServer::init() {

  this->server = new AsyncWebServer(port);

  LittleFS.begin();
  this->server->serveStatic("/", LittleFS, "/webinterface").setDefaultFile("index.html");
  this->server->serveStatic("/api/streams.json", LittleFS, "/streams.json");

  this->server->onNotFound(
    [](AsyncWebServerRequest *request){
      request->send(404, "text/html", "Not found");
    }
  );

  this->server->on(
    "/api/test",
    HTTP_GET,
    [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", "Ok");
    }
  );

}



void WebRadioServer::begin() {
    this->server->begin();
}