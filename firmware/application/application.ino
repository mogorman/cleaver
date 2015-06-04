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
#include <EEPROM.h>
#include <LiquidCrystal_I2C_ST7032i.h>
//                ________   http://highlowtech.org pinout
//  Reset PB5     |1    8| Vcc+
//  A3    PB3 D3  |2    7| D2 PB2 A1 SCK SCL 
//  A2    PB4 D4  |3    6| D1 PB1 PWM MISO 
//        GND     |4    5| D0 PB0 PWM AREF MOSI SDA
//                --------


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
#define EEPROM_LENGTH 512


//UNDEFINE IF YOU WANT TO DISABLE AUTO SHUTOFF
#define SAFE_IRON 1
#define TIME_OUT 20000 //about sixty minutes
#define C 1 //celcius

#define SOLDER_MELT_TEMP 183 // temperature 60/40 solder melts at
#ifdef SAFE_IRON
#define MAX_TEMP 475
#endif

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

uint8_t room_temp;
uint8_t iron_room_temp;
uint8_t calibrated;
uint16_t solder_melt_temp;
int16_t user_input;
int16_t temperature;
float scale_factor;
//Specify the links and initial tuning parameters

LiquidCrystal_I2C_ST7032i lcd(0x3E,8,2);  // set the LCD address to 0x3E for a 8 chars and 2 line display

