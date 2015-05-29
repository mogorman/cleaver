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
//#include <PID_v1.h>
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

//Define Variables we'll be connecting to
double Set_point, Input, Output;
int Pot_value;

int internal_offset;
int internal_readings[45];
int internal_pos;
int update_display;
int lock;
int iron_state;
int point;
int offset;
int temperature;
int minutes;
int seconds;
int ms;
int start;
int stop;
//Specify the links and initial tuning parameters
//PID Iron_PID(&Input, &Output, &Set_point,2,5,1, DIRECT);

LiquidCrystal_I2C_ST7032i lcd(0x3E,8,2);  // set the LCD address to 0x3E for a 8 chars and 2 line display

/* // the setup routine runs once when you press reset: */
void setup()
{
  //  delay(2000);
  pinMode(IRON,OUTPUT);
  pinMode(POT,INPUT);
  pinMode(TEMP,INPUT);
  digitalWrite(IRON,LOW);
  update_display = 1;
  lock = 0;
  point = 0;
  offset = 0;
  iron_state = 0;
  temperature = 0;
  minutes = 0;
  seconds = 0;
  start = 0;
  stop = 0;
  ms = 0;
  lcd.init();
  lcd.clear();
  //  lcd.blink();
  lcd.setContrast(29);
  lcd.setCursor(0,0);
  lcd.print("Bye bye");
  delay(1000);
  //turn the PID on
  digitalWrite(IRON, HIGH);
  while(1);
//  Iron_PID.SetMode(AUTOMATIC);;
}

void loop()
{
  int user_input = 0;

    //  lcd.clear();
  if(update_display == 1) {
  stop = millis();
  ms += stop - start;
  if( ms > 1000) {
     seconds += (ms/1000);
     ms = ms % 1000;
  }
  if(seconds == 60) {
     seconds = 0;
     minutes++;
  }
  start = millis(); 
  user_input = analogRead(POT);
  user_input = 1023 - user_input;
  if(iron_state) {
    digitalWrite(IRON, LOW);
    temperature = analogRead(TEMP);
    digitalWrite(IRON, HIGH);
  } else {
    temperature = analogRead(TEMP);
  }

  if(user_input < 50) {
    point = 0;
    lock = 0;
  } else if (user_input > 800) {
    if(!lock) {
    point  = temperature;
    lock = 1;
   }
  } else {
    point = 1023;
    lock = 0;
  }



    lcd.setCursor(0,0);
    lcd.print(temperature);
    lcd.print(" ");
    lcd.print(minutes);
    lcd.print(":");
    lcd.print(seconds);
    lcd.print("     ");
    lcd.setCursor(0,1);
    lcd.print("        ");
    lcd.setCursor(0,1);
    lcd.print(point);
    lcd.print(" ");
    lcd.print(ms);
  }

  if(update_display == 255) {
    update_display = 1;
    delay(2000);
  } else {
    update_display++;
  }
  offset = point - temperature;

  if( offset > 0 && iron_state == 0) {
    digitalWrite(IRON, HIGH);
    iron_state = 1;
  } else if (iron_state == 1) {
    digitalWrite(IRON, LOW);
    iron_state = 0;
  }

 // Input = temperature;
 // if(Iron_PID.Compute()) {
 //   analogWrite(IRON,Output);
 // }

}

