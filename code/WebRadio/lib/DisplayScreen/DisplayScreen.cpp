#include "DisplayScreen.h"

DisplayScreen::DisplayScreen() {
}

void DisplayScreen::init() {
    this->u8g2 = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
}

void DisplayScreen::clear() {
    this->u8g2->clearBuffer();
}


void DisplayScreen::demo() {
 
    String demo_string = "Démo";

    this->u8g2->setFont(u8g2_font_7x13B_tf);
    this->u8g2->clearBuffer();
    this->u8g2->setCursor(20, 20);
    this->u8g2->print(demo_string);
    this->display();
}


void DisplayScreen::display() {
    this->u8g2->sendBuffer();
}
