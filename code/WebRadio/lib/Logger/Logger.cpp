#include "Logger.h"

Logger::Logger() {
}

void Logger::info(String message) {
    if (this->enabled) {
        Serial.println(message);
    }
}


void Logger::enable() {
    this->enabled = true;
}


void Logger::disable() {
    this->enabled = false;
}