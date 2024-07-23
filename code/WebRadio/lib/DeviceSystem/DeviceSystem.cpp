#include "DeviceSystem.h"


DeviceSystem::DeviceSystem() {
}

void DeviceSystem::update() {
    multi_heap_info_t info;

    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT); // internal RAM, memory capable to store data or to create new task

    this->total_free_bytes = (int)info.total_free_bytes; // total currently free in all non-continues blocks
    this->minimum_free_bytes = (int)info.minimum_free_bytes; // minimum free ever
    this->largest_free_block = (int)info.largest_free_block; // largest continues block to allocate big array
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
