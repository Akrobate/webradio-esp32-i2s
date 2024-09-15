#include "WebRadioServer.h"

struct TaskTwoParameters {
  void * param_1;
  void * param_2;
};


WebRadioServer::WebRadioServer() {
}


void WebRadioServer::begin() {
    this->server->begin();
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
    "/api/info",
    HTTP_GET,
    [&](AsyncWebServerRequest *request) {
      String response;
      DynamicJsonDocument info(500);

      JsonObject obj = info.to<JsonObject>();
      obj["access_point_ssid"] = this->business_state->getAccessPointSSID();
      obj["temperature"] = this->business_state->getTemperature();
      obj["pressure"] = this->business_state->getPressure();
      obj["total_free_bytes"] = this->business_state->getTotalFreeBytes();
      obj["minimum_free_bytes"] = this->business_state->getMinimumFreeBytes();
      obj["is_connected_to_wifi"] = this->business_state->getIsConnectedToWifi();
      obj["is_connecting_to_wifi"] = this->business_state->getIsConnectingToWifi();
      obj["connected_to_ssid"] = this->business_state->getConnectedToSSID();
      obj["local_ip"] = this->business_state->getLocalIP();
      obj["date_time"] = this->business_state->getDateTime();
      obj["date_time_configured"] = this->business_state->getDateTimeConfigured();

      serializeJson(info, response);
      request->send(200, "application/json", response);
    }
  );


  this->server->on(
    "/api/streams",
    HTTP_GET,
    [&](AsyncWebServerRequest *request) {
      String response;
      DynamicJsonDocument * stream_list = this->stream_repository->stream_list;
      serializeJson(*stream_list, response);
      request->send(200, "application/json", response);
    }
  );


  this->server->on(
    "/api/streams",
    HTTP_POST,
    [&](AsyncWebServerRequest *request) {

      String name = "";
      String host = "";

      if (request->hasParam("name", true)) {
        name = request->getParam("name", true)->value();
      } else {
        request->send(400, "text/html", "Missing name");
        return;
      }

      if (request->hasParam("host", true)) {
        host = request->getParam("host", true)->value();
      } else {
        request->send(400, "text/html", "Missing host");
        return;
      }

      Serial.print("name : ");
      Serial.println(name);
      Serial.print("host : ");
      Serial.println(host);
      this->stream_repository->addStream(name, host);

      request->send(201, "text/html", "OK");
    }
  );


  this->server->on(
    "^\\/api\\/streams\\/([0-9]+)$",
    HTTP_PATCH,
    [&](AsyncWebServerRequest *request) {

      int index = -1;
      String name = "";
      String host = "";

      if (request->pathArg(0) != NULL) {
        String index_str = request->pathArg(0);
        index = index_str.toInt();
      } else {
        request->send(400, "text/html", "Bad index");
      }

      if (request->hasParam("name", true)) {
        name = request->getParam("name", true)->value();
      } else {
        request->send(400, "text/html", "Missing name");
        return;
      }

      if (request->hasParam("host", true)) {
        host = request->getParam("host", true)->value();
      } else {
        request->send(400, "text/html", "Missing host");
        return;
      }
      Serial.print("EDIT - index: ");
      Serial.println(index);
      Serial.print("name : ");
      Serial.println(name);
      Serial.print("host : ");
      Serial.println(host);

      this->stream_repository->updateStream(index, name, host);

      request->send(201, "text/html", "OK");
    }
  );


  this->server->on(
    "^\\/api\\/streams\\/([0-9]+)$",
    HTTP_DELETE,
    [&](AsyncWebServerRequest *request) {

      int index = -1;

      if (request->pathArg(0) != NULL) {
        String index_str = request->pathArg(0);
        index = index_str.toInt();
      } else {
        request->send(400, "text/html", "Bad index");
      }

      Serial.print("DELETE - index: ");
      Serial.println(index);

      this->stream_repository->removeStream(index);

      request->send(201, "text/html", "OK");
    }
  );




  this->server->on(
    "^\\/api\\/streams\\/([0-9]+)\\/position-up$",
    HTTP_POST,
    [&](AsyncWebServerRequest *request) {

      int index = -1;

      if (request->pathArg(0) != NULL) {
        String index_str = request->pathArg(0);
        index = index_str.toInt();
      } else {
        request->send(400, "text/html", "Bad index");
      }

      Serial.print("POSITION UP - index: ");
      Serial.println(index);

      // this->stream_repository->updateStream(index, name, host);

      request->send(201, "text/html", "OK");
    }
  );


  this->server->on(
    "^\\/api\\/streams\\/([0-9]+)\\/position-down$",
    HTTP_POST,
    [&](AsyncWebServerRequest *request) {

      int index = -1;

      if (request->pathArg(0) != NULL) {
        String index_str = request->pathArg(0);
        index = index_str.toInt();
      } else {
        request->send(400, "text/html", "Bad index");
      }

      Serial.print("POSITION DOWN - index: ");
      Serial.println(index);

      // this->stream_repository->updateStream(index, name, host);

      request->send(201, "text/html", "OK");
    }
  );

  this->server->on(
    "/api/configurations",
    HTTP_POST,
    [&](AsyncWebServerRequest *request) {

      String ntp_server_host = "";
      long gmt_offset_sec = 0;
      int daylight_offset_sec = 0;

      if (request->hasParam("ntp_server_host", true)) {
        ntp_server_host = request->getParam("ntp_server_host", true)->value();
      } else {
        request->send(400, "text/html", "Missing ntp_server_host");
        return;
      }

      if (request->hasParam("gmt_offset_sec", true)) {
        gmt_offset_sec = request->getParam("gmt_offset_sec", true)->value().toInt();
      } else {
        request->send(400, "text/html", "Missing gmt_offset_sec");
        return;
      }

      if (request->hasParam("daylight_offset_sec", true)) {
        daylight_offset_sec = request->getParam("daylight_offset_sec", true)->value().toInt();
      } else {
        request->send(400, "text/html", "Missing daylight_offset_sec");
        return;
      }

      Serial.print("ntp_server_host : ");
      Serial.println(ntp_server_host);
      Serial.print("gmt_offset_sec : ");
      Serial.println(gmt_offset_sec);
      Serial.print("daylight_offset_sec : ");
      Serial.println(daylight_offset_sec);

      // this->network_credential->upsertCredential(ssid, password);

      request->send(201, "text/html", "OK");
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


  this->server->on(
    "/api/credentials",
    HTTP_GET,
    [&](AsyncWebServerRequest *request) {
      String response;
      DynamicJsonDocument * network_credential_list = this->network_credential->network_credential_list;
      serializeJson(*network_credential_list, response);
      request->send(200, "application/json", response);
    }
  );


  this->server->on(
    "/api/credentials",
    HTTP_POST,
    [&](AsyncWebServerRequest *request) {

      String ssid = "";
      String password = "";
      if (request->hasParam("ssid", true)) {
        ssid = request->getParam("ssid", true)->value();
      } else {
        request->send(400, "text/html", "Missing ssid");
        return;
      }

      if (request->hasParam("password", true)) {
        password = request->getParam("password", true)->value();
      } else {
        request->send(400, "text/html", "Missing password");
        return;
      }

      Serial.print("ADD ssid : ");
      Serial.println(ssid);
      Serial.print("password : ");
      Serial.println(password);

      this->network_credential->upsertCredential(ssid, password);
      request->send(201, "text/html", "OK");
    }
  );


  this->server->on(
    "/api/credentials",
    HTTP_DELETE,
    [&](AsyncWebServerRequest *request) {

      String ssid = "";
      if (request->hasParam("ssid", true)) {
        ssid = request->getParam("ssid", true)->value();
      } else {
        request->send(400, "text/html", "Missing ssid");
        return;
      }

      Serial.print("Delete ssid : ");
      Serial.println(ssid);

      this->network_credential->removeCredentialBySSID(ssid);

      request->send(200, "text/html", "OK");
    }
  );


  this->server->serveStatic("/", LittleFS, "/webinterface").setDefaultFile("index.html");
  
  // Debug routes
  this->server->serveStatic("/api/streams.json", LittleFS, "/streams.json");
}

void WebRadioServer::injectWifiNetworking(WifiNetworking * wifi_networking) {
    this->wifi_networking = wifi_networking;
}

void WebRadioServer::injectNetworkCredentialRepository(NetworkCredentialRepository * network_credential) {
    this->network_credential = network_credential;
}

void WebRadioServer::injectStreamRepository(StreamRepository * stream_repository) {
    this->stream_repository = stream_repository;
}

void WebRadioServer::injectBusinessState(BusinessState * business_state) {
    this->business_state = business_state;
}