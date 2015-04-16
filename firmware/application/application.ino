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
#include <PID_v1.h>
#include <LiquidCrystal_I2C_ST7032i.h>

//           ________   http://highlowtech.org pinout
//  Reset    |1    8| Vcc+
//  A3   D3  |2    7| D2 A1 SCK SCL
//  A2   D4  |3    6| D1 PWM MISO 
//      GND  |4    5| D0 PWM AREF MOSI SDA
//           --------
//

// attiny85
// 1 led, iron is safe / reset
// 2 Temperature probe from iron
// 3 pot for control of the iron
// 4 Gnd
// 5 SDA for display
// 6 pwm for iron, led to alert user its heating
// 7 SCL for display
// 8 Vcc, led to show its on

#define TEMP A3
#define POT A2
#define IRON 1

//Define Variables we'll be connecting to
double Set_point, Input, Output;
int Pot_value;

//Specify the links and initial tuning parameters
PID Iron_PID(&Input, &Output, &Set_point,2,5,1, DIRECT);
LiquidCrystal_I2C_ST7032i lcd(0x3E,8,2);  // set the LCD address to 0x3E for a 8 chars and 2 line display

// the setup routine runs once when you press reset:
void setup()
{
  pinMode(TEMP, INPUT);
  pinMode(POT, INPUT);
  pinMode(IRON, OUTPUT);
  digitalWrite(IRON, LOW);
  lcd.init();

  Pot_value = 0;
  
  lcd.clear(); 
  lcd.setCursor(1,0);
  lcd.print("Hello");
  lcd.setCursor(1,1);
  lcd.print("World!");

  Input = analogRead(TEMP);
  Set_point = Pot_value;
  //turn the PID on
  Iron_PID.SetMode(AUTOMATIC);
}

void loop() {
  
  Input = analogRead(TEMP);
  Iron_PID.Compute();
  analogWrite(IRON,Output);

  Pot_value=analogRead(POT);
  Set_point = Pot_value;
  update_display();
}


void update_display() {
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Bye");
  lcd.setCursor(1,1);
  lcd.print("World!");
}
