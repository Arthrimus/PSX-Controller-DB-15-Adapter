/* PSX to DB-15 Adapter Firmware  
 *  Arthrimus 2019
 *  
 *  Based on PSX Controller Decoder Library (Psx.pde)
	Written by: Kevin Ahrendt June 22nd, 2008
	
	Controller protocol implemented using Andrew J McCubbin's analysis.
	http://www.gamesx.com/controldata/psxcont/psxcont.htm
	
	Shift command is based on tutorial examples for ShiftIn and ShiftOut
	functions both written by Carlyn Maw and Tom Igoe
	http://www.arduino.cc/en/Tutorial/ShiftIn
	http://www.arduino.cc/en/Tutorial/ShiftOut

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Psx.h>                                          // Includes the Psx Library 
                                                          // Any pins can be used since it is done in software
#define dataPin 19
#define cmndPin 14
#define attPin 16
#define clockPin 15

int UP = 1; // These choose the pins for the button outputs. (Change these-
int DN = 4; // if you wish to remap the buttons.)
int LT = 5;
int RT = 6;
int X = 10;
int O = 13;
int R2 = 12;
int SQ = 7;
int TR = 8;
int R1 = 9;
int SL = 2;
int ST = 11;  

Psx Psx;                                                  // Initializes the library

unsigned int data = 0;                                    // data stores the controller response

void setup()
{
  Psx.setupPins(dataPin, cmndPin, attPin, clockPin, 5);  // Defines what each pin is used
                                                          // (Data Pin #, Cmnd Pin #, Att Pin #, Clk Pin #, Delay)
                                                          // Delay measures how long the clock remains at each state,
                                                          // measured in microseconds.
  pinMode(UP, OUTPUT);  // declares buttons as outputs (don't change these)
  pinMode(DN, OUTPUT); 
  pinMode(LT, OUTPUT);
  pinMode(RT, OUTPUT);
  pinMode(O, OUTPUT);
  pinMode(X, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(TR, OUTPUT);
  pinMode(SQ, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(SL, OUTPUT);
  pinMode(ST,OUTPUT);                                                        // too small delay may not work (under 5)

}

void loop()
{
  data = Psx.read();                                      // Psx.read() initiates the PSX controller and returns
                                                          // the button data
  if (data & psxX) {
    digitalWrite(X, LOW);                     
  }
  else
  {
    digitalWrite(X, HIGH);                            
  }
    if (data & psxO) {
    digitalWrite(O, LOW);                     
  }
  else
  {
    digitalWrite(O, HIGH);                            
  }
      if (data & psxSqu) {
    digitalWrite(SQ, LOW);                     
  }
  else
  {
    digitalWrite(SQ, HIGH);                            
  }
    if (data & psxTri) {
    digitalWrite(TR, LOW);                     
  }
  else
  {
    digitalWrite(TR, HIGH);                            
  }
      if (data & psxR1) {
    digitalWrite(R1, LOW);                     
  }
  else
  {
    digitalWrite(R1, HIGH);                            
  }
      if (data & psxR2) {
    digitalWrite(R2, LOW);                     
  }
  else
  {
    digitalWrite(R2, HIGH);                            
  }
      if (data & psxSlct) {
    digitalWrite(SL, LOW);                     
  }
  else
  {
    digitalWrite(SL, HIGH);                            
  }
      if (data & psxStrt) {
    digitalWrite(ST, LOW);                     
  }
  else
  {
    digitalWrite(ST, HIGH);                            
  }
  if (data & psxUp) {
    digitalWrite(UP, LOW);                     
  }
  else
  {
    digitalWrite(UP, HIGH);                            
  }
      if (data & psxDown) {
    digitalWrite(DN, LOW);                     
  }
  else
  {
    digitalWrite(DN, HIGH);                            
  }
      if (data & psxLeft) {
    digitalWrite(LT, LOW);                     
  }
  else
  {
    digitalWrite(LT, HIGH);                            
  }
      if (data & psxRight) {
    digitalWrite(RT, LOW);                     
  }
  else
  {
    digitalWrite(RT, HIGH);                            
  }
  
}
