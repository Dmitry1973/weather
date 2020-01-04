/* 
 * Demo for SSD1306 based 128x64 OLED module using Adafruit SSD1306 
 * library (https://github.com/adafruit/Adafruit_SSD1306).
 * 
 * See https://github.com/pacodelgado/arduino/wiki/SSD1306-based-OLED-connected-to-Arduino 
 * for more information.
 *
 */
 
//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// If using software SPI (the default case):
/*#define OLED_MOSI  11   //D1
#define OLED_CLK   12   //D0
#define OLED_DC    9
#define OLED_CS    8
#define OLED_RESET 10*/

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SEALEVELPRESSURE_HPA (1013.25)

//Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
Adafruit_BME280 bme; // I2C

int counter = 0;

unsigned long delayTime;
// картинки
/*const unsigned char myTemperature [] PROGMEM = {
  0x18, 0xFC, 0xFC, 0x18, 0x18, 0xFC, 0xFC, 0x18,
  0x18, 0xFC, 0xFC, 0x18, 0x18, 0x3C, 0x7E, 0xFF*/
// градусник и плюс
const unsigned char myTemp_plus [] PROGMEM = {
  0x18, 0x00, 0xFC, 0x38, 0xFC, 0x38, 0x18, 0x38,
  0x19, 0xFF, 0xFD, 0xFF, 0xFD, 0xFF, 0x18, 0x38,
  0x18, 0x38, 0xFC, 0x38, 0xFC, 0x00, 0x18, 0x00,
  0x18, 0x00, 0x18, 0x00, 0x7E, 0x00, 0xFF, 0x00
};

// градусник и минус
  const unsigned char myTemp_minus [] PROGMEM = {
  0x18, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x18, 0x00,
  0x19, 0xFF, 0xFD, 0xFF, 0xFD, 0xFF, 0x18, 0x00,
  0x18, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x18, 0x00,
  0x18, 0x00, 0x18, 0x00, 0x7E, 0x00, 0xFF, 0x00
};
/*const unsigned char myHumidity [] PROGMEM = {
  0x31, 0x80, 0x98, 0xC0, 0xC8, 0x40, 0x63, 0x10,
  0x01, 0x90, 0xC8, 0x80, 0x6C, 0x20, 0x23, 0x30
};*/

// влажность
const unsigned char myHumidity [] PROGMEM = {
  0x06, 0x00, 0x06, 0x00, 0x0F, 0x00, 0x0F, 0x00,
  0x1F, 0x80, 0x19, 0x80, 0x39, 0xC0, 0x70, 0xE0,
  0x70, 0x70, 0xE0, 0x70, 0xE0, 0x70, 0xE0, 0xF0,
  0x70, 0xE0, 0x7F, 0xE0, 0x3F, 0xC0, 0x0F, 0x00
};

void setup()   {
   Serial.begin(9600);
   //Serial.println(F("BME280 test"));
  
    bool status;
    
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    
    //Serial.println("-- Default Test --");
    delayTime = 1000;
  
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop()
{
  // вывод примерно раз в минуту в монитор
    if (counter % 3){
      counter ++;
      
    }
    else{
      valuesForOutput();
      counter ++;
    }
  
//начало вывода на LCD 32x128
  display.clearDisplay();  
  display.setCursor(0,4);
  display.setTextSize(4);

// если темп больше 10С
    if (bme.readTemperature() > 10) {
      display.print(bme.readTemperature(),1);
      display.drawBitmap(52, 4, myTemp_plus, 16, 16, WHITE);
      display.setCursor(104,0);
      display.setTextSize(1);
      display.print("Temp");  
      display.setCursor(102,8);
      display.setTextSize(1);
      display.print("o");
      display.setCursor(110,10);
      display.setTextSize(3);
      display.print("C");
      display.display();
    }
//если темп от 0 и до 10С, чтобы не съехжала картинка добавляем пробел
    else if (bme.readTemperature() < 10 && bme.readTemperature() >=0) {
      display.print(" ");
      display.print(bme.readTemperature(),1);
      display.drawBitmap(52, 4, myTemp_plus, 16, 16, WHITE);
      display.setCursor(104,0);
      display.setTextSize(1);
      display.print("Temp");  
      display.setCursor(102,8);
      display.setTextSize(1);
      display.print("o");
      display.setCursor(110,10);
      display.setTextSize(3);
      display.print("C");
      display.display();
    }
//если темп от -10 и до 0С, чтобы не съехжала картинка добавляем пробел
    else if (bme.readTemperature() > -10 && bme.readTemperature() <0) {
      display.print(" ");
      display.print(bme.readTemperature() * (-1) ,1);
      display.drawBitmap(52, 4, myTemp_minus, 16, 16, WHITE);
      display.setCursor(104,0);
      display.setTextSize(1);
      display.print("Temp");  
      display.setCursor(102,8);
      display.setTextSize(1);
      display.print("o");
      display.setCursor(110,10);
      display.setTextSize(3);
      display.print("C");
      display.display();
    }
// если темп меньше -10
    else {
      display.print(bme.readTemperature() * (-1) ,1);
      display.drawBitmap(52, 4, myTemp_minus, 16, 16, WHITE);  
      display.setCursor(104,0);
      display.setTextSize(1);
      display.print("Temp");  
      display.setCursor(102,8);
      display.setTextSize(1);
      display.print("o");
      display.setCursor(110,10);  
      display.setTextSize(3);
      display.print("C");
      display.display();
    }
  
  delay(5000);

// давление в мм рт.ст.  
  display.clearDisplay();
  display.setCursor(0,4);
  display.setTextSize(4);
  display.print(bme.readPressure() / 133.32,0);
  display.setCursor(80,0);
  display.setTextSize(1);
  display.print("Pressure");
  display.setCursor(80,16);
  display.setTextSize(2);
  display.print("mmHg");
  display.display();
  
  delay(5000);

 // давление в гПа
  display.clearDisplay();
  display.setCursor(0,8);
  display.setTextSize(3);
  display.print(bme.readPressure()/100,0);
  display.setCursor(80,0);
  display.setTextSize(1);
  display.print("Pressure");
  display.setCursor(88,16);
  display.setTextSize(2);
  display.print("hPa");
  display.display();

//относительная влажность  
  delay(5000);
  display.clearDisplay();
  display.setCursor(0,4);
  display.setTextSize(4);
  display.print(bme.readHumidity(),1);
  display.drawBitmap(52, 4, myHumidity, 12, 16, WHITE);
  display.setCursor(96,0);
  display.setTextSize(1);
  display.print("R_Hum");
  display.setCursor(106,10);
  display.setTextSize(3);
  display.print("%");  
  display.display();
  
  delay(5000);
  display.display();
}
//вывод в серийный монитор
void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure_hPa = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Pressure_mmHg = ");
    Serial.print(bme.readPressure() / 133.32);
    Serial.println(" mmHg");
/*
    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");
*/
    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
    Serial.println();
}
// вывод нужных данных в строку
void valuesForOutput(){

  Serial.print(bme.readTemperature());
  Serial.print(",");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.print(",");
  Serial.print(bme.readPressure() / 133.32);
  Serial.print(",");
  Serial.print(bme.readHumidity());
  Serial.println(",");
  
  }
