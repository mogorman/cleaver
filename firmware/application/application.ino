/*-------------------------------------------------------------------------*/
/* @author    Matthew O'Gorman <mog@rldn.net>                              */
/* @copyright 2015 Matthew O'Gorman                                        */
/* Cleaver soldering iron controller code for attiny85.                    */
/* License : This program is free software, distributed under the terms of */
/*          the GNU General Public License Version 3. See the COPYING file */
/*          at the top of the source tree.                                 */
/*-------------------------------------------------------------------------*/

#include <TinyWireM.h>

#define Write_Address 0x7c/*slave addresses with write*/
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
  
}
