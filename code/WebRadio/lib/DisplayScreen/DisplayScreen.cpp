#include "DisplayScreen.h"

DisplayScreen::DisplayScreen() {
}

void DisplayScreen::injectBusinesState(BusinessState * business_state) {
    this->business_state = business_state;
}

void DisplayScreen::injectStreamRepository(StreamRepository * stream_repository) {
    this->stream_repository = stream_repository;
}

void DisplayScreen::init() {
    this->u8g2 = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
    Serial.println("..................Init display");
    this->u8g2->begin();
    this->u8g2->enableUTF8Print();

    xTaskCreate(
        [](void *arg){
            DisplayScreen * display_screen = (DisplayScreen *)arg;
            while(1) {
                display_screen->displayStack();
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        },
        "displayTask",
        2000,
        this,
        1,
        NULL
    );
}


void DisplayScreen::displayStack() {

    if (
        this->business_state->getInitingDevice() 
        && !this->business_state->getIsConnectingToWifi()
    ) {
        this->splashScreen();
        return;
    }

    if (this->business_state->getIsConnectingToWifi()) {
        this->connectingScreen();
        return;
    }

    if (!this->business_state->getIsConnectingToWifi() && !this->business_state->getDateTimeConfigured()) {
        this->configuringTimeScreen();
        return;
    }


    if (millis() - this->business_state->getVolumeChangedAtMillis() < 2000) {
        this->volumeScreen();
        return;
    }

    if (millis() - this->business_state->getStreamChangedAtMillis() < 2000) {
        this->streamSelectionScreen();
        return;
    }

    if (this->business_state->getPlayingStream() >= 0) {
        this->playingStreamScreen();
        return;
    }

    this->standbyScreen();
}


void DisplayScreen::clear() {
    this->u8g2->clearBuffer();
}


void DisplayScreen::splashScreen() {
    this->clear();
    String text = "WebRadio"; 
    this->u8g2->setFont(u8g2_font_spleen16x32_me);

    int text_width = this->u8g2->getUTF8Width(text.c_str());
    int text_height = this->u8g2->getAscent() - this->u8g2->getDescent();

    int x = this->u8g2->getDisplayWidth() / 2 - text_width / 2;
    int y = this->u8g2->getDisplayHeight() / 2 + text_height / 2;

    this->u8g2->drawUTF8(x, y, text.c_str());    
    this->display();
}


void DisplayScreen::display() {
    this->u8g2->sendBuffer();
}


void DisplayScreen::infoScreen() {
    this->clear();

    this->u8g2->setFont(u8g2_font_6x12_tf);
    this->u8g2->setCursor(5, 10);
    this->u8g2->print("Temperature: ");
    this->u8g2->print(this->business_state->getTemperature());
    this->u8g2->print(" C");
    this->u8g2->setCursor(0, 30);
    this->u8g2->print("Pressure: ");
    this->u8g2->print(this->business_state->getPressure());
    this->u8g2->print(" hPa");

    String text = this->business_state->getDateTime();
    int text_width = this->u8g2->getUTF8Width(text.c_str());
    int x = this->u8g2->getDisplayWidth() / 2 - text_width / 2;
    this->u8g2->drawUTF8(x, 50, text.c_str());    


    this->display();
}


void DisplayScreen::standbyScreen() {
    this->clear();

    int text_width = 0;
    int text_height = 0;
    int x = 0;
    int y = 0;

    String formated_time = this->business_state->getFormatedTime();
    this->u8g2->setFont(u8g2_font_osr26_tn);
    text_width = this->u8g2->getUTF8Width(formated_time.c_str());
    text_height = this->u8g2->getAscent() - this->u8g2->getDescent();
    x = this->u8g2->getDisplayWidth() / 2 - text_width / 2;
    y = this->u8g2->getDisplayHeight() / 2 + text_height / 2;
    this->u8g2->drawUTF8(x, y, formated_time.c_str());  
    

    String formated_date = this->business_state->getFormatedDate();
    this->u8g2->setFont(u8g2_font_6x12_tf);
    text_height = this->u8g2->getAscent() - this->u8g2->getDescent();
    x = 0;
    y = text_height;
    this->u8g2->drawUTF8(x, y, formated_date.c_str()); 

    float temperature = this->business_state->getTemperature();
    String formated_temperature = String(temperature) + " C";
    this->u8g2->setFont(u8g2_font_6x12_tf);
    text_width = this->u8g2->getUTF8Width(formated_temperature.c_str());
    text_height = this->u8g2->getAscent() - this->u8g2->getDescent();
    x = this->u8g2->getDisplayWidth() - text_width;
    y = text_height;
    this->u8g2->drawUTF8(x, y, formated_temperature.c_str());  

    this->display();
}


void DisplayScreen::configuringTimeScreen() {
    this->clear();

    int text_width = 0;
    int text_height = 0;
    int x = 0;
    int y = 0;

    String text = "setting time";
    this->u8g2->setFont(u8g2_font_7x13_tf);
    text_width = this->u8g2->getUTF8Width(text.c_str());
    text_height = this->u8g2->getAscent() - this->u8g2->getDescent();
    x = this->u8g2->getDisplayWidth() / 2 - text_width / 2;
    y = this->u8g2->getDisplayHeight() / 2 + text_height / 2;
    this->u8g2->drawUTF8(x, y, text.c_str());  
    
    float temperature = this->business_state->getTemperature();
    String formated_temperature = String(temperature) + " C";
    this->u8g2->setFont(u8g2_font_6x12_tf);
    text_width = this->u8g2->getUTF8Width(formated_temperature.c_str());
    text_height = this->u8g2->getAscent() - this->u8g2->getDescent();
    x = this->u8g2->getDisplayWidth() - text_width;
    y = text_height;
    this->u8g2->drawUTF8(x, y, formated_temperature.c_str());  
    

    this->u8g2->drawCircle(80, 48, 5, U8G2_DRAW_ALL);
    this->u8g2->drawCircle(48, 48, 5, U8G2_DRAW_ALL);
    this->u8g2->drawCircle(64, 48, 5, U8G2_DRAW_ALL);

    int position = millis() / 1000 % 3;

    switch (position) {
        case 0:
            this->u8g2->drawDisc(48, 48, 5, U8G2_DRAW_ALL);
            break;
        case 1:
            this->u8g2->drawDisc(64, 48, 5, U8G2_DRAW_ALL);
            break;
        case 2:
            this->u8g2->drawDisc(80, 48, 5, U8G2_DRAW_ALL);
            break;    
        default:
            break;
    }
    this->display();
}


void DisplayScreen::playingStreamScreen() {
    this->clear();

    int text_width = 0;
    int text_height = 0;
    int x = 0;
    int y = 0;

    String formated_time = this->business_state->getFormatedTime();
    this->u8g2->setFont(u8g2_font_6x12_tf);
    text_height = this->u8g2->getAscent() - this->u8g2->getDescent();
    x = 0;
    y = text_height;
    this->u8g2->drawUTF8(x, y, formated_time.c_str()); 

    JsonObject stream = this->stream_repository->getStreamByIndex(this->business_state->getPlayingStream());
    String playing_string = stream["name"];

    // Autosizing
    // u8g2_font_6x12_tf
    // u8g2_font_7x13_tf
    // u8g2_font_8x13_tf
    // u8g2_font_9x15_tf
    // u8g2_font_10x20_tf

    int max_width = this->u8g2->getDisplayWidth() - 20;

    this->u8g2->setFont(u8g2_font_10x20_tf);
    text_width = this->u8g2->getUTF8Width(playing_string.c_str());

    if (text_width > max_width) {
        this->u8g2->setFont(u8g2_font_9x15_tf);
        text_width = this->u8g2->getUTF8Width(playing_string.c_str()); 
    }

    if (text_width > max_width) {
        this->u8g2->setFont(u8g2_font_8x13_tf);
        text_width = this->u8g2->getUTF8Width(playing_string.c_str()); 
    }

    if (text_width > max_width) {
        this->u8g2->setFont(u8g2_font_7x13_tf);
        text_width = this->u8g2->getUTF8Width(playing_string.c_str()); 
    }

    if (text_width > max_width) {
        this->u8g2->setFont(u8g2_font_6x12_tf);
        text_width = this->u8g2->getUTF8Width(playing_string.c_str()); 
    }

    text_height = this->u8g2->getAscent() - this->u8g2->getDescent();
    x = this->u8g2->getDisplayWidth() / 2 - text_width / 2;
    y = this->u8g2->getDisplayHeight() / 2 + text_height / 2;
    this->u8g2->drawUTF8(x, y, playing_string.c_str());  
    

    float temperature = this->business_state->getTemperature();
    String formated_temperature = String(temperature) + " C";
    this->u8g2->setFont(u8g2_font_6x12_tf);
    text_width = this->u8g2->getUTF8Width(formated_temperature.c_str());
    text_height = this->u8g2->getAscent() - this->u8g2->getDescent();
    x = this->u8g2->getDisplayWidth() - text_width;
    y = text_height;
    this->u8g2->drawUTF8(x, y, formated_temperature.c_str());  

    // Circle animation
    int position = millis() / 200 % 6;
    if (position > 3) {
        position = 6 - position;
    }
    this->u8g2->drawCircle(64, 54, 1 + position, U8G2_DRAW_ALL);

    this->display();
}


void DisplayScreen::volumeScreen() {

    int volume = this->business_state->getPlayingVolume();
    if (this->displayed_volume == volume) {
        return;
    }

    this->clear();
    this->u8g2->setDrawColor(1);

    this->displayed_volume = volume;
    int width = this->u8g2->getDisplayWidth();
    int height = this->u8g2->getDisplayHeight();

    int max_volume = 21;
    int bar_width = width / max_volume;

    for (int i = 0; i < max_volume; i++) {
        int x = i * bar_width;
        int y = 0;
        int bar_height = (height / max_volume) * i;
        if (i < volume) {
            this->u8g2->drawRBox(x, y + (height - bar_height), bar_width, bar_height, 1);
        } else {
            this->u8g2->drawRFrame(x, y + (height - bar_height), bar_width, bar_height, 1);
        }
    }
    this->display();
}


void DisplayScreen::connectingScreen() {
    this->clear();

    String text = "connecting";
    this->u8g2->setFont(u8g2_font_7x13_tf);
    int text_width = this->u8g2->getUTF8Width(text.c_str());
    int text_height = this->u8g2->getAscent() - this->u8g2->getDescent();

    int x = this->u8g2->getDisplayWidth() / 2 - text_width / 2;
    int y = this->u8g2->getDisplayHeight() / 2 + text_height / 2;

    this->u8g2->drawUTF8(x, y, text.c_str());

    int width = this->u8g2->getDisplayWidth();
    int height = this->u8g2->getDisplayHeight();

    int position = millis() / 1000 % 3;

    this->u8g2->drawCircle(80, 48, 5, U8G2_DRAW_ALL);
    this->u8g2->drawCircle(48, 48, 5, U8G2_DRAW_ALL);
    this->u8g2->drawCircle(64, 48, 5, U8G2_DRAW_ALL);

    switch (position) {
        case 0:
            this->u8g2->drawDisc(48, 48, 5, U8G2_DRAW_ALL);
            break;
        case 1:
            this->u8g2->drawDisc(64, 48, 5, U8G2_DRAW_ALL);
            break;
        case 2:
            this->u8g2->drawDisc(80, 48, 5, U8G2_DRAW_ALL);
            break;    
        default:
            break;
    }

    
    this->display();
}


void DisplayScreen::streamSelectionScreen() {
    this->clear();


    int streams_count = this->stream_repository->countStream();
    int displayed_lines_count = 5;
    int playing_stream_index = this->business_state->getPlayingStream();

    int x = 10;
    this->u8g2->setFontMode(1);
    this->u8g2->setFont(u8g2_font_6x12_tf);

    this->u8g2->setDrawColor(1);
    int width = this->u8g2->getDisplayWidth();
    this->u8g2->drawBox(0, 27, width, 12);

    for (int i = 0; i < displayed_lines_count; i++) {
        String stream_name = "";
        if (playing_stream_index - 2 + i >= 0 && playing_stream_index - 2 + i < streams_count) {
            JsonObject stream = this->stream_repository->getStreamByIndex(playing_stream_index - 2 + i);
            stream_name = stream["name"].as<String>();
        }

        int y = (i + 1) * 12;
        this->u8g2->setDrawColor(2);
        this->u8g2->drawUTF8(x, y, stream_name.c_str());
    }

    this->display();
}