/* // the setup routine runs once when you press reset: */
void setup()
{
  int temp;
  //  int16_t temperature;
  //  delay(2000);
  pinMode(IRON,OUTPUT);
  pinMode(POT,INPUT);
  pinMode(TEMP,INPUT);
  digitalWrite(IRON,LOW);

  update_display = 0;
  position = 0;

  calibrated = 0;   //default uncalibrated value
  room_temp = 22;   //default uncalibrated value
  iron_room_temp = 110; //default uncalibrated value
  solder_melt_temp = 300; //default uncalibrated value
  
  scale_factor = (float)(SOLDER_MELT_TEMP - room_temp) / (float)(solder_melt_temp - iron_room_temp);
  
  temperature = analogRead(TEMP);
#ifdef SAFE_IRON
  user_input =  (((1023 - analogRead(POT)) - 50) * (float)( ( MAX_TEMP-room_temp)/(1023.0-100))) + room_temp;
#else
  user_input =  (((1023 - analogRead(POT)) - 50) * 1) + room_temp;
#endif
  knob_movement = user_input;
  check_eeprom(); 

  lcd.init();
  lcd.clear();
  lcd.setContrast(29);
  lcd.setCursor(0,0);

  //if the pot is at 0 and the iron is unplugged
  if(user_input < 50 && (temperature > 750 && temperature < 800 )) {
    lcd.print(GIT);
    lcd.setCursor(0,1);
    lcd.print(TIME);  
    delay(5000);
  } else if(user_input > MAX_TEMP && (temperature > 750 && temperature < 800 )) {
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
  
  stop = millis();
  ms += stop - start;
  if( ms > 100) {
    tenth_seconds += (ms/100);
    ms = ms % 100;
    update_display = 1;
  } 
  start = millis();
  if(update_display) {
    temperature = analogRead(TEMP);
#ifdef SAFE_IRON
    user_input =  (((1023 - analogRead(POT)) - 50) *  (float)( ( MAX_TEMP-room_temp)/(1023.0-100))) + 20;
#else
    user_input =  (((1023 - analogRead(POT)) - 50) * 1) + 20;    
#endif
    
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

    
    
    lcd.setCursor(0, 0);

    if(user_input < 20) {
      user_input = 0;
      lcd.print("OFF");
    }
#ifdef SAFE_IRON
    else if(user_input > MAX_TEMP) {
      user_input = MAX_TEMP;
      lcd.print("MAX");
    }
#endif
    else {
      lcd.print(user_input);
      if(user_input < 100) {
	lcd.print(" ");
      }
    }
    lcd.print("  ");
    temperature = normalize_temp();

    if(temperature < 100) {
      lcd.print(" ");
    }
    lcd.print(temperature); // dont update every time i get a reading just every 4 times
    //    lcd.print(readings[0]); // dont update every time i get a reading just every 4 times
    lcd.print(" ");
    //    lcd.print(tenth_seconds);
    update_display = 0;
    if((temperature - user_input) > 0) {
      digitalWrite(IRON, LOW);
    } else {
      digitalWrite(IRON, HIGH);
    }
  }

}

void plug_in_iron(int16_t temperature) {
  int16_t temp;
  update_display = 60;
  do {
    temp = temperature - analogRead(TEMP);
    if(update_display == 60) {
      lcd.setCursor(0,0);
      lcd.print("--PLUG-~");
      //lcd.print(temp);
      //delay(2000);
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
  int16_t temp;
  int16_t last_input;
  int16_t heat = 250;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Gauging");
  lcd.setCursor(0,1);
  lcd.print("the iron");
  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("set room");
  lcd.setCursor(0,1);
  lcd.print("temp and");
  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("thenPLUG");
  lcd.setCursor(0,1);
  lcd.print("iron in");
  delay(3000);

  do {

    temp = (1023 - analogRead(POT))/10 - 30;
    if(update_display == 60) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ROOMtemp");
      lcd.setCursor(0,1);
      lcd.print(temp);
      lcd.print("C");
      if(temp > 0) {
	lcd.print(" ");
      }
      lcd.print(((int16_t)(temp * 1.8) + 32) );
      lcd.print("F");
      update_display =0;
    }
    delay(10);
    update_display++;
  }
  while (analogRead(TEMP) > 750);
  room_temp = temp;
  delay(500);
  iron_room_temp = analogRead(TEMP);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("set");
  lcd.setCursor(0,1);
  lcd.print("solder");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("on tip");
  lcd.setCursor(0,1);
  lcd.print("of iron");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MOVEknob");
  lcd.setCursor(0,1);
  lcd.print("when");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("the iron");
  lcd.setCursor(0,1);
  lcd.print("melts");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("the");
  lcd.setCursor(0,1);
  lcd.print("solder");
  delay(3000);

  lcd.clear();
  last_input = 1023 - analogRead(POT);
  update_display = 12;
  do {
    temp = last_input - (1023 - analogRead(POT));
    if(update_display == 12) {
      //      lcd.clear();
      update_display = 0;
      lcd.setCursor(0,0);
      lcd.print(" Temp at");
      lcd.setCursor(0,1);
      lcd.print(analogRead(TEMP));
      lcd.print(" ");
      lcd.print(heat);
      heat+=1;

    }
    if((heat - analogRead(TEMP)) > 0) {
      digitalWrite(IRON, HIGH);
    } else {
      digitalWrite(IRON, LOW);
    }
    stop = millis();
    ms += stop - start;
    if( ms > 100) {
      tenth_seconds += (ms/100);
      ms = ms % 100;
      update_display++;
    } 
    start = millis();
    
    //    delay(10);
  }
  while (temp < 25);
  
  solder_melt_temp = heat;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("iron");
  lcd.setCursor(0,1);
  lcd.print("gauged");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(room_temp);
  lcd.setCursor(0,1);
  lcd.print(iron_room_temp);
  delay(3000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(183);
  lcd.setCursor(0,1);
  lcd.print(solder_melt_temp);
  delay(3000);

  write_eeprom();
}

void check_eeprom()
{
  int i;
  for(i =0; i < EEPROM_LENGTH - 4; i++)
  {
    if(EEPROM.read(i) != 42) {
      break;
    }
  }

  if(i == (EEPROM_LENGTH -4)) {
    calibrated = 1;
    iron_room_temp = EEPROM.read((EEPROM_LENGTH - 4));
    room_temp = EEPROM.read((EEPROM_LENGTH - 3));
    solder_melt_temp = ((EEPROM.read((EEPROM_LENGTH - 2)) << 0) & 0xFF) + ((EEPROM.read((EEPROM_LENGTH - 1)) << 8) & 0xFFFF);
  }
}

void write_eeprom()
{
  int i;
  for(i =0; i < EEPROM_LENGTH - 4; i++)
    {
      EEPROM.write(i,42);
    }
  EEPROM.write((EEPROM_LENGTH - 4), iron_room_temp);
  EEPROM.write((EEPROM_LENGTH - 3), room_temp);
  EEPROM.write((EEPROM_LENGTH - 2), (solder_melt_temp & 0xFF));
  EEPROM.write((EEPROM_LENGTH - 1), ((solder_melt_temp >> 8) & 0xFF));
  calibrated = 1;
  scale_factor = (float)(SOLDER_MELT_TEMP - room_temp) / (float)(solder_melt_temp - iron_room_temp);
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

uint16_t normalize_temp() { 
  // replace these constants with your 2 data points
  // these are sample values that will get you in the ballpark (in degrees C)
  float temp;
  int i;
  uint16_t average = 0;
  for (i = 0; i < BUFF_LENGTH; i++) {
    average = average + readings[i];
  }
  average = average/4;

  // now calculate the temperature
  temp = scale_factor * (int16_t)(average - iron_room_temp) + room_temp;

  return (uint16_t)temp;
}
