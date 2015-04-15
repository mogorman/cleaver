/*-------------------------------------------------------------------------*/
/* @author    Matthew O'Gorman <mog@rldn.net>                              */
/* @copyright 2015 Matthew O'Gorman                                        */
/* Cleaver soldering iron controller code for attiny85.                    */
/* License : This program is free software, distributed under the terms of */
/*          the GNU General Public License Version 3. See the COPYING file */
/*          at the top of the source tree.                                 */
/*-------------------------------------------------------------------------*/

#include <TinyWireM.h>

#define DISPLAY 0x7c/*slave addresses with write*/
#define Read_Address 0x7d/*slave addresses with read*/


#define LED 3
// the setup routine runs once when you press reset:
void setup()
{                
  pinMode(LED, OUTPUT);
  TinyWireM.begin();
  init_display();
}

void loop() {
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(5000);               // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(5000);               // wait for a secondIGH is the voltage level)
}


void init_display()
{
  TinyWireM.beginTransmission(DISPLAY);
  //ack
  TinyWireM.send(0x80); //control byte.
  //ack
  TinyWireM.send(0x38);   //FUNCTION SET 8 bit,N=1,5*7dot
  //  Check_Ack();
  TinyWireM.send(0x80);   // control byte
  //Check_Ack();
  TinyWireM.send(0x39);  //FUNCTION SET 8 bit,N=1,5*7dot IS=1
  //Check_Ack();
  TinyWireM.send(0x80);   // control byte
  //    Check_Ack();
  TinyWireM.send(0x1c);   //Internal OSC frequency adjustment 183HZ    bias will be 1/4 
  //    Check_Ack();
  TinyWireM.send(0x80);   // control byte
  //    Check_Ack();
  TinyWireM.send(0x78);    //Contrast control  low byte
  //    Check_Ack();
  TinyWireM.send(0x80);     // control byte
  //    Check_Ack();
  TinyWireM.send(0x53);    //booster circuit is turn off./ICON   /Contrast control   high byte
  //    Check_Ack();
  TinyWireM.send(0x80);    // control byte
  //    Check_Ack();
  TinyWireM.send(0x6c);  //Follower control
  //    Check_Ack();
  TinyWireM.send(0x80);   // control byte
  //    Check_Ack();
  TinyWireM.send(0x0c);    //DISPLAY ON
  //  Check_Ack();
  TinyWireM.send(0x80);   // control byte
  //    Check_Ack();
  TinyWireM.send(0x01);   //CLEAR DISPLAY
  //    Check_Ack();
  TinyWireM.send(0x80);   // control byte
  //    Check_Ack();
  TinyWireM.send(0x06);   //ENTRY MODE SET  CURSOR MOVES TO RIGHT
  //    Check_Ack();
  TinyWireM.send(0x80);   // control byte
  //.  Check_Ack();
  TinyWirM.send(0x02);
  //    Check_Ack();
  TinyWireM.endTransmission();
}
