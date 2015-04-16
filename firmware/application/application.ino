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


const float fAmplification = 183 ; // measured by series of experiment, for ex.: input 21.2mV, output 3.9V
// const float fKTypeSensitivity = 0.000041; // V/degC   this is in theory for K type
// But based on experiment on known melting point for the Multicore Alloy C99 (Liquidus on 240 degC), generate voltage is 15mV, so it correspond to TMelting-TAmb = 240-25 = 215 degC
// so sensitivity is calculated as 0.015/215 = 6.9767e-5
//const float fKTypeSensitivity = 0.000075; //old
const float fKTypeSensitivity = 6.9767e-5;

const float fADResolution = 1024.0; // for 10 bit AD converter
const float fADMax = 5.0; // AD convertor max range
//--PID
const float fKp = 10.0; //proportional gian (Value tested with HQ Soldering Iron = 10 )
const float fKi = 1.0; //integral gian (Value tested with HQ Soldering Iron = 1)
const float fKd = 10.0; //derivative gian (Value tested with HQ Soldering Iron = 10)


//calculation
float fTemp = 0.0;   // Temperature [C]
float fTAmb = 25.0;  // Ambient temperature [C]
float fTSet = 0.0; // Set point [C]
float fVoltage; // Voltage from Ain0

float fLSB = 0.0 ; // one LowSiginificantBit [V], (5/1024=0.0048828125) 
float fScaleFactor = 0.0; // [degC/LSB] (0.0048828125/340/0.000041=0.35)
float fTime = 0.0; // Time
//---------
float fTimeSampling;
float fTimeLast;
int iCycleCounter=0; //Counter of cycles for printing on display
int iPID_or_OnOff = 0; //0=PWM, 1=OnOff  !!!!! HERE THE INITIAL CONTROLER IS SET !!!!
//--PID
float fEk = 0.0; //current error
float fEk_1 = 0.0; //last error
float fPWM; //output PWM
float fIntegralTerm =0.0;


int pot_value=0;

LiquidCrystal_I2C_ST7032i lcd(0x3E,8,2);  // set the LCD address to 0x3E for a 8 chars and 2 line display

// the setup routine runs once when you press reset:
void setup()
{
  pinMode(TEMP, INPUT);
  pinMode(POT, INPUT);
  pinMode(IRON, OUTPUT);
  digitalWrite(IRON, LOW);
  lcd.init();

  fLSB = fADMax / fADResolution; // one LowSiginificantBit [V], (5/1024=0.0048828125) 
  fScaleFactor = ( 1.0 / fAmplification ) / fKTypeSensitivity;  // [degC/LSB] (0.0048828125/340/0.000041=0.35)
  fTimeLast = millis();
  
  lcd.clear(); 
  lcd.setCursor(1,0);
  lcd.print("Hello");
  lcd.setCursor(1,1);
  lcd.print("World!");
}

void loop() {
  fTime = millis() / 1000.0 ;
  fTimeSampling = fTime - fTimeLast;
  fTimeLast = fTime ;

  fVoltage = fMeasureOversampling() * fLSB; // read the input pin and calculate Voltage
  fTemp = fTAmb + fScaleFactor * fVoltage;    // calculate the Temperature

  fEk = fTSet - fTemp; //error for simple PID
  
  if (iPID_or_OnOff == 1 )  { //On-Off regulator  // simple ON-OFF control, works well (about +8 and -2 degC about SetPoint)
    if (fEk < 0) { analogWrite(IRON, 0);     }
    else         { analogWrite(IRON, 255);   }
  }

  if (iPID_or_OnOff == 0 )  { //PID regulator
    fPWM = fSimplePID(); //calculate PID command
    fEk_1 = fEk;  //store the last error 
    analogWrite(IRON, fPWM);  //execute the command
  }

  pot_value=analogRead(POT);
  update_display();
}



float fMeasureOversampling() {
  // Iako mozda nema potrebe, uradimo vise merenja za jedno odredjivanje temperature (oversampling), teoretski sa 4^n merenja dobijamo dodatnih n bita.
  // It takes about 100 microseconds (0.0001 s) to read an analog input so we can make 1024 measurements = 4^5 to hopfully add 5 bits to reac 10 bits in AD converter, it will took about 0.1s to execute  
  float fSum = 0.0; 
  for (int i=1; i <= 1024; i++){
    fSum = fSum + analogRead(TEMP);
  } 
  return fSum / 1024 ;
}

float fLimit (float fPromenljiva, float fMax, float fMin){ //Saturation function
  if (fPromenljiva > fMax)  {  fPromenljiva = fMax ; }
  if (fPromenljiva < fMin)  {  fPromenljiva = fMin ; }
  return fPromenljiva ;
}

float fSimplePID() {
  // calculate PID command, first calculate the coeficients
  float fSimplePID;
  fIntegralTerm = fIntegralTerm + fKi * fEk * fTimeSampling;
  fIntegralTerm = fLimit(fIntegralTerm, 255.0, 0.0);
  fSimplePID = fKp * fEk + fIntegralTerm + fKd * (fEk - fEk_1); // SimplePID
  fSimplePID = fLimit(fSimplePID, 255.0, 0.0);
  return fSimplePID;
}


void update_display() {
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Bye");
  lcd.setCursor(1,1);
  lcd.print("World!");
}
