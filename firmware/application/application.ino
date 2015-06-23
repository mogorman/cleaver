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
#define DEBUG 1

#define FARENHEIT 2
#define CELCIUS 1
#define UNDEFINED 0

#define BUFF_LENGTH 10
#define EEPROM_LENGTH 512


//UNDEFINE IF YOU WANT TO DISABLE AUTO SHUTOFF
#define SAFE_IRON 1
//#define TIME_OUT 36000 //about sixty minutes
#define  TIME_OUT 8000

#define SOLDER_MELT_TEMP 183 // temperature 60/40 solder melts at
#ifdef SAFE_IRON
#define MAX_TEMP 475
#define MAX_RES 725
//  int32_t input_scale = ((int32_t)(MAX_TEMP - room_temp) * 100)/(1023-100);
#define INPUT_SCALE 49
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
int16_t my_update_display;
uint16_t user_input;

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
  int32_t temperature;
  int16_t scale_factor;
  uint8_t iron_state;
  uint8_t i;


  
  pinMode(IRON,OUTPUT);
  pinMode(POT,INPUT);
  pinMode(TEMP,INPUT);
  digitalWrite(IRON,LOW);

  //  on_time = 0;
  //  position = 0;
  i = 0;
  my_update_display = 0;
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

  lcd.init();
  lcd.clear();
  lcd.setContrast(29);
  lcd.setCursor(0,0);
  //  lcd.print( ((MAX_TEMP - room_temp) *100) / (1023-100));
  // lcd.print(scale_factor);
  // lcd.setCursor(0,1);
  // //  lcd.print( ( (user_input - 50) * ((int32_t)(MAX_TEMP - room_temp) * 100)/(1023-100) ) /100 + room_temp );
  // lcd.print(user_input);
  // //  lcd.print((1023 - analogRead(POT)) -50);
  // while(1);
  //if the pot is at 0 and the iron is unplugged
  if(user_input < 50 && (temperature > 750 && temperature < 800 )) {
    lcd.print(GIT);
    lcd.setCursor(0,1);
    lcd.print(TIME);
    blocking_delay(2500);
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
    blocking_delay(2500);
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
#if DEBUG
    iron_state = debug_loop(iron_state, raw_reading);
#else
    iron_state = main_loop(iron_state, calibrated, room_temp, raw_reading, scale_factor, iron_room_temp);
#endif
  }
}
#if DEBUG
uint8_t debug_loop(const uint8_t iron_state, const int16_t raw_reading)
{
  return 0;
}
#endif
#if !DEBUG
uint8_t  main_loop(const uint8_t iron_state, const uint8_t calibrated, const uint8_t room_temp, const int16_t raw_reading, const int16_t scale_factor, const uint16_t iron_room_temp)
{
  int32_t temperature;
  uint16_t local_user_input;
  
  my_update_display++;
  
  if(my_update_display == 700) {
    user_input = 1023 - analogRead(POT);

  }

  local_user_input = user_input;
  
#ifdef SAFE_IRON
  if((knob_movement - local_user_input) > 25 || (knob_movement - (int16_t)local_user_input) < -25  ) {
    knob_movement = local_user_input;
    tenth_seconds = 0;
  }

  if (tenth_seconds > TIME_OUT ) {
    digitalWrite(IRON, LOW);
    lcd.clear();
    lcd.command(0x38); //two row mode
    time_out(local_user_input);
    knob_movement = local_user_input;
    lcd.command(0x34); //one row mode
    tenth_seconds = 0;
    return 0;
  }
#endif
  
  temperature = normalize_temp(raw_reading, iron_room_temp, room_temp, scale_factor);
  if (local_user_input < 50) {
    local_user_input = 0;
  } else {
    local_user_input = local_user_input - 50;
  }
#ifdef SAFE_IRON

  local_user_input =  (local_user_input * INPUT_SCALE)/100;
  if(local_user_input > MAX_TEMP) {
    local_user_input = MAX_TEMP;
  } else if (local_user_input <= room_temp && user_input < 50) {
    local_user_input = 0;
  } else {
    local_user_input = local_user_input + room_temp;
  }
#else
  local_user_input = local_user_input;
  if(local_user_input <= room_temp) {
    local_user_input = 0;
  } else {
    local_user_input = local_user_input + room_temp;
  }
#endif
  if(my_update_display == 700) {
    main_readout(calibrated, local_user_input, temperature, room_temp);
    my_update_display = 0;
  }


  if(raw_reading > 750 && raw_reading < 800) {
    lcd.clear();
    lcd.command(0x38); //two row mode
    digitalWrite(IRON, LOW);
    plug_in_iron(raw_reading);
    lcd.clear();
    lcd.command(0x34); //one row mode
    return 0;
  }
  
  if(((temperature >  local_user_input) && iron_state) || raw_reading > MAX_RES) {
    digitalWrite(IRON, LOW);
    return 0;
  } else if (iron_state == 0 && (local_user_input > temperature) && raw_reading < MAX_RES ) {
    digitalWrite(IRON, HIGH);
    return 1;
  }
  return iron_state;
}
#endif

