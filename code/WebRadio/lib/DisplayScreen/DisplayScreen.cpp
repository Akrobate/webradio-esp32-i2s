#include "DisplayScreen.h"

DisplayScreen::DisplayScreen() {
}

void DisplayScreen::init() {
    this->u8g2 = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
    this->u8g2->begin();
    this->u8g2->enableUTF8Print();
}

void DisplayScreen::clear() {
    this->u8g2->clearBuffer();
}


void DisplayScreen::demoScreen() {
 
    this->clear();
    String demo_string = "Démo"; 
    this->u8g2->setFont(u8g2_font_spleen32x64_me); // u8g2_font_7x13B_tf
    this->u8g2->setCursor(2, 64);
    this->u8g2->print(demo_string);
    this->display();
}


void DisplayScreen::display() {
    this->u8g2->sendBuffer();
}


void DisplayScreen::injectBusinesState(BusinessState * business_state) {
    this->business_state = business_state;
}


void DisplayScreen::infoScreen() {
    this->clear();

    this->u8g2->setFont(u8g2_font_7x13B_tf);
    this->u8g2->setCursor(0, 10);
    this->u8g2->print("Temperature: ");
    this->u8g2->print(this->business_state->getTemperature());
    this->u8g2->print(" C");
    this->u8g2->setCursor(0, 30);
    this->u8g2->print("Pressure: ");
    this->u8g2->print(this->business_state->getPressure());
    this->u8g2->print(" hPa");

    this->display();
}



void DisplayScreen::temperatureScreen() {
    this->clear();

    this->u8g2->setFont(u8g2_font_osr26_tn);
    this->u8g2->setCursor(2, 45);
    this->u8g2->print(this->business_state->getTemperature());
    this->u8g2->print("°C");
    this->display();
}

