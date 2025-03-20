#ifndef Logger_h
#define Logger_h

#include <Arduino.h>

// @todo use logger library
class Logger {

    private:
        bool enabled = true;
        int type = 0;   // 0: Serial.print

        void print(String message);

    public:
        void info(String message);
        void debug(String message);

        void enable();
        void disable();
};

#endif