void loop()
{
  //never reach here.
} 

void plug_in_iron(int32_t temperature) {
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
    blocking_delay(10);
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
    blocking_delay(10);
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
  blocking_delay(3000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("set room");
  lcd.setCursor(0,1);
  lcd.print("temp and");
  blocking_delay(3000);  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("thenPLUG");
  lcd.setCursor(0,1);
  lcd.print("iron in");
  blocking_delay(3000);
  update_display = 60;
  do {

    temp = (1023 - analogRead(POT))/10 - 50;
    if(update_display == 60) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ROOMtemp");
      lcd.setCursor(0,1);
      lcd.print(((int16_t)(temp * 1.8) + 32) );
      lcd.print("F");
      if(temp > 0) {
	lcd.print(" ");
      }
      lcd.print(temp);
      lcd.print("C");
      update_display =0;
    }
    blocking_delay(10);
    update_display++;
  }
  while (analogRead(TEMP) > 750);
  room_temp = temp;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("waiting");
  lcd.setCursor(0,1);
  lcd.print("on iron");
  blocking_delay(3000);
  iron_room_temp = analogRead(TEMP);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("set");
  lcd.setCursor(0,1);
  lcd.print("solder");
  blocking_delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("on tip");
  lcd.setCursor(0,1);
  lcd.print("of iron");
  blocking_delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MOVEknob");
  lcd.setCursor(0,1);
  lcd.print("when");
  blocking_delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("the iron");
  lcd.setCursor(0,1);
  lcd.print("melts");
  blocking_delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("the");
  lcd.setCursor(0,1);
  lcd.print("solder");
  blocking_delay(3000);

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
  blocking_delay(3000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("turn ~ F");
  lcd.setCursor(0,1);
  lcd.print("Turn ");
  lcd.write(0x7F);
  lcd.print(" C");
  blocking_delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("then");
  lcd.setCursor(0,1);
  lcd.print("unplug");
  blocking_delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("iron to");
  lcd.setCursor(0,1);
  lcd.print("select");
  blocking_delay(3000);
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
  blocking_delay(3000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(SOLDER_MELT_TEMP);
  lcd.setCursor(0,1);
  lcd.print(solder_melt_temp);
  blocking_delay(3000);
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

uint32_t normalize_temp(uint32_t average, const uint16_t iron_room_temp, const uint8_t room_temp, const int16_t scale_factor) { 
  // replace these constants with your 2 data points
  // these are sample values that will get you in the ballpark (in degrees C)
  uint32_t temp;
  int i;

  // now calculate the temperature
  temp = (scale_factor * (int32_t)(average - iron_room_temp) / 100) + room_temp;

  return temp;
}



void main_readout(const int8_t type_of_degree, int16_t goal, const int16_t current_temp, const uint8_t room_temp)
{
  int16_t temp_in_f;
  uint8_t i;
  i = 5;

  lcd.setCursor(0,0);
  //  lcd.print(tenth_seconds);
  if(goal <= room_temp) {
    lcd.print("OFF ");
    tenth_seconds = 0;
    i--;
    goal = 0;
  }
#ifdef SAFE_IRON
  else if(goal >= MAX_TEMP) {
    lcd.print("MAX ");
    //    lcd.print(tenth_seconds);
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


void blocking_delay(const uint16_t ms_to_delay)
{
  uint16_t ms;
  unsigned long start;
  unsigned long  stop;
  ms = 0;
  stop = millis();
  while(1) {
    start = millis();
    ms += start - stop;
    if( ms > ms_to_delay) {
      return;
    }
    stop = millis();
  }
}
