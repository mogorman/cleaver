// LiquidCrystal_I2C_ST7032i V1.0 for Arduino

#include "LiquidCrystal_I2C_ST7032i.h"
#include <inttypes.h>

#include "TinyWireM.h"

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).


// Used by backlight control functions to detect if a pin is PWM capable.
// For other AVR types similar arrays need to be added.

uint8_t  pwm[4]={2,3,5,6};


LiquidCrystal_I2C_ST7032i::LiquidCrystal_I2C_ST7032i(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows){
  _Addr = lcd_Addr;
  _cols = lcd_cols;
  _rows = lcd_rows;
  _pwm = false;
}

LiquidCrystal_I2C_ST7032i::LiquidCrystal_I2C_ST7032i(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows, uint8_t lcd_backlightpin){
  _Addr = lcd_Addr;
  _cols = lcd_cols;
  _rows = lcd_rows;
  _backlightpin = lcd_backlightpin;
  #define BACKLIGHT_PIN _backlightpin
  _pwm = false;
}

void LiquidCrystal_I2C_ST7032i::init(){
  _display_basic = LCD_INSTRUCTION_SET_BASIC | LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
  _display_extended = LCD_INSTRUCTION_SET_EXTENDED | LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
  
  #if defined BACKLIGHT_PIN  // Is the selected backlightpin PWM capable?
    for (int i=0; i < (sizeof(pwm) / sizeof(byte)); i++){
      if (pwm[i] == BACKLIGHT_PIN){
        _pwm = true;
      }
    }
    if(_pwm){
      _backlightval = 255;
    }
    pinMode(BACKLIGHT_PIN, OUTPUT);
  #else
    _backlightval = HIGH;
  #endif
  init_priv();
}

void LiquidCrystal_I2C_ST7032i::init_priv(){
    TinyWireM.begin();             			// This has not neen tested
  
  _displaycontrol = LCD_DISPLAYON;

  begin(_cols, _rows,LCD_5x8DOTS);
 
}

void LiquidCrystal_I2C_ST7032i::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  byte contrast = 0x0A;
  
  delay(40);
  
  #if defined BACKLIGHT_PIN
      backlight();
  #endif

  if (lines > 1) {
    _display_basic |= LCD_2LINE;
    _display_extended |= LCD_2LINE;
  }
  _numlines = lines;
  
  
// function set  basic
  command(LCD_FUNCTIONSET | _display_basic );
  delayMicroseconds(30);

// function set extended
  command(LCD_FUNCTIONSET | _display_extended);
  delayMicroseconds(30);
  
// interval osc  
  command(LCD_BIAS_OSC_CONTROL | LCD_BIAS1_5 | LCD_OSC_192);
  delayMicroseconds(30);
  
// contrast low nible 
  command(LCD_CONTRAST_LOW_BYTE | (contrast & LCD_CONTRAST_LOW_BYTE_MASK));
  delayMicroseconds(30);

// contrast high nible / icon / power
  command(LCD_ICON_CONTRAST_HIGH_BYTE | LCD_ICON_ON | LCD_BOOSTER_ON | (contrast >> 4 & LCD_CONTRAST_HIGH_BYTE_MASK));
  delayMicroseconds(30);
  
// follower control  
  _rab = LCD_Rab_2_00;
  command(LCD_FOLLOWER_CONTROL | LCD_FOLLOWER_ON | _rab);
  delay(200);
  
// function set basic
  command(LCD_FUNCTIONSET | _display_basic);
  delayMicroseconds(30);
  
// display on 
  command(LCD_DISPLAYCONTROL |  LCD_DISPLAYON |  LCD_CURSOROFF | LCD_BLINKOFF );
  delayMicroseconds(30);

// entry mode set
  _displaymode=LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
  delayMicroseconds(30);

// clear display
  command(LCD_CLEARDISPLAY);
  delay(2);

  home();
}

// ************* high level commands, for the user! *************
void LiquidCrystal_I2C_ST7032i::clear(){
  command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
  delay(2);  // this command takes a long time!
}

void LiquidCrystal_I2C_ST7032i::home(){
  command(LCD_RETURNHOME);  // set cursor position to zero
  delay(2);  // this command takes a long time!
}

void LiquidCrystal_I2C_ST7032i::setCursor(uint8_t col, uint8_t row){
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row > _numlines ) {
    row = _numlines-1;    // we count rows starting w/0
  }
  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
  delayMicroseconds(30);
}

// Turn the display on/off
void LiquidCrystal_I2C_ST7032i::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
  delayMicroseconds(30);
}

void LiquidCrystal_I2C_ST7032i::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
  delayMicroseconds(30);
}

// Turns the underline cursor on/off
void LiquidCrystal_I2C_ST7032i::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
  delayMicroseconds(30);
}

void LiquidCrystal_I2C_ST7032i::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
  delayMicroseconds(30);
}

// Turn cursor blinking on/off 
void LiquidCrystal_I2C_ST7032i::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
  delayMicroseconds(30);
}

void LiquidCrystal_I2C_ST7032i::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
  delayMicroseconds(30);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_I2C_ST7032i::scrollDisplayLeft(void) {
  command(LCD_FUNCTIONSET | _display_basic);
  delayMicroseconds(30);
  
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
  delayMicroseconds(30);
}

void LiquidCrystal_I2C_ST7032i::scrollDisplayRight(void) {
  command(LCD_FUNCTIONSET | _display_basic);
  delayMicroseconds(30);
  
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
  delayMicroseconds(30);
}

