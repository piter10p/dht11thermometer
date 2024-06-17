#include "DHT.h" //https://github.com/markruys/arduino-DHT
#include "U8g2lib.h" //https://github.com/olikraus/u8g2
#include "Wire.h"

/*
/ PINS:
/ A5 - OLED SCK/SCL
/ A4 - OLED SDA
/ D2 - DHT11 data
*/

#define DHTPIN 2
#define TEXT_BUFFER_SIZE 7

// Defines temperature considered hot in celcius.
#define HOT_TEMP 28

// Defines temperature considered hot in celcius.
#define COLD_TEMP 22

// Set true to display temperature in fahrenheit instead of celcius.
#define USE_FAHRENHEIT false

// Value of offset added to temperature in celcius. Use for dht11 error calibration.
#define TEMPERATURE_OFFSET -2

// Value of offset added to humidity in percent. Use for dht11 error calibration.
#define HUMIDITY_OFFSET 5

DHT dht;

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

int humidity;
int temperature;
char numberTextBuffer[TEXT_BUFFER_SIZE];
String temperatureUnit;

void setup()
{
  //Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.setup(DHTPIN);
  u8g2.begin();

  if (USE_FAHRENHEIT)
  {
    temperatureUnit = "°F";
  }
  else
  {
    temperatureUnit = "°C";
  }
}

void writeNumberToBuffer(int number, String unit)
{
  String str = String(number);
  str.concat(unit);
  str.toCharArray(numberTextBuffer, TEXT_BUFFER_SIZE);
}

void writeDataToOled()
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvR12_tf);
  
  writeNumberToBuffer(temperature, temperatureUnit);
  u8g2.drawUTF8(30, 30, numberTextBuffer);

  writeNumberToBuffer(humidity, "%");
  u8g2.drawUTF8(30, 55, numberTextBuffer);

  u8g2.setFont(u8g2_font_unifont_t_weather);
  u8g2.drawGlyph(10, 30, 0x0031);
  u8g2.drawGlyph(10, 55, 0x0032);

  u8g2.setFont(u8g2_font_unifont_t_emoticons);

  if (temperature <= COLD_TEMP)
    u8g2.drawGlyph(90, 40, 0x0072);
  else if (temperature >= HOT_TEMP)
    u8g2.drawGlyph(90, 40, 0x008f);
  else
    u8g2.drawGlyph(90, 40, 0x0024);
  
  u8g2.sendBuffer();
}

void loop()
{
  delay(1000);

  digitalWrite(LED_BUILTIN, HIGH);

  humidity = dht.getHumidity() + HUMIDITY_OFFSET;
  temperature = dht.getTemperature() + TEMPERATURE_OFFSET;

  if (USE_FAHRENHEIT)
  {
    temperature = (temperature * 9/5) + 32;
  }

  writeDataToOled();

  digitalWrite(LED_BUILTIN, LOW);
}
