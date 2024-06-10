#include "DHT.h" //https://github.com/markruys/arduino-DHT
#include "U8g2lib.h" //https://github.com/olikraus/u8g2
#include "Wire.h"

#define DHTPIN 2
#define TEXT_BUFFER_SIZE 7

#define HOT_TEMP 28
#define COLD_TEMP 22

DHT dht;

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

int humidity;
int temperature;
char numberTextBuffer[TEXT_BUFFER_SIZE];

void setup()
{
  //Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.setup(DHTPIN);
  u8g2.begin();
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
  
  writeNumberToBuffer(temperature, "°C");
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

  humidity = dht.getHumidity();
  temperature = dht.getTemperature();

  writeDataToOled();

  digitalWrite(LED_BUILTIN, LOW);
}
