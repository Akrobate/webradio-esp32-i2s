#include "WebRadioServer.h"

struct TaskTwoParameters {
  void * param_1;
  void * param_2;
};


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

      TaskTwoParameters * task_two_parameters = new TaskTwoParameters();
      task_two_parameters->param_1 = (void*)request;
      task_two_parameters->param_2 = (void*)this;

      xTaskCreate(
        [](void *arg){
          {
            TaskTwoParameters* params = (TaskTwoParameters*)arg;
            AsyncWebServerRequest * request = (AsyncWebServerRequest *)params->param_1;
            WebRadioServer * web_radio_server = (WebRadioServer *)params->param_2;

            request->client()->setRxTimeout(50);
            String response;
            web_radio_server->wifi_networking->scan();
            DynamicJsonDocument * available_networks = web_radio_server->wifi_networking->getAvailableNetworks();
            serializeJson(*available_networks, response);

            request->send(200, "application/json", response);

            delete params;
          }
          vTaskDelete(NULL);
        },
        "handler",
        5000,
        (void*)task_two_parameters,
        1,
        NULL
      );
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
