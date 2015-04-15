/*
"Hello World" EXAMPLE for LiquidCrystal_I2C_ST7032i library.
  
This example doesn't use backlight control. Connect the backlight via an appropriate series resistor to a constant power source.
*/

#if defined(__AVR_ATtiny85__) || (__AVR_ATtiny2313__)  // This has not been tested
  #include "TinyWireM.h"
#else 
  #include <Wire.h>
#endif

#include <LiquidCrystal_I2C_ST7032i.h>

LiquidCrystal_I2C_ST7032i lcd(0x3E,8,2);  //  Set the LCD address to 0x3E for a 8 chars and 2 line display (no backlight controll)

void setup(){
  lcd.init();           //  Initialize the lcd 

  lcd.clear();          //  Clear the display
  
  lcd.setCursor(1,0);   //  Set the cursor to row 0, col 1
  lcd.print("Hello");   //  Print the first word
  
  lcd.setCursor(1,1);   //  Set the cursor to row 1, col 1
  lcd.print("World!");  //  print the second word
}

void loop()
{
}
