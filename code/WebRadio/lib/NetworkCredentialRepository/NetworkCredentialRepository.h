#ifndef NetworkCredentialRepository_h
#define NetworkCredentialRepository_h

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

class NetworkCredentialRepository {

    public:
        NetworkCredentialRepository();

        DynamicJsonDocument * network_credential_list = nullptr;

        boolean load();
        boolean save();

        String getPasswordBySSID(String ssid);
        JsonObject getCredentialByIndex(int index);
        int getCredentialIndexBySSID(String ssid);
        void setCredentialByIndex(int index, String ssid, String password);

        void addCredential(String ssid, String password);
        void upsertCredential(String ssid, String password);
        
        void removeCredentialByIndex(int index);
        void removeCredentialBySSID(String ssid);
        String getPasswordByIndex(int index);

};

#endif
