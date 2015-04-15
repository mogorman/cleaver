/*
Serial display example for LiquidCrystal_I2C_ST7032i library.

Displays text sent over the serial port (e.g. from the Serial Monitor) on the attached LCD.

This example doesn't use backlight control. Connect the backlight via an appropriate series resistor to a constant power source.
*/
 
#if defined(__AVR_ATtiny85__) || (__AVR_ATtiny2313__)  // This has not been tested
  #include "TinyWireM.h"
#else 
  #include <Wire.h>
#endif

#include <LiquidCrystal_I2C_ST7032i.h>

LiquidCrystal_I2C_ST7032i lcd(0x3E,8,2);  //  set the LCD address to 0x3E for a 8 chars and 2 line display (no backlight controll)

void setup()
{
  lcd.init();                      // Initialize the lcd 
  Serial.begin(9600);
}

void loop()
{
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      lcd.write(Serial.read());
    }
  }
}
