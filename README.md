# PSX Controller to DB15 Adapter.

This is an Arduino sketch converting PSX controller inputs to discrete outputs. I desinged it for Arcade Supergun use but it could be adapted for use in many other control applications that require discrete outputs. This code is dependent on the PSX.h library. You need to install that library in order to compile the sketch. https://github.com/dcunited001/arduino-psx

# Instructions

The upload the .ino file to an Arduino Uno or similar compatible device and connect the I/O pins according to the layout described in the .ino file. If you wish to remap the buttons you can modify the pin selections in the file to suit your needs. 
  
For the custom PCB version you will need to program the compiled HEX file that matches your MCU of choice. This project will work with Atmega88, 168, and 328 models. I recommend using AVRDUDESS for easy GUI based programming. You can use a USBASP and make an adapter cable using a male DB15 connector for programming. The SPI pinout for the DB15 connector is listed on the bottom silkscreen of the PCB.
# Pinout (Arduino pin labels) 

  These values are hard coded and cannot be easily changed. This is because the code uses direct port maniplulation to set the button outputs to open drain when not pressed. This is a safety feature.

**Button Outputs**
  - UP       = 1
  - SELECT   = 2
  - DOWN     = 4
  - LEFT     = 5
  - RIGHT    = 6
  - TRIANGLE = 7
  - SQUARE   = 8
  - R2       = 9
  - CIRCLE   = 10
  - START    = 11  
  - CROSS    = 13
  - R1       = 12

**PSX Pins**
 - CLOCK = 15
 - CMD   = 14
 - ATTN  = 16
 - DATA  = 19

# BUTTON REMAPPING AND AUTOFIRE

In the code folder there is an arduino sketch labeled "PSX_to_DB15_Remap.ino" This firmware has full fledged on the fly remapping of CROSS,CIRCLE,R1,SQUARE,TRIANGLE and R2. L1 and L2 can also be mapped to any of the aformentioned buttons.

To enter button remapping mode hold any 2 face buttons and START simultaniously for 3 seconds.
Once in remapping mode press each button the corresponding number of times to achive the desired output based on the list below.

 - Button 1  = 1 press
 - Button 2  = 2 presses
 - Button 3  = 3 presses
 - Button 4  = 4 presses
 - Button 5  = 5 presses
 - Button 6  = 6 presses
 - N/A       = 7 presses
 
 In the code folder there is another sketch labaled "PSX_to_DB15_Remap_AF.ino" This is an experemental firmware that has the same button remapping system as the above version, but also adds auto fire capability as well. 
 
 To enter auto fire programming mode hold any 1 face button and START simultaniously for 3 seconds. 
 Once in auto fire programming mode press each button that you want to set as auto fire the number of times that corresponds to the desired speed in the list below.
 
  - ~30hz = 1 press
  - ~20hz = 2 presses
  - ~15hz = 3 presses
  - ~12hz = 4 presses
  - ~7.5hz = 5 presses
  - ~6hz = 6 presses

Button mapping and auto fire settings are saved in the EEPROM so they will persist after power cycling the AVR.


# Custom PCB Info.

The PCB folder has source files and Gerbers for the custom PCB version of this project.
