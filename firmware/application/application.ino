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

// 16 minute cool down from max to room temp
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
#define FARENHEIT 2
#define CELCIUS 1
#define UNDEFINED 0

#define BUFF_LENGTH 10
#define EEPROM_LENGTH 512


//UNDEFINE IF YOU WANT TO DISABLE AUTO SHUTOFF
#define SAFE_IRON 1
//#define TIME_OUT 20000 //about sixty minutes
//#define TIME_OUT 36000 //about sixty minutes
#define  TIME_OUT 8000

#define SOLDER_MELT_TEMP 183 // temperature 60/40 solder melts at
#ifdef SAFE_IRON
#define MAX_TEMP 475
#define MAX_RES 725
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




int16_t knob_movement;
uint16_t tenth_seconds;
//uint16_t on_time;
//Specify the links and initial tuning parameters

LiquidCrystal_I2C_ST7032i lcd(0x3E,8,2);  // set the LCD address to 0x3E for a 8 chars and 2 line display

/* // the setup routine runs once when you press reset: */
void setup()
{


  uint16_t ms;
  unsigned long start;
  unsigned long  stop;

  //  uint16_t readings[BUFF_LENGTH];
  //  uint8_t position;
  int16_t raw_reading;
  
  uint8_t room_temp;
  uint16_t iron_room_temp;
  uint8_t calibrated;
  uint16_t solder_melt_temp;
  int16_t user_input;
  int16_t temperature;
  int16_t scale_factor;
  uint8_t iron_state;
  uint8_t i;
  //  uint32_t average;

  pinMode(IRON,OUTPUT);
  pinMode(POT,INPUT);
  pinMode(TEMP,INPUT);
  digitalWrite(IRON,LOW);

  //  on_time = 0;
  //  position = 0;
  i = 0;
  calibrated = 0;   //default uncalibrated value
  room_temp = 22;   //default uncalibrated value
  iron_room_temp = 110; //default uncalibrated value
  solder_melt_temp = 300; //default uncalibrated value
  if (check_eeprom()) {
    calibrated = EEPROM.read((EEPROM_LENGTH -6));;
    iron_room_temp =  ((EEPROM.read((EEPROM_LENGTH - 5)) << 0) & 0xFF) + ((EEPROM.read((EEPROM_LENGTH - 4)) << 8) & 0xFFFF);
    room_temp = EEPROM.read((EEPROM_LENGTH - 3));
    solder_melt_temp = ((EEPROM.read((EEPROM_LENGTH - 2)) << 0) & 0xFF) + ((EEPROM.read((EEPROM_LENGTH - 1)) << 8) & 0xFFFF);
  }
  scale_factor = ((SOLDER_MELT_TEMP - room_temp)*100) / (solder_melt_temp - iron_room_temp);
  iron_state = 0;
  
  temperature = analogRead(TEMP);
  user_input = 1023 - analogRead(POT);
  knob_movement = user_input;
#ifdef SAFE_IRON
  user_input =  ((user_input - 50) * (double)( ( MAX_TEMP-room_temp)/(1023.0-100))) + room_temp;
#else
  user_input =  ((user_input - 50) * 1) + room_temp;
#endif


  lcd.init();
  lcd.clear();
  lcd.setContrast(29);
  lcd.setCursor(0,0);
  //if the pot is at 0 and the iron is unplugged
  if(user_input < 50 && (temperature > 750 && temperature < 800 )) {
    lcd.print(GIT);
    lcd.setCursor(0,1);
    lcd.print(TIME);
    delay(2500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(calibrated);
    lcd.print(" ");
    lcd.print(room_temp);
    lcd.print(" ");
    lcd.print(iron_room_temp);
    lcd.setCursor(0,1);
    lcd.print(solder_melt_temp);
    lcd.print(" ");
    lcd.print(SOLDER_MELT_TEMP);
    delay(2500);
  } else if(user_input > MAX_TEMP && (temperature > 750 && temperature < 800 )) {
    initialize();
    if (check_eeprom()) {
      calibrated = EEPROM.read((EEPROM_LENGTH -6));;
      iron_room_temp =  ((EEPROM.read((EEPROM_LENGTH - 5)) << 0) & 0xFF) + ((EEPROM.read((EEPROM_LENGTH - 4)) << 8) & 0xFFFF);
      room_temp = EEPROM.read((EEPROM_LENGTH - 3));
      solder_melt_temp = ((EEPROM.read((EEPROM_LENGTH - 2)) << 0) & 0xFF) + ((EEPROM.read((EEPROM_LENGTH - 1)) << 8) & 0xFFFF);
    }
  }
  if (temperature > 750 && temperature < 800 ) {
    plug_in_iron(temperature);
  }
  scale_factor = ((SOLDER_MELT_TEMP - room_temp)*100) / (solder_melt_temp - iron_room_temp);
  lcd.command(0x34); //one column mode
  start = 0;
  ms = 0;
  tenth_seconds = 0;

  while(1) {
    stop = millis();
    ms += stop - start;
    if( ms > 100) {
      tenth_seconds += (ms/100);
      //      on_time += (ms/100);
      ms = ms % 100;
    }
    start = millis();
    temperature = analogRead(TEMP);
    raw_reading = temperature;
    //    readings[position] = temperature;
    
    //    for (i = 0; i < BUFF_LENGTH; i++) {
    //      average = average + readings[i];
    //    }
    //    average = average/BUFF_LENGTH;
    
    //    average = normalize_temp(average, iron_room_temp, room_temp, scale_factor);
    //    position++;
    //    if(position == BUFF_LENGTH) {
    //      position = 0;
    //    }
    iron_state = main_loop(iron_state, calibrated, room_temp, raw_reading, scale_factor, iron_room_temp);
  }
}

uint8_t  main_loop(const uint8_t iron_state, const uint8_t calibrated, const uint8_t room_temp, const int16_t raw_reading, const int16_t scale_factor, const uint16_t iron_room_temp)
{
  int16_t user_input;
  int16_t temperature;
  user_input = 1023 - analogRead(POT);

#ifdef SAFE_IRON
  if((knob_movement - user_input) > 25 || (knob_movement - user_input) < -25  ) {
    knob_movement = user_input;
    tenth_seconds = 0;
  }
  if (tenth_seconds > TIME_OUT ) {
    digitalWrite(IRON, LOW);
    lcd.clear();
    lcd.command(0x38); //two row mode
    time_out(user_input);
    knob_movement = user_input;
    lcd.command(0x34); //one row mode
    tenth_seconds = 0;
    return 0;
  }
#endif
  
  temperature = normalize_temp(raw_reading, iron_room_temp, room_temp, scale_factor);
  
#ifdef SAFE_IRON
  user_input =  ((user_input - 50) *  (double)( ( MAX_TEMP-room_temp)/(1023.0-100))) + room_temp;
  if(user_input > MAX_TEMP) {
    user_input = MAX_TEMP;
  } else if (user_input < room_temp) {
    user_input = 0;
  }
#else
  user_input =  ((user_input - 50) * 1) + room_temp;
  if(user_input < room_temp) {
    user_input = 0;
  }
#endif
  //  if(on_time) {
  main_readout(calibrated, user_input, temperature, room_temp);
    //  }


  if(raw_reading > 750 && raw_reading < 800) {
    lcd.clear();
    lcd.command(0x38); //two row mode
    digitalWrite(IRON, LOW);
    plug_in_iron(raw_reading);
    lcd.clear();
    lcd.command(0x34); //one row mode
    return 0;
  }

  if(((temperature >  user_input) && iron_state) || raw_reading > MAX_RES) {
    digitalWrite(IRON, LOW);
    return 0;
  } else if (iron_state == 0 && (user_input > temperature) && raw_reading < MAX_RES ) {
    digitalWrite(IRON, HIGH);
    return 1;
  }
  return iron_state;
}

void loop()
{
  //never reach here.
} 

void plug_in_iron(int16_t temperature) {
  int16_t temp;
  uint8_t update_display;
  update_display = 60;
  do {
    temp = temperature - analogRead(TEMP);
    if(update_display == 60) {
      lcd.setCursor(0,0);
      lcd.print("--PLUG-~");
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
}


void time_out(int16_t last_input) {
  int16_t temp;
  uint8_t update_display;
  
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
  while (temp < 25 && temp > -25);
}

void initialize()
{
  int16_t temp;
  int16_t last_input;
  int16_t heat = 200;
  uint8_t update_display;
  uint16_t ms;
  unsigned long start;
  unsigned long  stop;
  uint8_t room_temp;
  uint16_t iron_room_temp;
  uint8_t calibrated;
  uint16_t solder_melt_temp;
  tenth_seconds = 0;
  ms = 0;
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
  update_display = 60;
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
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("waiting");
  lcd.setCursor(0,1);
  lcd.print("on iron");
  delay(3000);
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
    if(heat > 500) {
      heat = 100;
    }
    stop = millis();
    ms += stop - start;
    if( ms > 100) {
      tenth_seconds += (ms/100);
      ms = ms % 100;
      update_display++;
    } 
    start = millis();
  }
  while (temp < 25 && temp > -25);
  digitalWrite(IRON,LOW);
  solder_melt_temp = heat;
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("iron");
  lcd.setCursor(0,1);
  lcd.print("gauged");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("turn ~ F");
  lcd.setCursor(0,1);
  lcd.print("Turn ");
  lcd.write(0x7F);
  lcd.print(" C");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("then");
  lcd.setCursor(0,1);
  lcd.print("unplug");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("iron to");
  lcd.setCursor(0,1);
  lcd.print("select");
  delay(3000);
  do {
    temp = analogRead(TEMP);
  }
  while (!(temp > 750 && temp < 800));
  temp = analogRead(POT);
  if (temp > 500) {
    calibrated = CELCIUS;
  } else if(temp < 500) {
    calibrated = FARENHEIT;
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(room_temp);
  lcd.setCursor(0,1);
  lcd.print(iron_room_temp);
  lcd.print(" ");
  lcd.print(calibrated);
  delay(3000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(SOLDER_MELT_TEMP);
  lcd.setCursor(0,1);
  lcd.print(solder_melt_temp);
  delay(3000);
  write_eeprom(calibrated, iron_room_temp, room_temp, solder_melt_temp);
}

uint8_t check_eeprom()
{
  int i;
  for(i =0; i < EEPROM_LENGTH - 6; i++)
    {
      if(EEPROM.read(i) != 42) {
	break;
      }
    }

  if(i == (EEPROM_LENGTH -5)) {
    return 1;
  }
  return 0;
}

void write_eeprom(const uint8_t calibrated, const uint16_t iron_room_temp, const uint8_t room_temp, const uint16_t solder_melt_temp)
{
  int i;
  for(i =0; i < EEPROM_LENGTH - 6; i++)
    {
      EEPROM.write(i,42);
    }
  EEPROM.write((EEPROM_LENGTH - 6), calibrated);
  EEPROM.write((EEPROM_LENGTH - 5), (iron_room_temp & 0xFF));
  EEPROM.write((EEPROM_LENGTH - 4), ((iron_room_temp >> 8) & 0xFF));
  EEPROM.write((EEPROM_LENGTH - 3), room_temp);
  EEPROM.write((EEPROM_LENGTH - 2), (solder_melt_temp & 0xFF));
  EEPROM.write((EEPROM_LENGTH - 1), ((solder_melt_temp >> 8) & 0xFF));
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

uint16_t normalize_temp(uint16_t average, const uint16_t iron_room_temp, const uint8_t room_temp, const int16_t scale_factor) { 
  // replace these constants with your 2 data points
  // these are sample values that will get you in the ballpark (in degrees C)
  double temp;
  int i;

  // now calculate the temperature
  temp = (scale_factor * (int16_t)(average - iron_room_temp) / 100) + room_temp;

  return (uint16_t)temp;
}



void main_readout(const int8_t type_of_degree, int16_t goal, const int16_t current_temp, const uint8_t room_temp)
{
  int16_t temp_in_f;
  uint8_t i;
  i = 5;

  lcd.setCursor(0,0);
  //  lcd.print(tenth_seconds);
  if(goal < room_temp) {
    lcd.print("OFF ");
    i--;
    goal = 0;
  }
#ifdef SAFE_IRON
  else if(goal >= MAX_TEMP) {
    lcd.print("MAX ");
    i--;
    goal = MAX_TEMP;
  }
#endif
  else {
    if(type_of_degree ==  FARENHEIT) { // display in f..
      temp_in_f = ( (int16_t) (goal * 1.8) + 32);
      lcd.print(temp_in_f);
      lcd.print("f ");
      if(temp_in_f < 100) {
	i--;
      } 
    } else {
      lcd.print(goal);
      lcd.print("c ");
      if(goal < 100) {
	i--;
      } 
    }
  }
  
  if(type_of_degree ==  FARENHEIT) {
    temp_in_f = ( (int16_t) (current_temp * 1.8) + 32);
    if(temp_in_f < 100 && i < 5) {
      i++;
      if(temp_in_f < 10) {
	i++;
      }
    } else if (temp_in_f < 10) {
      i++;
    }
    lcd.setCursor(i-1,0);
    lcd.print(" ");
    lcd.print(temp_in_f);
    lcd.print("f");
  } else {
    if(current_temp < 100 && i < 5) {
      i++;
      if (current_temp <10) {
	i++;
      }
    } else if (current_temp < 10) {
      i++;
    }
    lcd.setCursor(i-1,0);
    lcd.print(" ");
    lcd.print(current_temp);
    lcd.print("c");
  }
  return;
}
