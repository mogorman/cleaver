/*-------------------------------------------------------------------------*/
/* @author    Matthew O'Gorman <mog@rldn.net>                              */
/* @copyright 2015 Matthew O'Gorman                                        */
/* Cleaver soldering iron controller code for attiny85.                    */
/* License : This program is free software, distributed under the terms of */
/*          the GNU General Public License Version 3. See the COPYING file */
/*          at the top of the source tree.                                 */
/*-------------------------------------------------------------------------*/
#define __AVR_ATtiny85__ 1

#include <TinyWireM.h>
#include <LiquidCrystal_I2C_ST7032i.h>


#define LED 3

LiquidCrystal_I2C_ST7032i lcd(0x3E,8,2);  // set the LCD address to 0x3E for a 8 chars and 2 line display

// the setup routine runs once when you press reset:
void setup()
{                
  pinMode(LED, OUTPUT);
  lcd.init();
  
  lcd.clear(); 
  lcd.setCursor(1,0);
  lcd.print("Hello");
  lcd.setCursor(1,1);
  lcd.print("World!");
}

void loop() {
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(5000);               // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(5000);               // wait for a secondIGH is the voltage level)
}
