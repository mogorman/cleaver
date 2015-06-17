Cleaver is a design by mog to make a more portable soldering station.

Dependencies for cleaver

sudo apt-get install geda-utils coreutils sed grep bash geda-gschem ghostscript poppler-utils pcb imagemagick zip bc make git srecord geda-gnetlist gerbv xvfb

[firmware](http://artifacts.meatstand.com/cleaver/application.hex) [schematic](http://artifacts.meatstand.com/cleaver/schematic.png) [gerbers](http://artifacts.meatstand.com/cleaver/gerbers.zip)


[![the board](http://artifacts.meatstand.com/cleaver/board.png)](http://artifacts.meatstand.com/cleaver/board.png)

[![build status](http://ci.meatstand.com/projects/5/status.png?ref=master)](http://ci.meatstand.com/projects/5?ref=master)


this is a project to build a soldering iron driver for the hakko 907 5 pin style irons


ideas:
8x2 display
pot for temperature control and switch for on off
led to show on
led to show heating
buzzer?

2 boards one for lcd and pot other for soldering iron connector and power supply connector

internal temperature sensor for thermal compensation


attiny85

1 RED led, iron is safe / reset

2 Temperature probe from iron

3 pot for control of the iron

4 BLACK Gnd

5 ORANGE SDA for display

6 BROWN pwm for iron, led to alert user its heating

7 YELLOW SCL for display

8 WHITE Vcc, led to show its on



red / green bi led for power / heat


parts
erc802 23.5 x 36mm
attiny85
lm38l05




MALE end 

CW 1,2,3,4,5

blue	1, GROUND OF 20v

white	2, postive of 20v

black	3 EARTH GROUND

red	4 V+ of thermal

green	5 V- of thermal

arduino http://highlowtech.org/?p=1695 end
// dont use didnt need. pid library https://github.com/br3ttb/Arduino-PID-Library/ 
display library http://hmario.home.xs4all.nl/arduino/LiquidCrystal_I2C_ST7032i/
tinywirem https://github.com/adafruit/TinyWireM


todo

cut case better

print last prototype

adjust backlight on display  resistor value

code refactor.  reduce globals. loop in the loop


10k to ground for mosfet for safety.

move led to gate path so that i am good

have to see why resistance is being all over the map. no luck yet