// This is for text that flows Left to Right
void LiquidCrystal_I2C_ST7032i::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
  delayMicroseconds(30);
}

// This is for text that flows Right to Left
void LiquidCrystal_I2C_ST7032i::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
  delayMicroseconds(30);
}

// This will 'right justify' text from the cursor
void LiquidCrystal_I2C_ST7032i::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
  delayMicroseconds(30);
}

// This will 'left justify' text from the cursor
void LiquidCrystal_I2C_ST7032i::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
  delayMicroseconds(30);
}

// Allows us to fill the first 8 CGRAM locations with custom characters
void LiquidCrystal_I2C_ST7032i::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  delayMicroseconds(30);
  
  for (int i=0; i<8; i++) {
    write(charmap[i]);
    delayMicroseconds(30);
  }
}

// Set contast level
// The range of 'new_val' depends on the harware configuration of the ST7032i and depends on the follower value, Booster value, Vin and VDD.
// In case of the YMFC-G0802D the follower value is 0x04. According to the info in the ST7032 datasheet this gives us a range of 0 to 10 for the contrast.
// The setting in this library are desigend for and tesed on display YMFC-G0802D ( sold on eBay zyscom http://stores.ebay.com/zyscom?_trksid=p4340.l2563 )
// For other displays other values and ranges may apply.
void LiquidCrystal_I2C_ST7032i::setContrast(uint8_t new_val){
  command(LCD_FUNCTIONSET | _display_extended);					
  delayMicroseconds(30);	//																								Needed ??

  command(LCD_ICON_CONTRAST_HIGH_BYTE | LCD_ICON_ON | LCD_BOOSTER_ON | (new_val >> 4 & LCD_CONTRAST_HIGH_BYTE_MASK));
  delayMicroseconds(30);

  command(LCD_CONTRAST_LOW_BYTE | (new_val & LCD_CONTRAST_LOW_BYTE_MASK));
  delayMicroseconds(30);
}



// Turn the (optional) backlight off/on
void LiquidCrystal_I2C_ST7032i::backlight(void) {
  #if defined BACKLIGHT_PIN
    if(_pwm){
      _backlightval=255;
      analogWrite(BACKLIGHT_PIN,_backlightval);
    }else{
      digitalWrite(BACKLIGHT_PIN, HIGH);
    }
  #endif
}

void LiquidCrystal_I2C_ST7032i::noBacklight(void) {
  #if defined BACKLIGHT_PIN
      digitalWrite(BACKLIGHT_PIN, LOW);
  #endif
}

// Set the backlight intensity. Only available when the backlight pin is PWM capable
void LiquidCrystal_I2C_ST7032i::backlight(uint8_t new_val) {
  #if defined BACKLIGHT_PIN
    if(_pwm){
      _backlightval=new_val;
      analogWrite(BACKLIGHT_PIN,_backlightval);
    }else{
      digitalWrite(BACKLIGHT_PIN, HIGH);
    }
  #endif
}


// ************* mid level commands, for sending data/cmds *************

inline void LiquidCrystal_I2C_ST7032i::command(uint8_t value) {
  send(value, 0x00);
}

#if defined(ARDUINO) && ARDUINO >= 100
  inline size_t LiquidCrystal_I2C_ST7032i::write(uint8_t value) {
    send(value, 0x40);
    return 0;
  }
#else
  inline void LiquidCrystal_I2C_ST7032i::write(uint8_t value) {  // This has not neen tested
    send(value, 0x40);
  } 
#endif



// ************* low level data pushing commands *************

// write either command or data
void LiquidCrystal_I2C_ST7032i::send(uint8_t value, uint8_t mode) {
  TinyWireM.beginTransmission(_Addr);
  TinyWireM.send((int)(mode)); 
  TinyWireM.send((int)(value));
  TinyWireM.endTransmission();
}
 


// ************* Alias functions *************

void LiquidCrystal_I2C_ST7032i::cursor_on(){
 cursor();
}

void LiquidCrystal_I2C_ST7032i::cursor_off(){
 noCursor();
}

void LiquidCrystal_I2C_ST7032i::blink_on(){
  blink();
}

void LiquidCrystal_I2C_ST7032i::blink_off(){
  noBlink();
}

void LiquidCrystal_I2C_ST7032i::load_custom_character(uint8_t char_num, uint8_t *rows){
  createChar(char_num, rows);
}

void LiquidCrystal_I2C_ST7032i::setBacklight(uint8_t new_val){
  if(new_val){
    backlight();
  }else{
    noBacklight();
  }
}

void LiquidCrystal_I2C_ST7032i::printstr(const char c[]){
  //This function is not identical to the function as described in the API
  //it's here so the user sketch doesn't have to be changed 
  print(c);
}

void LiquidCrystal_I2C_ST7032i::on(){
  display();
}

void LiquidCrystal_I2C_ST7032i::off(){
  noDisplay();
}

// ************* unsupported API functions *************
void LiquidCrystal_I2C_ST7032i::setDelay (int cmdDelay,int charDelay) {}
uint8_t LiquidCrystal_I2C_ST7032i::status(){return 0;}
uint8_t LiquidCrystal_I2C_ST7032i::keypad (){return 0;}
uint8_t LiquidCrystal_I2C_ST7032i::init_bargraph(uint8_t graphtype){return 0;}
void LiquidCrystal_I2C_ST7032i::draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end){}
void LiquidCrystal_I2C_ST7032i::draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_row_end){}


	
