#ifndef Logger_h
#define Logger_h

#include <Arduino.h>

// @todo use logger library

class Logger {

    private:
        bool enabled = true;

    public:
        void info(String message);
        void enable();
        void disable();
};

#endif