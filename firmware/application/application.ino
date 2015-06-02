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
//1134

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
#define BUFF_LENGTH 4

//UNDEFINE IF YOU WANT TO DISABLE AUTO SHUTOFF
#define SAFE_IRON 1
#define MAX_TEMP 750
#define TIME_OUT 20000 //about sixty minutes
//#define TIME_OUT 100 //about sixty minutes

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
uint8_t update_display;

uint16_t knob_movement;
uint16_t tenth_seconds;
uint16_t ms;
unsigned long start;
unsigned long  stop;
uint16_t readings[BUFF_LENGTH];
uint8_t position;
//Specify the links and initial tuning parameters

LiquidCrystal_I2C_ST7032i lcd(0x3E,8,2);  // set the LCD address to 0x3E for a 8 chars and 2 line display

/* // the setup routine runs once when you press reset: */
void setup()
{
  int temp;
  int16_t user_input;
  int16_t temperature;
  //  delay(2000);
  pinMode(IRON,OUTPUT);
  pinMode(POT,INPUT);
  pinMode(TEMP,INPUT);
  digitalWrite(IRON,LOW);

  update_display = 0;
  position = 0;

  temperature = analogRead(TEMP);
  user_input = analogRead(POT);
  user_input = 1023 - user_input;
  knob_movement = user_input;
  
  lcd.init();
  lcd.clear();
  lcd.setContrast(29);
  //if the pot is at 0 and the iron is unplugged
  if(user_input < 50 && (temperature > 750 && temperature < 800 )) {
    lcd.setCursor(0,0);
    lcd.print(GIT);
    lcd.setCursor(0,1);
    lcd.print(TIME);  
    delay(5000);
  } else if(user_input > 1000 && (temperature > 750 && temperature < 800 )) {
    initialize();
  }
  if (temperature > 750 && temperature < 800 ) {
    plug_in_iron(temperature);
  }
  lcd.command(0x34); //one column mode
  start = 0;
  ms = 0;
  //  minutes = 0;
  tenth_seconds = 0;
}

void loop()
{
 
  uint8_t i;
  int16_t user_input;
  int16_t temperature;
  stop = millis();
  ms += stop - start;
  if( ms > 100) {
    tenth_seconds += (ms/100);
    ms = ms % 100;
    update_display = 1;
  } 
  //  if(tenth_seconds == 600) {
  //    tenth_seconds = 0;
    //    minutes++;
  //  }
  start = millis();

  if(update_display) {
    temperature = analogRead(TEMP);

    user_input = analogRead(POT);
    user_input = 1023 - user_input;

    
#ifdef SAFE_IRON
  if (tenth_seconds > TIME_OUT ) {
    digitalWrite(IRON, LOW);
    lcd.clear();
    lcd.command(0x38); //two row mode
    time_out(user_input);
    knob_movement = user_input;
    lcd.command(0x34); //one row mode
    tenth_seconds = 0;
  }
#endif
  
    if((knob_movement - user_input) > 50) {
      knob_movement = user_input;
      tenth_seconds = 0;
    }
#ifdef SAFE_IRON
    if(user_input > MAX_TEMP)
      user_input = MAX_TEMP;
#endif
    if(user_input < 50)
      user_input = 0;


  readings[position] = temperature;
    position++;
    if(position == BUFF_LENGTH)
      position = 0;
    for(i = 0; i < BUFF_LENGTH; i++) {
      if (readings[i] > 750 && readings[i] < 800) {
	continue;
      } else {
	break;
      }
    }
    if(i == BUFF_LENGTH) {
      lcd.clear();
      lcd.command(0x38); //two row mode
      digitalWrite(IRON, LOW);
      plug_in_iron(temperature);
      lcd.clear();
      lcd.command(0x34); //one row mode
    }
    if((temperature - user_input) > 0) {
      update_display = 4;
      digitalWrite(IRON, LOW);
    } else {
      update_display = 5;
      digitalWrite(IRON, HIGH);
    }
    
    
    lcd.setCursor(0, 0);
    if(!user_input) {
      lcd.print("OFF");
#ifdef SAFE_IRON
    } else if(user_input == MAX_TEMP) {
      lcd.print("MAX");
#endif
    } else {
      lcd.print(user_input);
      if(user_input < 100) {
	lcd.print(" ");
      }
    }
    lcd.print("  ");
    lcd.print(readings[0]); // dont update every time i get a reading just every 4 times
    lcd.print(tenth_seconds);
    update_display = 0;
  }
}

void plug_in_iron(int16_t temperature) {
  uint16_t temp;
  update_display = 60;
  do {
    temp = temperature - analogRead(TEMP);
    if(update_display == 60) {
      lcd.setCursor(0,0);
      lcd.print("--PLUG-~");
      //      lcd.print(analogRead(TEMP));
      lcd.setCursor(0,1);
      lcd.print("IRON IN!");
    } else if (update_display == 120) {
      lcd.clear();
      update_display = 0;
    }
    delay(10);
    update_display++;
  }
  while (temp < 25);
  //  minutes = 0;
  tenth_seconds = 0;
}


void time_out(uint16_t last_input) {
  uint16_t temp;
  update_display = 60;
  do {
    temp = last_input - (1023 - analogRead(POT));
    if(update_display == 60) {
      lcd.setCursor(0,0);
      lcd.print("  Auto  ");
      lcd.setCursor(0,1);
      lcd.print("Shutoff");
    } else if (update_display == 120) {
      lcd.clear();
    } else if (update_display == 180) {
      lcd.setCursor(0,0);
      lcd.print("  Move  ");
      lcd.setCursor(0,1);
      lcd.print("  Knob  ");
    } else if (update_display == 240) {
      lcd.clear();
      update_display = 0;
    }
    delay(10);
    update_display++;
  }
  while (temp < 25);
}

void initialize()
{
  lcd.setCursor(0,0);
  lcd.print(" config");
  lcd.setCursor(0,1);
  lcd.print("the iron");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("set knob");
  lcd.setCursor(0,1);
  
  lcd.setCursor(0,0);
  lcd.print(analogRead(TEMP));
  lcd.setCursor(0,1);
  delay(1000);
  while(1) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(analogRead(TEMP));
    lcd.setCursor(0,1);
    //    lcd.print(read_temp());
    delay(200);
  }
  while(1);
}

// i don't think this is very useful after all
// long read_temp() { 
//   // Read temperature sensor against 1.1V reference
//   byte ADMUX_copy;
//   ADMUX_copy = ADMUX;
//   ADMUX = _BV(REFS1) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);
//   delay(2); // Wait for ADMUX setting to settle
//   ADCSRA |= _BV(ADSC); // Start conversion
//   while (bit_is_set(ADCSRA,ADSC)); // measuring

//   uint8_t low = ADCL; // must read ADCL first - it then locks ADCH
//   uint8_t high = ADCH; // unlocks both
//   long result = (high << 8) | low; // combine the two
//   ADMUX = ADMUX_copy;
//   return result;
// }

// float normalizeTemperature(long rawData) { 
//   // replace these constants with your 2 data points
//   // these are sample values that will get you in the ballpark (in degrees C)
//   float temp1 = 0;
//   long data1 = 274;
//   float temp2 = 25.0;
//   long data2 = 304;
 
//   // calculate the scale factor
//   float scaleFactor = (temp2 - temp1) / (data2 - data1);

//   // now calculate the temperature
//   float temp = scaleFactor * (rawData - data1) + temp1;

//   return temp;
// }

// //temp readings
// //107 ~= 22.8c 73F

// //776,777,778 when unplugged

