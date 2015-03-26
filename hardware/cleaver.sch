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
C 41400 46700 1 0 1 connector5-2.sym
{
T 40700 49200 5 10 1 1 0 0 1
refdes=CONN?
T 41100 49150 5 10 0 0 0 6 1
device=CONNECTOR_5
T 41100 49350 5 10 0 0 0 6 1
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
C 42800 48200 1 0 1 input-2.sym
{
T 42800 48400 5 10 0 0 0 6 1
net=Vcc24:1
T 42200 48900 5 10 0 0 0 6 1
device=none
T 42300 48300 5 10 1 1 0 1 1
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
C 46900 47700 1 270 1 pot-1.sym
{
T 47800 48500 5 10 0 0 90 2 1
device=VARIABLE_RESISTOR
T 47300 48300 5 10 1 1 90 2 1
refdes=R?
T 48400 48500 5 10 0 0 90 2 1
footprint=none
}
C 46800 47400 1 0 0 ground.sym
C 47100 50000 1 90 1 input-2.sym
{
T 46900 50000 5 10 0 0 90 6 1
net=Vcc5:1
T 46400 49400 5 10 0 0 90 6 1
device=none
T 47200 49500 5 10 1 1 180 1 1
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
