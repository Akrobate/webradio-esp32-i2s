#include <U8g2lib.h>
#include <Adafruit_BMP085.h>

#include <Wire.h>

#define seaLevelPressure_hPa 1013.25


U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
// U8G2_R0 ou U8G2_R2: mode paysage, U8G2_R1 ou U8G2_R3: mode portrait
Adafruit_BMP085 bmp;


void setup(void) {
  Serial.begin(115200);

  u8g2.begin();
  u8g2.enableUTF8Print(); //nécessaire pour écrire des caractères accentués
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
}


int loops = 0;

void loop(void) {

  /*
    Serial.print("Température = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" °C");
    
    Serial.print("Pression = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" mètres");

    Serial.print("Pression au niveau de la mer (calculée) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");

    Serial.print("Altitude réelle = ");
    Serial.print(bmp.readAltitude(seaLevelPressure_hPa * 100));
    Serial.println(" mètres");
    
    Serial.println();
  */

  /********* On dessine le logo (bitmap défini plus haut) ***************/
  u8g2.clearBuffer(); // on efface ce qui se trouve déjà dans le buffer

  /************** On écrit "Électronique en amateur" *********************/
  // u8g2.setFont(u8g2_font_7x13B_tf); // choix d'une police de caractère
  
/******************* Écrire du texte *******************************/

  // u8g2.setFont(u8g2_font_ncenB10_tf); // choix de la police
  u8g2.setFont(u8g2_font_7x13B_tf);
  u8g2.clearBuffer();
  u8g2.setCursor(5, 20); // position du début du texte

  String temperature = "Temperature ";
  String pression = "Pression ";
  temperature.concat(bmp.readTemperature());
  pression.concat((float)bmp.readPressure() / 100);

  u8g2.print(temperature);  // on écrit le texte
  u8g2.setCursor(5, 35);
  u8g2.print(pression);
  u8g2.sendBuffer();
  delay(10);

  if (loops % 100 == 0) {
    showMemoryUsage(loops);
  }
  loops++;
}


void showMemoryUsage(int iteration) {
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    Serial.print("iteration: ");
    Serial.print(iteration);
    Serial.print("\tTotal Free: ");
    Serial.print(info.total_free_bytes);
    Serial.print("\tMinimum free: ");
    Serial.print(info.minimum_free_bytes);
    Serial.print("\tLargest free block: ");
    Serial.println(info.largest_free_block);
}