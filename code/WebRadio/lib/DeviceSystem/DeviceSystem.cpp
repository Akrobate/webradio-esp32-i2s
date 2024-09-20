#include "DeviceSystem.h"
#include "time.h"

DeviceSystem::DeviceSystem() {
}

void DeviceSystem::update() {
    multi_heap_info_t info;

    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT); // internal RAM, memory capable to store data or to create new task

    this->total_free_bytes = (int)info.total_free_bytes; // total currently free in all non-continues blocks
    this->minimum_free_bytes = (int)info.minimum_free_bytes; // minimum free ever
    this->largest_free_block = (int)info.largest_free_block; // largest continues block to allocate big array

    if (this->date_time_configured) {   
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("Failed to obtain time");
        } else {
            char buffer[64];
            strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &timeinfo);
            this->date_time = String(buffer);
            
            strftime(buffer, sizeof(buffer), "%H:%M", &timeinfo);
            this->formated_time = String(buffer);
            
            strftime(buffer, sizeof(buffer), "%d/%m", &timeinfo);
            this->formated_date = String(buffer);
        }
    }
}


void DeviceSystem::configureTimeTask() {
    xTaskCreate(
        [](void *arg){
          {
            DeviceSystem * instance = (DeviceSystem *)arg;
            while(!instance->date_time_configured) {
              Serial.println("Configuring time");
              if (instance->business_state->getIsConnectedToWifi()) {
                configTime(instance->gmt_offset_sec, instance->daylight_offset_sec, instance->ntp_server_host.c_str());
                vTaskDelay(1000);
                struct tm timeinfo;
                if (getLocalTime(&timeinfo)) {
                    instance->date_time_configured = true;
                    break;
                } else {
                    Serial.println("Failed to obtain time");
                }
              } else {
                Serial.println("ConfigTimeTask waiting for wifi connection");
              }
              vTaskDelay(5000);
            }
          }
          vTaskDelete(NULL);
        },
        "ConfigTimeTask",
        5000,
        (void*)this,
        1,
        NULL
    );


}


int DeviceSystem::getTotalFreeBytes() {
    return this->total_free_bytes;
}

int DeviceSystem::getMinimumFreeBytes() {
    return this->minimum_free_bytes;
}

int DeviceSystem::getLargestFreeBlock() {
    return this->largest_free_block;
}

void DeviceSystem::showMemoryUsage() {
    multi_heap_info_t info;

    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT); // internal RAM, memory capable to store data or to create new task

    Serial.print("Total Free: ");
    Serial.print(info.total_free_bytes); // total currently free in all non-continues blocks

    Serial.print("\tMinimum free: ");
    Serial.print(info.minimum_free_bytes); // minimum free ever

    Serial.print("\tLargest free block: ");
    Serial.println(info.largest_free_block); // largest continues block to allocate big array
}

void DeviceSystem::injectBusinesState(BusinessState * business_state) {
    this->business_state = business_state;
}


void DeviceSystem::updateBusinessState() {
    if (this->business_state->lock()) {
        this->business_state->setTotalFreeBytes(this->getTotalFreeBytes());
        this->business_state->setMinimumFreeBytes(this->getMinimumFreeBytes());

        this->business_state->setDateTime(this->date_time);
        this->business_state->setDateTimeConfigured(this->date_time_configured);

        this->business_state->setFormatedTime(this->formated_time);
        this->business_state->setFormatedDate(this->formated_date);
        
        this->business_state->unlock();
    }
}
