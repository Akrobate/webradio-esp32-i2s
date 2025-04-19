#include "Logger.h"

Logger::Logger() {
    // Exmpty
}

void Logger::print(String message) {
    if (this->enabled == false) {
        return;
    }
    this->serialPrint(message);
}


void Logger::serialPrint(String message) {
    Serial.println(message);
}

// @todo implement telnetPrint
void Logger::telnetPrint(String message) {
    Serial.println(message);
}


void Logger::info(String message) {
    this->print(message);
}


void Logger::log(String message) {
    this->print(message);
}


void Logger::debug(String message) {
    this->print(message);
}

void Logger::enable() {
    this->enabled = true;
}


void Logger::disable() {
    this->enabled = false;
}


Logger& Logger::getInstance() {
    static Logger instance;  // Initialisation thread-safe garantie en C++11+
    return instance;
}

