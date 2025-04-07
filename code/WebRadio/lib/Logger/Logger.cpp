#include "Logger.h"

Logger::Logger() {
}


void Logger::print(String message) {
    if (!this->enabled) {
        return;
    }

    switch(this->type) {
        case 0:
            this->serialPrint(message);
            break;
        case 1:
            this->telnetPrint(message);
            break;
        default:
          // code block
    }
}


void Logger::serialPrint(String message) {
    Serial.print(message);
}

// @todo implement telnetPrint
void Logger::telnetPrint(String message) {
    Serial.print(message);
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