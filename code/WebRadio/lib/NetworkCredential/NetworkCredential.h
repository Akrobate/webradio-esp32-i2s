#ifndef NetworkCredential_h
#define NetworkCredential_h

#include "ArduinoJson.h"
#include <LittleFS.h>

/**
 * 
 * 
 * {
 *   ssid: "ssid",
 *   quality: 100,
 *   password: "password"
 * }
 */

#define NETWORK_CREDENTIAL_FILE "/network_credentials.json"

class NetworkCredential {

    public:
        NetworkCredential();

        DynamicJsonDocument * network_credential_list;

        boolean load();
        boolean save();

        String getPasswordBySSID(String ssid);
        JsonObject getCredentialByIndex(int index);
        int getCredentialIndexBySSID(String ssid);

};

#endif
