#ifndef Logger_h
#define Logger_h

#include <Arduino.h>

// @todo use logger library

class Logger {
    public:
        void info(String message) {
            Serial.println(message);
        }
};

#endif