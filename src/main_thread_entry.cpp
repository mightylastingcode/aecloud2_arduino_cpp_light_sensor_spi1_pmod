/*********************************************************************
This is the main thread for placing Arduino Sketch code to run on Renesas AE Cloud2 S5D9 board

Created on: September 17, 2018
First Released on: March 19, 2019
Author: Michael Li (michael.li@miketechuniverse.com)

An Arduino sketch is placed inside the section that is defined by two comment lines.  The section has the
example of setup() and loop() functions.   You can replace them with your example code.  It is very simple.
Most common functions like Serial and Wire functions are supported except of the SPI functions which need to
be modified to run.


The MIT License (MIT)


Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


*********************************************************************/



#include "main_thread.h"
#include <Arduino.h>
#include <Wire.h>
#include <Wire1.h>
#include "SERIAL.h"
#include "SERIAL1.h"
#include "SPI.h"
#include "SPI1.h"
#include <stdio.h>


//====================== Your Arduino Example Sketch Begin ===========//
SERIAL1 Serial  = SERIAL1();   //UART 1
SPI1    SPI = SPI1();   // SPI 1 port

void setup() {
    Serial.begin(9600);
    //while(!Serial);
    Serial.println("begin uart1...");
    SPI.begin(SPI1_SSL0);
}

/*
i0=00000001  (Data = bit3-bit0) with four leading zeros (not as spec said)
i1=11010000  (Data = bit7-bit4) with four trailing zeros
Lumiere=00011101
Lumiere=29  <Dim Light>

i0=00000000
i1=01000000
Lumiere=00000100
Lumiere=4 <No light>

i0=00001111
i1=01110000
Lumiere=11110111
Lumiere=247 <Bright Light>

*/
void loop() {
    int i;
     int recu[2]; // Storage of module data
     int lumiere;

     //////////////////////////////////////////////
     char buffer[20];

     SPI.read_transfer(buffer, 2);

     recu[0] = buffer[0];
     recu[1] = buffer[1];
     //////////////////////////////////////////////

      for (i=0;i<2;i=i+1) // Send in serial monitor
          {
          Serial.print("i");
          Serial.print(i);
          Serial.print("=");
          //Serial.println(recu[i]);  // not good to see negative numbers
          Serial.println(buffer[i],BIN);
          }


       //lumiere=(recu[0]<<3)|(recu[1]>>4); // Reconstitution of the 8-bit light variable (wrong construction)
       lumiere= ((buffer[0]<<4) & 0xF0) | ((buffer[1]>>4) & 0x0F); // Correct construction)

       //Serial.print("Lumiere=");
       //Serial.println(lumiere);
       //Serial.println(lumiere,BIN);
       Serial.print("Lumiere=");
       //Serial.println(lumiere);  // not good with negative number
       Serial.println((unsigned)lumiere,DEC);
       delay(1000);


}

//====================== Your Arduino Example Sketch End ===========//



//============================= Renesas Main Thread entry function ====================================//
void main_thread_entry(void)
{
   system_setup();
   setup();

    while (1) {
        loop();
        delay(1);  // required for the thread if delay()does not exist in the loop() function.
    }
}

