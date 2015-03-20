Cleaver is a design by mog to make a more portable soldering station.

Dependencies for cleaver

sudo apt-get install geda-utils coreutils sed grep bash geda-gschem ghostscript poppler-utils pcb imagemagick zip bc make git srecord geda-gnetlist gerbv xvfb

[gerbers](http://artifacts.meatstand.com/cleaver/gerbers.zip)

[schematic](http://artifacts.meatstand.com/cleaver/schematic.png)

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
1 led, iron is safe / reset
2 Temperature probe from iron
3 pot for control of the iron
4 Gnd
5 pwm for iron, led to alert user its heating
6 SCL for display
7 SDA for display
8 Vcc, led to show its on



red / green bi led for power / heat
