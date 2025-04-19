#ifndef Logger_h
#define Logger_h

#include <Arduino.h>

// @todo use logger library
class Logger {

    private:
        Logger();
        bool enabled = true;
        int type = 0;   // 0: Serial.print
        // int type = 1;   // 1: Telnet print

        void print(String message);
        void telnetPrint(String message);
        void serialPrint(String message);
    public:
        static Logger& getInstance();

        void info(String message);
        void debug(String message);
        void log(String message);

        void enable();
        void disable();
};

#endif