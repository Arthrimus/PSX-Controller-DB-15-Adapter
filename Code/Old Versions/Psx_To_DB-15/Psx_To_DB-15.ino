/* PSX to DB-15 Adapter Firmware  
 *  Arthrimus 2019
 *  
 *  Based on PSX Controller Decoder Library (Psx.pde)
	Written by: Kevin Ahrendt June 22nd, 2008
 
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

Psx Psx;                                                  // Initializes the library

unsigned int data = 0;                                    // data stores the controller response
int combo = 0; // buttonmap stores the current buttonmap mode setting
int output = 0;

void setup()
{
  Psx.setupPins(dataPin, cmndPin, attPin, clockPin, 5);  // Defines what each pin is used
                                                          // (Data Pin #, Cmnd Pin #, Att Pin #, Clk Pin #, Delay)
                                                          // Delay measures how long the clock remains at each state,
                                                          // measured in microseconds. too small delay may not work (under 5)                                                   
  delay(100);
  }
  
void loop()
{
delay(16);
if (combo == 0){
  sixbuttonmode();
}
else if (combo == 3){
  neogeomode ();
 }
}

void sixbuttonmode(){                                   // Six Button Mode maps all buttons to a standard six button layout. Useful for most games with standard control schemes. 

if (combo >= 0){
  combo = 0;
}
data = Psx.read();                                      // Psx.read() initiates the PSX controller and returns

output = 0;
if (data & psxTri)
  output |= 1;                    
if (data & psxR1)
  output |= 2; 
if (data & psxX){
  output |= 4;   
  combo = combo + 1;   
}     
if (data & psxStrt)  { 
  output |= 8;
  combo = combo + 1; 
}
if (data & psxR2)
  output |= 16;
if (data & psxO) 
  output |= 32;    
DDRB = output;

output = 0;
if (data & psxUp)
  output |= 2;                    
if (data & psxSlct){
  output |= 4;
  combo = combo + 1;
}
if (data & psxDown)
  output |= 16;           
if (data & psxLeft)   
  output |= 32;
if (data & psxRight)
  output |= 64;
if (data & psxSqu) 
  output |= 128;    
DDRD = output;

if (combo == 3){
delay(1000);
} else {
  combo = 0;
 }
}

void neogeomode(){                                       // NEO-GEO mode remaps K1, P1, P2 and P3 buttons to P1, P2, P3 and K1 outputs. Makes NEO-GEO games more convenient to play.

if (combo <= 3){
  combo = 3;
  }

data = Psx.read();                                  

output = 0;
if (data & psxSqu)
  output |= 1;                    
if (data & psxTri)
  output |= 2; 
if (data & psxR1)
  output |= 4;           
if (data & psxStrt){   
  output |= 8;
  combo = combo - 1; 
}
if (data & psxR2)
  output |= 16;
if (data & psxO) 
  output |= 32;    
DDRB = output;

output = 0;
if (data & psxUp)
  output |= 2;                    
if (data & psxSlct){
  output |= 4;
  combo = combo - 1; 
}
if (data & psxDown)
  output |= 16;           
if (data & psxLeft)   
  output |= 32;
if (data & psxRight)
  output |= 64;
if (data & psxX) {
  output |= 128;  
  combo = combo - 1;
}
DDRD = output;


if (combo == 0){
delay(1000);
} else {
  combo = 3;
 }
}
