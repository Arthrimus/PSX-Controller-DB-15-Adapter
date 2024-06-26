/* PSX to DB-15 Adapter Firmware  
 *  Arthrimus 2024 
 *  
 *  Based on PSX Controller Decoder Library (Psx.h)
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
#include <EEPROM.h>
#include <Psx.h>                                          // Includes the Psx Library 
                                                          // Any pins can be used since it is done in software
#define dataPin 19
#define cmndPin 14
#define attPin 16
#define clockPin 15

//Buttonpress Status Variables
int UP = 0;                      // Sets up button press variables.
int DN = 0; 
int LT = 0;
int RT = 0;
int O = 0;
int X = 0;
int TR = 0;
int SQ = 0;
int R2 = 0;
int R1 = 0;
int L1 = 0;
int L2 = 0;
int ST = 0;   
int SL = 0;
// General Variables
int val = 0;                     // variable for reading the pin status (should be zero)
int outputb = 0;                 // variable for storing PORTB output values.
int outputd = 0;                 // variable for storing PORTD output values.
int buttonmap = 0;               // variable for storing which buttonmap is used for outputs, 0 = sixbuttonmode.
int remapcount = 0;              // variable for storing the count for the button combo timer
int bfcount = 0;
int remapcombo = 0;              // variable for storing the button combo state
int combodelay = 160;             // variable for storing the number of cycles for the combo timer
int combodelay2 = 220;           // variable for storing the number of cycles for combo timer 2
int NB = 0;                      // variable for storing the number of face buttons pressed simultaniously.
int autofire = 0;
int autofirecombo = 0;
int autofirecount = 0;
int autofiretimer1 = 0;
int autofiretimer2 = 0;
int autofiretimer3 = 0;
int autofiretimer4 = 0;
int autofiretimer5 = 0;
int autofiretimer6 = 0;
int HASmode = 0;
int HAScount = 0;
//Buttonmap Values
int SQO = 1;                      // Stores the current output map for each input
int TRO = 2;
int R1O = 3;
int XO = 4;
int OO = 5;
int R2O = 6;
int L1O = 0;
int L2O = 0;
// Previous Buttonpress Variables
int TRP = 0;                     // Stores the previous state of each button
int SQP = 0;
int R1P = 0;
int OP = 0;
int XP = 0;
int R2P = 0;
int L1P = 0;
int L2P = 0;
int STP = 0;
int SLP = 0;
// Current Buttonpress Variables
int TRC = 0;                    // Stores the current state of each button
int SQC = 0;
int R1C = 0;
int OC = 0;
int XC = 0;
int R2C = 0;
int L1C = 0;
int L2C = 0;
int STC = 0;
int SLC = 0;
int UPC = 0;
int DNC = 0;
int LTC = 0;
int RTC = 0;
//Autofire Values
int TRA = 0;                      // Stores the current autofire setting for each input
int SQA = 0;
int R2A = 0;
int OA = 0;
int XA = 0;
int R1A = 0;
int L1A = 0;
int L2A = 0;

Psx Psx;                                                  // Initializes the library

unsigned int data = 0;                                    // data stores the controller response

void setup(){
  Psx.setupPins(dataPin, cmndPin, attPin, clockPin, 10);  // Defines what each pin is used
                                                          // (Data Pin #, Cmnd Pin #, Att Pin #, Clk Pin #, Delay)
                                                          // Delay measures how long the clock remains at each state,
                                                          // measured in microseconds. too small delay may not work (under 5)   
 HASmode = EEPROM.read(17);
if (HASmode != 0 && HASmode != 1){
  HASmode = 1;
  EEPROM.write(17,HASmode);
}

  if (HASmode == 0){
    for (int i = 0; i <= 11; i++) {
    DDRD = 0;
    delay(16);
    DDRD |= 1;
    delay(16);
  }
TRO = EEPROM.read(0);
SQO = EEPROM.read(1);
R2O = EEPROM.read(2);
OO = EEPROM.read(3);
XO = EEPROM.read(4);
R1O = EEPROM.read(5);
L1O = EEPROM.read(13);
L2O = EEPROM.read(14);
TRA = EEPROM.read(7);
SQA = EEPROM.read(8);
R2A = EEPROM.read(9);
OA = EEPROM.read(10);
XA = EEPROM.read(11);
R1A = EEPROM.read(12);
L1A = EEPROM.read(15);
L2A = EEPROM.read(16);
  }
if (HASmode == 1){
   DDRD |= 1;
   delay(1000);
   DDRD = 0;
}
}
  
void loop()
{
  scaninputs(); 
  buttoncombos();
if (buttonmap == 1){             // Stores output data for PORTB and PORTD based on sixbutton map.
  autofire = 0;
  buttonmapping();
}else if (autofire == 1){
  autofiremapping();
}else{
  setautofire();
  setoutputs(); 
  }
  DDRB = outputb;                // Sets PORTB and PORTD outputs based on inputs and the chosen buttonmap.
  DDRD = outputd;
   delayMicroseconds(13700);
}

void setautofire(){
autofiretimer1 = (autofiretimer1 + 1);
autofiretimer2 = (autofiretimer2 + 1);
autofiretimer3 = (autofiretimer3 + 1);
autofiretimer4 = (autofiretimer4 + 1);
autofiretimer5 = (autofiretimer5 + 1);
autofiretimer6 = (autofiretimer6 + 1);

if (autofiretimer1 > 1){
  autofiretimer1 = 0; 
}
if (autofiretimer2 > 2){
  autofiretimer2 = 0; 
}
if (autofiretimer3 > 3){
  autofiretimer3 = 0; 
}
if (autofiretimer4 > 5){
  autofiretimer4 = 0; 
}
if (autofiretimer5 > 7){
  autofiretimer5 = 0; 
}
if (autofiretimer6 > 9){
  autofiretimer6 = 0; 
}

if ((TRA == 1 && autofiretimer1 < 1) || (TRA == 2 && autofiretimer2 < 2) || (TRA == 3 && autofiretimer3 < 2) || (TRA == 4 && autofiretimer4 < 3) || (TRA == 5 && autofiretimer5 < 4) || (TRA == 6 && autofiretimer6 < 5)){
    TR = 0;     
  }
if ((SQA == 1 && autofiretimer1 < 1) || (SQA == 2 && autofiretimer2 < 2) || (SQA == 3 && autofiretimer3 < 2) || (SQA == 4 && autofiretimer4 < 3) || (SQA == 5 && autofiretimer5 < 4) || (SQA == 6 && autofiretimer6 < 5)){
    SQ = 0;     
  }
if ((R2A == 1 && autofiretimer1 < 1) || (R2A == 2 && autofiretimer2 < 2) || (R2A == 3 && autofiretimer3 < 2) || (R2A == 4 && autofiretimer4 < 3) || (R2A == 5 && autofiretimer5 < 4) || (R2A == 6 && autofiretimer6 < 5)){
    R2 = 0;     
  }  
if ((OA == 1 && autofiretimer1 < 1) || (OA == 2 && autofiretimer2 < 2) || (OA == 3 && autofiretimer3 < 2) || (OA == 4 && autofiretimer4 < 3) || (OA == 5 && autofiretimer5 < 4) || (OA == 6 && autofiretimer6 < 5)){
    O = 0;     
  }
if ((XA == 1 && autofiretimer1 < 1) || (XA == 2 && autofiretimer2 < 2) || (XA == 3 && autofiretimer3 < 2) || (XA == 4 && autofiretimer4 < 3) || (XA == 5 && autofiretimer5 < 4) || (XA == 6 && autofiretimer6 < 5)){
    X = 0;     
  }
if ((R1A == 1 && autofiretimer1 < 1) || (R1A == 2 && autofiretimer2 < 2) || (R1A == 3 && autofiretimer3 < 2) || (R1A == 4 && autofiretimer4 < 3) || (R1A == 5 && autofiretimer5 < 4) || (R1A == 6 && autofiretimer6 < 5)){
    R1 = 0;     
  }
if ((L1A == 1 && autofiretimer1 < 1) || (L1A == 2 && autofiretimer2 < 2) || (L1A == 3 && autofiretimer3 < 2) || (L1A == 4 && autofiretimer4 < 3) || (L1A == 5 && autofiretimer5 < 4) || (L1A == 6 && autofiretimer6 < 5)){
    L1 = 0;     
  }
if ((L2A == 1 && autofiretimer1 < 1) || (L2A == 2 && autofiretimer2 < 2) || (L2A == 3 && autofiretimer3 < 2) || (L2A == 4 && autofiretimer4 < 3) || (L2A == 5 && autofiretimer5 < 4) || (L2A == 6 && autofiretimer6 < 5)){
    L2 = 0;     
  }
}

void scaninputs(){                                   // Six Button Mode maps all buttons to a standard six button layout. Useful for most games with standard control schemes. 

TRP = TRC;                      // new previous state variables = old current state variables
SQP = SQC;
R2P = R2C;
OP = OC;
XP = XC;
R1P = R1C;
STP = STC;
SLP = SLC;
L1P = L1C;
L2P = L2C;
// Reset All Variables
outputb = 0;                   // Resets Port Maniplulation Variables
outputd = 0;
UP = 0;                        // Resets all button state variables
DN = 0;
LT = 0;
RT = 0;
SL = 0;
ST = 0;
NB = 0;
STC = 0;
SLC = 0;
UPC = 0;
DNC = 0;
LTC = 0;
RTC = 0;
TRC = 0;
SQC = 0;
R1C = 0;
L1C = 0;
L2C = 0;
OC = 0;
XC = 0;
R2C = 0;
TR = 0;
SQ = 0;
R1 = 0;
O = 0;
X = 0;
R2 = 0;
L1 = 0;
L2 = 0;

data = Psx.read();                                      // Psx.read() initiates the PSX controller and returns

if (data & psxTri){
  TR = TRO; 
  TRC = 1;  
  NB = (NB + 1);  
}                               
if (data & psxR1){
  R1 = R1O; 
  R1C = 1;  
  NB = (NB + 1);
}
if (data & psxL1){
  L1 = L1O; 
  L1C = 1;  
  NB = (NB + 1);
}
if (data & psxL2){
  L2 = L2O; 
  L2C = 1;  
  NB = (NB + 1);
}
if (data & psxX){
  X = XO; 
  XC = 1;   
  NB = (NB + 1); 
}  
if (data & psxStrt) {
  ST = 1; 
  STC = 1;  
}
if (data & psxR2){
  R2 = R2O; 
  R2C = 1;  
  NB = (NB + 1);
}
if (data & psxO) {
  O = OO; 
  OC = 1; 
  NB = (NB + 1);
}
if (data & psxUp){
  outputd |= 2;   
}                 
if (data & psxSlct){
  SL = 1; 
  SLC = 1;  
}
if (data & psxDown){
  outputd |= 16;    
}       
if (data & psxLeft){   
  outputd |= 32;
}
if (data & psxRight){
  outputd |= 64;
}
if (data & psxSqu) {
  SQ = SQO; 
  SQC = 1; 
  NB = (NB + 1);   
}
}


void setoutputs(){           // Translates demuxpad data into the sixbutton output map.

if (SQ == 2 || TR == 2 || R1 == 2 || O == 2 || X == 2 || R2 == 2 || L1 == 2 || L2 == 2) 
  outputb |= 1;                    
if (R1 == 3 || SQ == 3 || TR == 3 || O == 3 || X == 3 || R2 == 3 || L1 == 3 || L2 == 3)
  outputb |= 2; 
if (O == 4 || SQ == 4 || TR == 4 || R1 == 4 || X == 4 || R2 == 4 || L1 == 4 || L2 == 4)
  outputb |= 4;    
if (ST == 1) 
  outputb |= 8;
if (R2 == 6 || SQ == 6 || TR == 6 || R1 == 6 || O == 6 || X == 6 || L1 == 6 || L2 == 6)
  outputb |= 16;
if (X == 5 || SQ == 5 || TR == 5 || R1 == 5 || O == 5 || R2 == 5 || L1 == 5 || L2 == 5) 
  outputb |= 32;                   
if (SL == 1)
  outputd |= 4;
if (TR == 1 || SQ == 1 || R1 == 1 || O == 1 || X == 1 || R2 == 1 || L1 == 1 || L2 == 1) 
  outputd |= 128;     
}

void buttoncombos(){
if (L1C == 1 && (R1C == 1 && (SL == 1 && (L1P == 1 && (R1P == 1 && (SLP == 1)))))){        // Checks if Start and 3 buttons are pressed. 
  HAScount = (HAScount + 1);
}else{ 
  HAScount = 0;
}

if (HAScount >= combodelay2){{{
HAScount = 0;
HASmode = (HASmode + 1); 
 }
if (HASmode > 1){
  HASmode = 0;
    for (int i = 0; i <= 11; i++) {
    DDRD = 0;
    delay(16);
    DDRD |= 1;
    delay(16);
  }
TRO = EEPROM.read(0);
SQO = EEPROM.read(1);
R2O = EEPROM.read(2);
OO = EEPROM.read(3);
XO = EEPROM.read(4);
R1O = EEPROM.read(5);
L1O = EEPROM.read(13);
L2O = EEPROM.read(14);
TRA = EEPROM.read(7);
SQA = EEPROM.read(8);
R2A = EEPROM.read(9);
OA = EEPROM.read(10);
XA = EEPROM.read(11);
R1A = EEPROM.read(12);
L1A = EEPROM.read(15);
L2A = EEPROM.read(16);
  }else{   
    DDRD |= 1;
   delay(1000);
   DDRD = 0;
   DDRB = 0;                      // Stores the current output map for each input
SQO = 1;                     
TRO = 2;
R1O = 3;
XO = 4;
OO = 5;
R2O = 6;
L1O = 0;
L2O = 0;
TRA = 0;                      // Stores the current autofire setting for each input
SQA = 0;
R2A = 0;
OA = 0;
XA = 0;
R1A = 0;
L1A = 0;
L2A = 0;
  }
 
}
EEPROM.write(17,HASmode);
}
  
if (ST == 1 && (NB == 2 && (STP == 1 && (HASmode == 0)))){        // Checks if Start and 2 buttons are pressed. 
  remapcount = (remapcount + 1);
}else{ 
  remapcount = 0;
}

if (remapcount >= combodelay){
remapcount = 0;
buttonmap = 1;                // Sets buttonmap mode to 1 
 }
 if (ST == 1 && (NB == 1 && (STP == 1 && (HASmode == 0)))){        // Checks if Start and 2 buttons are pressed. 
  autofirecount = (autofirecount + 1);
}else{ 
  autofirecount = 0;
}

if (autofirecount >= combodelay){
autofirecount = 0;
autofire = 1;                // Sets autofire mode to 1 
 }
}

void buttonmapping(){
if (ST == 1 && (NB == 2)){                        // Checks if the buttonmap combo is still held. 
 TRO = 0;                                          // Resets all buttonmap values to 0
 SQO = 0;
 R2O = 0;
 OO = 0;
 XO = 0;
 R1O = 0;
 L1O = 0;
 L2O = 0;
  outputd = 0;
 outputd |= 1;
 DDRD = outputd;
}else{// If buttonmap combo is no longer pressed, continue to button mapping mode
outputd |= 1;
if (TRC == 1 && (TRP == 0 && (NB == 1))){            // If X is currently pressed, if X was previously not pressed, if only one button (X) is pressed and if XO is less than 6
  TRO = (TRO + 1);                                             // If all of the above conditions are met, TRO is iterated +1
}
if (TRO > 6){                                       // If TRO is greater than 6, TRO is reset to 0
  TRO = 0;
}
if (SQC == 1 && (SQP == 0 && (NB == 1))){
  SQO = (SQO + 1);
}
if (SQO > 6){
  SQO = 0;
}
if (R2C == 1 && (R2P == 0 && (NB == 1))){
  R2O = (R2O + 1);
}
if (R2O > 6){
  R2O = 0;
}
if (L1C == 1 && (L1P == 0 && (NB == 1))){
  L1O = (L1O + 1);
}
if (L1O > 6){
  L1O = 0;
}
if (L2C == 1 && (L2P == 0 && (NB == 1))){
  L2O = (L2O + 1);
}
if (L2O > 6){
  L2O = 0;
}
if (OC == 1 && (OP == 0 && (NB == 1))){
  OO = (OO + 1);
}
if (OO > 6){
  OO = 0;
}
if (XC == 1 && (XP == 0 && (NB == 1))){
  XO = (XO + 1);
}
if (XO > 6){
  XO = 0;
}
if (R1C == 1 && (R1P == 0 && (NB == 1))){
  R1O = (R1O + 1);
}
if (R1O > 6){
  R1O = 0;
  }
    if (NB == 1){
  outputd = 0;
  }
DDRD = outputd;
if (ST == 1 && (STP == 0)){
    buttonmap = 0;
    EEPROM.write(0,TRO);
    EEPROM.write(1,SQO);
    EEPROM.write(2,R2O);
    EEPROM.write(3,OO);
    EEPROM.write(4,XO);
    EEPROM.write(5,R1O);
    EEPROM.write(13,L1O);
    EEPROM.write(14,L2O);
    
  } 
 }
}

void autofiremapping(){
if (ST == 1 && (NB == 1)){                        // Checks if the buttonmap combo is still held. 
 TRA = 0;                                          // Resets all buttonmap values to 0
 SQA = 0;
 R2A = 0;
 OA = 0;
 XA = 0;
 R1A = 0;
 L1A = 0;
 L2A = 0;
   outputd = 0;
 outputd |= 1;
 DDRD = outputd;
}else{          // If buttonmap combo is no longer pressed, continue to button mapping mode
outputd |= 1;
if (TRC == 1 && (TRP == 0 && (NB == 1))){            // If X is currently pressed, if X was previously not pressed, if only one button (X) is pressed and if XO is less than 6
  TRA = (TRA + 1);                                             // If all of the above conditions are met, XO is iterated +1
}
if (TRA > 6){                                       // If XO is greater than 6, XO is reset to 0
  TRA = 0;
}
if (SQC == 1 && (SQP == 0 && (NB == 1))){
  SQA = (SQA + 1);
}
if (SQA > 6){
  SQA = 0;
}
if (R2C == 1 && (R2P == 0 && (NB == 1))){
  R2A = (R2A + 1);
}
if (R2A > 6){
  R2A = 0;
}
if (OC == 1 && (OP == 0 && (NB == 1))){
  OA = (OA + 1);
}
if (OA > 6){
  OA = 0;
}
if (XC == 1 && (XP == 0 && (NB == 1))){
  XA = (XA + 1);
}
if (XA > 6){
  XA = 0;
}
if (R1C == 1 && (R1P == 0 && (NB == 1))){
  R1A = (R1A + 1);
}
if (R1A > 6){
  R1A = 0;
  }
if (L1C == 1 && (L1P == 0 && (NB == 1))){
  L1A = (L1A + 1);
}
if (L1A > 6){
  L1A = 0;
  }
if (L2C == 1 && (L2P == 0 && (NB == 1))){
  L2A = (L2A + 1);
}
if (L2A > 6){
  L2A = 0;
  }
      if (NB == 1){
  outputd = 0;
  }
DDRD = outputd;
if (ST == 1 && (STP == 0)){
    autofire = 0;
    EEPROM.write(7,TRA);
    EEPROM.write(8,SQA);
    EEPROM.write(9,R2A);
    EEPROM.write(10,OA);
    EEPROM.write(11,XA);
    EEPROM.write(12,R1A);
    EEPROM.write(15,L1A);
    EEPROM.write(16,L2A);
    
  } 
 }
}
