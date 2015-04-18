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

//Specify the links and initial tuning parameters
PID Iron_PID(&Input, &Output, &Set_point,2,5,1, DIRECT);
LiquidCrystal_I2C_ST7032i lcd(0x3E,8,2);  // set the LCD address to 0x3E for a 8 chars and 2 line display

// the setup routine runs once when you press reset:
void setup()
{
  //  analogReference( INTERNAL2V56_NO_CAP );
  pinMode(TEMP, INPUT);
  pinMode(POT, INPUT);
  pinMode(IRON, OUTPUT);
  digitalWrite(IRON, LOW);
  init_internal_sensor();
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

void loop()
{
  get_internal_temperature();
  Input = analogRead(TEMP);
  if(Iron_PID.Compute()) {
    analogWrite(IRON,Output);
  }
  Pot_value=analogRead(POT);
  Set_point = Pot_value;
  update_display();
}


void update_display() {
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(Pot_value);
  lcd.setCursor(1,1);
  lcd.print(Input);
}

void init_internal_sensor() {
  ADMUX = B1111;
  ADMUX &= ~_BV( ADLAR );
  ADMUX |= _BV( REFS1 );
  ADMUX &= ~( _BV( REFS0 ) | _BV( REFS2 ) );
  ADCSRA &= ~( _BV( ADATE ) |_BV( ADIE ) );
  ADCSRA |= _BV(ADEN);
  ADCSRA |= _BV(ADSC);
  int raw_temp;
  while( ( ( raw_temp = raw_internal_temperature() ) < 0 ) );
  for( int i = 0; i < 45; i++ ) {
    internal_readings[i] = raw_temp;
  }
}

int raw_internal_temperature() {
  if( ADCSRA & _BV( ADSC ) ) {
    return -1;
  } else {
    int ret = ADCL | ( ADCH << 8 );
    ADCSRA |= _BV(ADSC);
    return ret;
  }
}


int get_internal_temperature() {
  int readings_dup[45];
  int raw_temp;
  if( ( raw_temp = raw_internal_temperature() ) > 0 ) {
    internal_readings[internal_pos] = raw_temp;
    internal_pos++;
    internal_pos %= 45;
  }
  for( int i = 0; i < 45; i++ ) {
    readings_dup[i] = internal_readings[i];
  }
  int extremes_count = 9;
  int swap;
  for( int i = 0; i < extremes_count; ++i ) {
    for( int j = 0; j < 45 - 1; j++ ) {
      if( readings_dup[i] > readings_dup[i+1] ) {
	swap = readings_dup[i];
	readings_dup[i] = readings_dup[i+1];
	readings_dup[i+1] = swap;
      }
    }
  }
  int sum_temp = 0;
  for( int i = extremes_count; i < 45 - extremes_count; i++ ) {
    sum_temp += readings_dup[i];
  }
  return (sum_temp / ( 45 - extremes_count * 2 )) + internal_offset - 273;
}

