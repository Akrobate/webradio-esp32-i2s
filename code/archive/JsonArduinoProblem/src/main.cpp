#include <Arduino.h>
#include <WiFi.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

void showMemoryUsage(int iteration);

DynamicJsonDocument * list_json_doc;

int loops = 0;

void setup() {
    Serial.begin(115200);
    String json_string = "[{\"name\":\"Network 1\",\"password\": \"password1\"},{\"name\": \"Network 2\",\"password\": \"password2\"},{\"name\": \"Network 3\",\"password\": \"password3\"}]";
    list_json_doc = new DynamicJsonDocument(1024);
    deserializeJson(*list_json_doc, json_string);
}


void loop() {

    if (loops % 100 == 0) {
        showMemoryUsage(loops);
    }

    // TEST Without Task
    // String response;
    // serializeJson(*list_json_doc, response);


    // TEST With Task
    if (loops < 2000) {
        xTaskCreate(
            [](void *arg){
                {
                    String _response;
                    DynamicJsonDocument * _list = (DynamicJsonDocument*)arg;
                    serializeJson(*_list, _response);
                }
                vTaskDelete(NULL);
            },
            "handler",
            5000,
            (void*)list_json_doc,
            1,
            NULL
        );
    }

    delay(10);
    loops++;
}


void showMemoryUsage(int iteration) {
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    Serial.print("iteration: ");
    Serial.print(iteration);
    Serial.print("\tTotal Free: ");
    Serial.print(info.total_free_bytes);
    Serial.print("\tMinimum free: ");
    Serial.print(info.minimum_free_bytes);
    Serial.print("\tLargest free block: ");
    Serial.println(info.largest_free_block);
}