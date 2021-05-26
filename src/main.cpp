#include <Arduino.h>
#include <TM1637Display.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// DS18B20 (Digital Temperature Sensor) and Arduino © GPL3+
// https://create.arduino.cc/projecthub/TheGadgetBoy/ds18b20-digital-temperature-sensor-and-arduino-9cc806
// TM1637 Digit Display - Arduino Quick Tutorial © GPL3+
// https://create.arduino.cc/projecthub/ryanchan/tm1637-digit-display-arduino-quick-tutorial-ca8a93

#define TM1637_CLK 5   // TM1637 clock pin
#define TM1637_DIO 6   // TM1637 data pin
#define ONE_WIRE_BUS 7 //DS18S20 Signal pin

TM1637Display display(TM1637_CLK, TM1637_DIO); // 7-segments display
OneWire oneWire(ONE_WIRE_BUS);                 // Temperature chip i/o on digital pin 2
DallasTemperature sensors(&oneWire);           // Pass oneWire reference to DallasTemperature library

void setup(void)
{
  sensors.begin();
  Serial.begin(9600);
  display.setBrightness(0x0f);
}

void loop(void)
{
  // Send the command to get temperatures
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  display.showNumberDec(temperature, false);
  delay(1000);
}