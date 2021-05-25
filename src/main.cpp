#include <Arduino.h>
#include <TM1637Display.h>
#include <OneWire.h>

// Module connection pins (Digital Pins)
#define CLK 5
#define DIO 6

int DS18S20_Pin = 7;             //DS18S20 Signal pin
TM1637Display display(CLK, DIO); // 7-segments display
OneWire ds(DS18S20_Pin);         // Temperature chip i/o on digital pin 2

float getTemp();

void setup()
{
  Serial.begin(9600);
  display.setBrightness(0x0f);
}

void loop(void)
{
  float temperature = getTemp();
  display.showNumberDec(temperature, false);
  Serial.println(temperature);

  delay(1000);
}

// returns the temperature from one DS18S20 in DEG Celsius
float getTemp()
{
  byte data[12];
  byte addr[8];

  if (!ds.search(addr))
  {
    //no more sensors on chain, reset search
    ds.reset_search();
    return -1000;
  }

  if (OneWire::crc8(addr, 7) != addr[7])
  {
    Serial.println("CRC is not valid!");
    return -1000;
  }

  if (addr[0] != 0x10 && addr[0] != 0x28)
  {
    Serial.print("Device is not recognized");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++)
  { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;
}