#ifndef DisplayScreen_h
#define DisplayScreen_h

#include <Arduino.h>
#include <U8g2lib.h>


class DisplayScreen {

    private:
        U8G2_SH1106_128X64_NONAME_F_HW_I2C * u8g2;

    public:
        DisplayScreen();

        void init();
        void clear();
        void display();

        void demo();
};

#endif
