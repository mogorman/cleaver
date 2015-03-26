v 20130925 2
C 40600 38100 1 0 0 include-1.sym
{
T 40700 38200 5 10 1 1 0 0 1
device=include
T 40600 38400 5 10 1 1 0 0 1
refdes=ICO1
T 41300 38200 5 10 1 1 0 0 1
footprint=oshw
}
C 40300 37800 0 0 0 title-A2.sym
T 56600 38500 14 10 1 0 0 0 1
Spam
T 56500 38200 14 8 1 0 0 0 1
http://dev.meatstand.com/meatstand/spam.git
T 57600 37900 14 10 1 0 0 0 1
1
T 58100 37900 14 10 1 0 0 0 1
1
T 60500 37900 14 10 1 0 0 0 1
Matthew O'Gorman <mog@rldn.net>
T 60400 38200 14 10 1 0 0 0 1
@version@
T 62000 38200 14 10 1 0 0 0 1
@git@
T 60800 38500 14 10 1 0 0 0 1
This schematic is licensed GPLv3
C 48300 46200 1 0 0 attiny85.sym
{
T 50400 48400 5 10 1 1 0 0 1
refdes=U?
T 49500 48400 5 10 1 1 0 0 1
footprint=attiny85
}
C 50600 45900 1 0 0 ground.sym
C 54500 46600 1 0 1 input-2.sym
{
T 54500 46800 5 10 0 0 0 6 1
net=Vcc5:1
T 53900 47300 5 10 0 0 0 6 1
device=none
T 54000 46700 5 10 1 1 0 1 1
value=Vcc5
}
C 42700 43500 1 0 1 connector5-2.sym
{
T 42000 46000 5 10 1 1 0 0 1
refdes=CONN?
T 42400 45950 5 10 0 0 0 6 1
device=CONNECTOR_5
T 42400 46150 5 10 0 0 0 6 1
footprint=SIP5N
}
C 41800 51200 1 0 0 pwrjack-1.sym
{
T 41900 51700 5 10 0 0 0 0 1
device=PWRJACK
T 41800 51700 5 10 1 1 0 0 1
refdes=CONN?
}
C 42500 51000 1 0 0 ground.sym
C 42700 51400 1 0 0 output-2.sym
{
T 43600 51600 5 10 0 0 0 0 1
net=Vcc24:1
T 42900 52100 5 10 0 0 0 0 1
device=none
T 43600 51500 5 10 1 1 0 1 1
value=Vcc24
}
C 44100 45000 1 0 1 input-2.sym
{
T 44100 45200 5 10 0 0 0 6 1
net=Vcc24:1
T 43500 45700 5 10 0 0 0 6 1
device=none
T 43600 45100 5 10 1 1 0 1 1
value=Vcc24
}
C 47700 51600 1 0 0 connector3-1.sym
{
T 49500 52500 5 10 0 0 0 0 1
device=CONNECTOR_3
T 47700 52700 5 10 1 1 0 0 1
refdes=CONN?
T 47700 51300 5 10 1 1 0 0 1
footprint=7805
}
C 50800 52300 1 0 1 input-2.sym
{
T 50800 52500 5 10 0 0 0 6 1
net=Vcc24:1
T 50200 53000 5 10 0 0 0 6 1
device=none
T 50300 52400 5 10 1 1 0 1 1
value=Vcc24
}
C 49700 51900 1 90 0 ground.sym
C 49400 51700 1 0 0 output-2.sym
{
T 50300 51900 5 10 0 0 0 0 1
net=Vcc5:1
T 49600 52400 5 10 0 0 0 0 1
device=none
T 50300 51800 5 10 1 1 0 1 1
value=Vcc5
}
C 47300 52300 1 0 1 input-2.sym
{
T 47300 52500 5 10 0 0 0 6 1
net=Vcc24:1
T 46700 53000 5 10 0 0 0 6 1
device=none
T 46800 52400 5 10 1 1 0 1 1
value=Vcc24
}
C 44700 52600 1 270 0 ground.sym
C 45000 52200 1 0 0 capacitor-1.sym
{
T 45200 52900 5 10 0 0 0 0 1
device=CAPACITOR
T 45200 52700 5 10 1 1 0 0 1
refdes=C?
T 45200 53100 5 10 0 0 0 0 1
symversion=0.1
T 45600 52500 5 10 1 1 0 0 1
value=0.1uF
}
C 44700 51800 1 270 0 ground.sym
C 45000 51400 1 0 0 capacitor-1.sym
{
T 45200 52100 5 10 0 0 0 0 1
device=CAPACITOR
T 45200 52300 5 10 0 0 0 0 1
symversion=0.1
T 45200 51900 5 10 1 1 0 0 1
refdes=C?
T 45600 51700 5 10 1 1 0 0 1
value=0.1uF
}
C 47300 51500 1 0 1 input-2.sym
{
T 47300 51700 5 10 0 0 0 6 1
net=Vcc5:1
T 46700 52200 5 10 0 0 0 6 1
device=none
T 46800 51600 5 10 1 1 0 1 1
value=Vcc5
}
C 43300 47600 1 270 1 pot-1.sym
{
T 44200 48400 5 10 0 0 90 2 1
device=VARIABLE_RESISTOR
T 43700 48200 5 10 1 1 90 2 1
refdes=R?
T 44800 48400 5 10 0 0 90 2 1
footprint=none
T 43600 47800 5 10 1 1 0 0 1
value=10K
}
C 43200 47300 1 0 0 ground.sym
C 43500 49900 1 90 1 input-2.sym
{
T 43300 49900 5 10 0 0 90 6 1
net=Vcc5:1
T 42800 49300 5 10 0 0 90 6 1
device=none
T 43600 49400 5 10 1 1 180 1 1
value=Vcc5
}
C 54200 49500 1 0 0 lm358.sym
{
T 54400 50400 5 10 1 1 0 0 1
name=LM358
T 56100 50400 5 10 1 1 0 0 1
refdes=U?
T 55100 50200 5 10 0 1 0 0 1
footprint=lm358p
}
C 43900 48000 1 0 0 output-2.sym
{
T 44800 48200 5 10 0 0 0 0 1
net=Iron_control:1
T 44100 48700 5 10 0 0 0 0 1
device=none
T 44800 48100 5 10 1 1 0 1 1
value=Iron_control
}
C 46900 47300 1 0 0 input-2.sym
{
T 46900 47500 5 10 0 0 0 0 1
net=Iron_control:1
T 47500 48000 5 10 0 0 0 0 1
device=none
T 47400 47400 5 10 1 1 0 7 1
value=Iron_control
}
C 57900 49500 1 0 1 input-2.sym
{
T 57900 49700 5 10 0 0 0 6 1
net=Vcc5:1
T 57300 50200 5 10 0 0 0 6 1
device=none
T 57400 49600 5 10 1 1 0 1 1
value=Vcc5
}
C 58100 49500 1 0 0 ground.sym
N 56500 49800 58300 49800 4
C 57000 52400 1 0 0 icsp.sym
{
T 57200 53200 5 10 1 1 0 0 1
footprint=icsp
T 58400 53200 5 10 1 1 0 0 1
refdes=CONN?
}
C 59000 52300 1 0 0 ground.sym
C 60600 52900 1 0 1 input-2.sym
{
T 60600 53100 5 10 0 0 0 6 1
net=Vcc5:1
T 60000 53600 5 10 0 0 0 6 1
device=none
T 60100 53000 5 10 1 1 0 1 1
value=Vcc5
}
C 44700 45300 1 270 0 resistor-1.sym
{
T 45100 45000 5 10 0 0 270 0 1
device=RESISTOR
T 44400 44700 5 10 1 1 0 0 1
refdes=R?
T 44900 44600 5 10 1 1 0 0 1
value=100K
}
C 44700 44200 1 270 0 resistor-1.sym
{
T 45100 43900 5 10 0 0 270 0 1
device=RESISTOR
T 44400 43600 5 10 1 1 0 0 1
refdes=R?
T 44900 43600 5 10 1 1 0 0 1
value=100
}
C 44900 46700 1 90 1 input-2.sym
{
T 44700 46700 5 10 0 0 90 6 1
net=Vcc5:1
T 44200 46100 5 10 0 0 90 6 1
device=none
T 45000 46200 5 10 1 1 180 1 1
value=Vcc5
}
C 44600 43000 1 0 0 ground.sym
N 44800 44400 44800 44200 4
N 44800 44300 42700 44300 4
C 42700 43800 1 0 0 output-2.sym
{
T 43600 44000 5 10 0 0 0 0 1
net=Temp:1
T 42900 44500 5 10 0 0 0 0 1
device=none
T 43500 43900 5 10 1 1 0 1 1
value=Temp
}
C 42700 45400 1 0 0 output-2.sym
{
T 43600 45600 5 10 0 0 0 0 1
net=Iron_EN:1
T 42900 46100 5 10 0 0 0 0 1
device=none
T 43500 45500 5 10 1 1 0 1 1
value=Iron_EN
}
