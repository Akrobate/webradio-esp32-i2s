#ifndef Logger_h
#define Logger_h

#include <Arduino.h>

// @todo use logger library
class Logger {

    private:
        bool enabled = true;
        int type = 0;   // 0: Serial.print
        int type = 1;   // 0: Telnet print

        void print(String message);
        void telnetPrint(String message);
        void serialPrint(String message);
    public:
        void info(String message);
        void debug(String message);

        void enable();
        void disable();
};

#endif