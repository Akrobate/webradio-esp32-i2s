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

  while(!LittleFS.begin()) {
    Logger::getInstance().log("WebRadioServer: LittleFS mount failed, retrying...");
    delay(100);
  }

  this->server = new AsyncWebServer(port);

  this->server->onNotFound(
    [](AsyncWebServerRequest *request){
      request->send(HTTP_CODE_NOT_FOUND, "text/html", "Not found");
    }
  );

  this->server->on(
    "/api/ping",
    HTTP_GET,
    [](AsyncWebServerRequest *request) {
      request->send(HTTP_CODE_OK, "text/html", "Ok");
    }
  );


  this->server->on(
    "/api/info",
    HTTP_GET,
    [&](AsyncWebServerRequest *request) {
      Logger::getInstance().log("/api/info GET request");
      String response;
      DynamicJsonDocument info(500);
      JsonObject response_object = info.to<JsonObject>();
      response_object["access_point_ssid"] = this->business_state->getAccessPointSSID();
      response_object["temperature"] = this->business_state->getTemperature();
      response_object["pressure"] = this->business_state->getPressure();
      response_object["total_free_bytes"] = this->business_state->getTotalFreeBytes();
      response_object["minimum_free_bytes"] = this->business_state->getMinimumFreeBytes();
      response_object["is_connected_to_wifi"] = this->business_state->getIsConnectedToWifi();
      response_object["is_connecting_to_wifi"] = this->business_state->getIsConnectingToWifi();
      response_object["wifi_rssi"] = this->business_state->getWifiRSSI();
      response_object["connected_to_ssid"] = this->business_state->getConnectedToSSID();
      response_object["local_ip"] = this->business_state->getLocalIP();
      response_object["date_time"] = this->business_state->getDateTime();
      response_object["date_time_configured"] = this->business_state->getDateTimeConfigured();
      serializeJson(info, response);
      request->send(HTTP_CODE_OK, "application/json", response);
    }
  );


  this->server->on(
    "/api/streams",
    HTTP_GET,
    [&](AsyncWebServerRequest *request) {
      String response;
      DynamicJsonDocument * stream_list = this->stream_repository->stream_list;
      serializeJson(*stream_list, response);
      request->send(HTTP_CODE_OK, "application/json", response);
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
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Bad index");
        return;
      }

      if (request->hasParam("name", true)) {
        name = request->getParam("name", true)->value();
      } else {
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Missing name");
        return;
      }

      if (request->hasParam("host", true)) {
        host = request->getParam("host", true)->value();
      } else {
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Missing host");
        return;
      }
      Serial.print("EDIT - index: ");
      Serial.println(index);
      Serial.print("name : ");
      Serial.println(name);
      Serial.print("host : ");
      Serial.println(host);

      this->stream_repository->updateStream(index, name, host);

      request->send(HTTP_CODE_CREATED, "text/html", "OK");
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
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Bad index");
        return;
      }

      Serial.print("DELETE - index: ");
      Serial.println(index);

      this->stream_repository->removeStream(index);

      request->send(HTTP_CODE_CREATED, "text/html", "OK");
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
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Bad index");
        return;
      }

      Serial.print("POSITION UP - index: ");
      Serial.println(index);

      // this->stream_repository->updateStream(index, name, host);

      request->send(HTTP_CODE_CREATED, "text/html", "OK");
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
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Bad index");
        return;
      }

      Serial.print("POSITION DOWN - index: ");
      Serial.println(index);

      // this->stream_repository->updateStream(index, name, host);

      request->send(HTTP_CODE_CREATED, "text/html", "OK");
    }
  );


  this->server->on(
    "^\\/api\\/streams\\/(-?[0-9]+)\\/play$",
    HTTP_POST,
    [&](AsyncWebServerRequest *request) {

      int index = -1;

      if (request->pathArg(0) != NULL) {
        String index_str = request->pathArg(0);
        index = index_str.toInt();
      } else {
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Bad index");
        return;
      }

      int total_streams = this->stream_repository->countStream();
      if (index < 0 || index >= total_streams) {
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Bad index");
        return;
      }

      Serial.print("Play ");
      Serial.println(index);

      this->business_state->setPlayingStream(index);
      request->send(HTTP_CODE_CREATED, "text/html", "OK");
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
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "/api/streams Missing name");
        return;
      }

      if (request->hasParam("host", true)) {
        host = request->getParam("host", true)->value();
      } else {
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Missing host");
        return;
      }

      Serial.print("name : ");
      Serial.println(name);
      Serial.print("host : ");
      Serial.println(host);
      this->stream_repository->addStream(name, host);

      request->send(HTTP_CODE_CREATED, "text/html", "OK");
    }
  );


  this->server->on(
    "/api/volume",
    HTTP_POST,
    [&](AsyncWebServerRequest *request) {

      int value = 0;

      if (request->hasParam("value", true)) {
        value = request->getParam("value", true)->value().toInt();
      } else {
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Missing value");
        return;
      }

      Serial.print("Volume ");
      Serial.println(value);

      this->business_state->setPlayingVolume(value);
      request->send(HTTP_CODE_CREATED, "text/html", "OK");
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
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Missing ntp_server_host");
        return;
      }

      if (request->hasParam("gmt_offset_sec", true)) {
        gmt_offset_sec = request->getParam("gmt_offset_sec", true)->value().toInt();
      } else {
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Missing gmt_offset_sec");
        return;
      }

      if (request->hasParam("daylight_offset_sec", true)) {
        daylight_offset_sec = request->getParam("daylight_offset_sec", true)->value().toInt();
      } else {
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Missing daylight_offset_sec");
        return;
      }

      Serial.print("ntp_server_host : ");
      Serial.println(ntp_server_host);
      Serial.print("gmt_offset_sec : ");
      Serial.println(gmt_offset_sec);
      Serial.print("daylight_offset_sec : ");
      Serial.println(daylight_offset_sec);

      this->configuration_repository->setNTPServerHost(ntp_server_host);
      this->configuration_repository->setGMTOffsetSec(gmt_offset_sec);
      this->configuration_repository->setDaylightOffsetSec(daylight_offset_sec);

      // Save data and update business state
      request->send(HTTP_CODE_CREATED, "text/html", "OK");
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

            request->send(HTTP_CODE_OK, "application/json", response);

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
      request->send(HTTP_CODE_OK, "application/json", response);
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
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Missing ssid");
        return;
      }

      if (request->hasParam("password", true)) {
        password = request->getParam("password", true)->value();
      } else {
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Missing password");
        return;
      }

      Serial.print("ADD ssid : ");
      Serial.println(ssid);
      Serial.print("password : ");
      Serial.println(password);

      this->network_credential->upsertCredential(ssid, password);
      request->send(HTTP_CODE_CREATED, "text/html", "OK");
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
        request->send(HTTP_CODE_BAD_REQUEST, "text/html", "Missing ssid");
        return;
      }

      Serial.print("Delete ssid : ");
      Serial.println(ssid);

      this->network_credential->removeCredentialBySSID(ssid);

      request->send(HTTP_CODE_OK, "text/html", "OK");
    }
  );

  this->server->serveStatic("/", LittleFS, "/webinterface").setDefaultFile("index.html");
  
  // Debug routes
  this->server->serveStatic("/api/streams.json", LittleFS, "/streams.json");

  // Start server
  this->begin();

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

void WebRadioServer::injectConfigurationRepository(ConfigurationRepository * configuration_repository) {
  this->configuration_repository = configuration_repository;
}
