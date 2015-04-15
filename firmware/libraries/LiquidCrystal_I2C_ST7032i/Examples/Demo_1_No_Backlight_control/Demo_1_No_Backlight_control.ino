/*
Demonstration of most of the available functions in the LiquidCrystal_I2C_ST7032i library.
Demo is modeled for a YMFC-G0802D 2-line, 8 charcter display with ST7032i controller.

This demo doesn't use backlight control. Connect the backlight via an appropriate series resistor to a constant power source.
*/

#if defined(__AVR_ATtiny85__) || (__AVR_ATtiny2313__)  // This has not been tested
  #include "TinyWireM.h"
#else 
  #include <Wire.h>
#endif

#include <LiquidCrystal_I2C_ST7032i.h>

LiquidCrystal_I2C_ST7032i lcd(0x3E,8,2);  // set the LCD address to 0x3E for a 8 chars and 2 line display

//  Arrays with user generated symbols
uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
uint8_t note[8]  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t duck[8]  = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
uint8_t retarrow[8] = {	0x1,0x1,0x5,0x9,0x1f,0x8,0x4};

void setup(){
  Serial.begin ( 9600 ); 
  lcd.init();                      // initialize the lcd 

  lcd.createChar(0, bell);
  lcd.createChar(1, note);
  lcd.createChar(2, clock);
  lcd.createChar(3, heart);
  lcd.createChar(4, duck);
  lcd.createChar(5, check);
  lcd.createChar(6, cross);
  lcd.createChar(7, retarrow);

  lcd.clear(); 
  lcd.setCursor(1,0);
  lcd.print("Hello");
  lcd.setCursor(1,1);
  lcd.print("World!"); delay(2000);

  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Starting");
  lcd.setCursor(2,1);
  lcd.print("Demo"); delay(2000);



}
void loop(){
  
// Contrast control
  lcd.clear();
  lcd.setCursor(0,0);  lcd.print("Contras");  
  lcd.setCursor(0,1);  lcd.print("Control");  delay(1500);
  for(int h=0;h<20;h++){
    lcd.setContrast(h);
    delay(300);
  }
  delay(1000);

// Cursor control
  lcd.clear();
  lcd.setCursor(0,0);  lcd.print("Cursor");  
  lcd.setCursor(0,1);  lcd.print("Control");  delay(1500);

  lcd.clear();
  lcd.setCursor(0,0);  lcd.print("Cursor");  
  lcd.setCursor(0,1);  lcd.print("off");  delay(1500);

  lcd.clear();
  lcd.cursor();
  lcd.setCursor(0,0);  lcd.print("Cursor");  
  lcd.setCursor(0,1);  lcd.print("on");  delay(1500);

  lcd.clear();
  lcd.blink();
  lcd.setCursor(0,0);  lcd.print("Cursor");  
  lcd.setCursor(0,1);  lcd.print("blink");  delay(1500);
  lcd.noBlink();
  
// Display control
  lcd.clear();         
  lcd.setCursor(0,0);  lcd.print("Display");  
  lcd.setCursor(0,1);  lcd.print("off");  delay(1500);
  lcd.noDisplay();     delay(1000);
  lcd.noCursor();
  
  lcd.display();
  lcd.setCursor(0,0);  lcd.print("Display");  
  lcd.setCursor(0,1);  lcd.print("on ");  delay(1500);

// Scoll left
  lcd.clear();         
  lcd.setCursor(0,0);  lcd.print("Scroll");  
  lcd.setCursor(0,1);  lcd.print("left");  delay(1500);
  for(int k=0;k<8;k++){
    lcd.scrollDisplayLeft();
    delay(150);
  }
  delay(1000);
  
// Scoll right
  lcd.clear();
  lcd.home();  
  lcd.setCursor(0,0);  lcd.print("Scroll");  
  lcd.setCursor(0,1);  lcd.print("right");  delay(1500);
  for(int k=0;k<8;k++){
    lcd.scrollDisplayRight();
    delay(150);
  }
  delay(1000);

// Autoscroll
  lcd.clear();
  lcd.home();  
  lcd.setCursor(0,0);  lcd.print("Auto");  
  lcd.setCursor(0,1);  lcd.print("scroll");  delay(1500);

  lcd.autoscroll();
  lcd.setCursor(7,1);
  char msg1[]="autoscroll        ";
  for(int h=0;h<18;h++){
  lcd.print(msg1[h]);
  delay(500);
  }
  delay(1000);


// No autoscroll
  lcd.clear();
  lcd.home();
  lcd.noAutoscroll();
  lcd.setCursor(0,1);
  char msg2[]="noAutoscroll";
  for(int h=0;h<12;h++){
  lcd.print(msg2[h]);
  delay(150);
  }
  delay(1000);

  lcd.clear();         
  lcd.setCursor(0,0);  lcd.print("User");  
  lcd.setCursor(0,1);  lcd.print("chars");  delay(1500);
  lcd.clear();
  lcd.setCursor(0,0);  
  for(uint8_t cr = 0; cr < 8; cr++){
    lcd.write(cr);
  }
  delay(1500);
  
  for (uint8_t m = 0; m < 32; m++) {
      lcd.clear();
      lcd.print((m*8), HEX);  lcd.print("H-");
      lcd.print((m*8)+7, HEX);lcd.print("H");
      lcd.setCursor(0, 1);
      for (int j=0; j<8; j++) {
          lcd.write((m*8)+j);
      }
      delay(2000);
  }

}

