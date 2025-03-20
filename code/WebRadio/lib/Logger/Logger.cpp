#include "Logger.h"

Logger::Logger() {
}


void Logger::print(String message) {
    if (!this->enabled) {
        return;
    }

    switch(this->type) {
        case 0:
            Serial.print(message);
            break;
        default:
          // code block
    }
}


void Logger::info(String message) {
    print(message);
}


void Logger::debug(String message) {
    print(message);
}

void Logger::enable() {
    this->enabled = true;
}


void Logger::disable() {
    this->enabled = false;
}