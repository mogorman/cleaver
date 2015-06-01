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
//                ________   http://highlowtech.org pinout
//  Reset PB5     |1    8| Vcc+
//  A3    PB3 D3  |2    7| D2 PB2 A1 SCK SCL 
//  A2    PB4 D4  |3    6| D1 PB1 PWM MISO 
//        GND     |4    5| D0 PB0 PWM AREF MOSI SDA
//                --------
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
//#define INTERNAL2V56_NO_CAP (6)

#define TEMP A3
#define POT  A2
#define IRON  1

#ifndef GIT
#define GIT "not git"
#endif

#ifndef TIME
#define TIME "no time"
#endif

//Define Variables we'll be connecting to
int update_display;
int temperature;
int minutes;
int tenth_seconds;
int user_input;
int ms;
int start;
int stop;
//Specify the links and initial tuning parameters

LiquidCrystal_I2C_ST7032i lcd(0x3E,8,2);  // set the LCD address to 0x3E for a 8 chars and 2 line display

/* // the setup routine runs once when you press reset: */
void setup()
{
  int temp;
  //  delay(2000);
  pinMode(IRON,OUTPUT);
  pinMode(POT,INPUT);
  pinMode(TEMP,INPUT);
  digitalWrite(IRON,LOW);
  update_display = 1;

  temperature = analogRead(TEMP);
  user_input = analogRead(POT);
  user_input = 1023 - user_input;
  minutes = 0;
  tenth_seconds = 0;
  start = 0;
  stop = 0;
  ms = 0;
  lcd.init();
  lcd.clear();
  //  lcd.blink();
  lcd.setContrast(29);
  //if the pot is at 0 and the iron is unplugged
  if(user_input < 50 && (temperature > 750 && temperature < 800 )) {
    lcd.setCursor(0,0);
    lcd.print(GIT);
    lcd.setCursor(0,1);
    lcd.print(TIME);  
    delay(5000);
  }
  if (temperature > 750 && temperature < 800 ) {
    delay(1000);
    do {
      temp = temperature - analogRead(TEMP);
      if(update_display == 60 && stop == 0) {
	lcd.setCursor(0,0);
	lcd.print("  PLUG  ");
	lcd.setCursor(0,1);
	lcd.print("IRON IN ");
	update_display = 0;
	stop = 1;
      } else if (update_display == 60 && stop == 1) {
	lcd.clear();
	update_display = 0;
	stop = 0;
      }
      delay(10);
      update_display++;
    }
    while (temp < 25);    
  }
  stop = 0;
  update_display = 0;
  //  lcd.command(0b00011000);
  lcd.command(0b00110100);
  //turn the PID on
  // Iron_PID.SetMode(AUTOMATIC);
  //  Iron_PID.SetSampleTime(250);
}

void loop()
{
  stop = millis();
  ms += stop - start;
  if( ms > 100) {
    tenth_seconds += (ms/100);
    ms = ms % 100;
    update_display = 1;
  } 
  if(tenth_seconds == 600) {
    tenth_seconds = 0;
    minutes++;
  }
  start = millis();
  //  lcd.clear();
  if(update_display) {
    temperature = analogRead(TEMP);
    if((temperature - user_input) > 0) {
      digitalWrite(IRON, LOW);
    } else {
      digitalWrite(IRON,HIGH);
    }
    
    update_display = 0;
    user_input = analogRead(POT);
    user_input = 1023 - user_input;
    if(user_input >750)
      user_input = 750;
    if(user_input < 50)
      user_input = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    if(!user_input) {
      lcd.print("Off");
    } else {
      lcd.print(user_input);
    }
    lcd.print("  ");
    lcd.print(temperature);

  }
}

