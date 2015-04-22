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
Cleaver
T 56500 38200 14 8 1 0 0 0 1
http://dev.meatstand.com/meatstand/cleaver.git
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
T 48500 48400 5 10 1 1 0 0 1
refdes=U1
T 49900 47100 5 10 0 1 0 0 1
footprint=SO8M
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
C 41800 51200 1 0 0 pwrjack-1.sym
{
T 41900 51700 5 10 0 0 0 0 1
device=PWRJACK
T 41800 51700 5 10 1 1 0 0 1
refdes=CONN2
T 42200 51300 5 10 0 1 0 0 1
footprint=CON__CUI_PJ-202AH
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
refdes=C1
T 45200 53100 5 10 0 0 0 0 1
symversion=0.1
T 45600 52500 5 10 1 1 0 0 1
value=0.1uF
T 45000 52200 5 10 0 1 0 0 1
footprint=0805
}
C 44700 51800 1 270 0 ground.sym
C 45000 51400 1 0 0 capacitor-1.sym
{
T 45200 52100 5 10 0 0 0 0 1
device=CAPACITOR
T 45200 52300 5 10 0 0 0 0 1
symversion=0.1
T 45200 51900 5 10 1 1 0 0 1
refdes=C2
T 45600 51700 5 10 1 1 0 0 1
value=0.1uF
T 45000 51400 5 10 0 1 0 0 1
footprint=0805
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
C 42000 47700 1 270 1 pot-1.sym
{
T 42900 48500 5 10 0 0 90 2 1
device=VARIABLE_RESISTOR
T 42400 48500 5 10 1 1 0 2 1
refdes=R1
T 42400 47900 5 10 1 1 0 0 1
value=10K
T 42000 48400 5 10 0 1 0 0 1
footprint=POT__CTS_296U-Series
}
C 41900 47400 1 0 0 ground.sym
C 42200 50000 1 90 1 input-2.sym
{
T 42000 50000 5 10 0 0 90 6 1
net=Vcc5:1
T 41500 49400 5 10 0 0 90 6 1
device=none
T 42300 49500 5 10 1 1 180 1 1
value=Vcc5
}
C 58700 49000 1 0 0 lm358.sym
{
T 58900 49900 5 10 1 1 0 0 1
name=LM358
T 60600 49900 5 10 1 1 0 0 1
refdes=U2
T 59600 49700 5 10 0 1 0 0 1
footprint=SO8M
}
C 42600 48100 1 0 0 output-2.sym
{
T 43500 48300 5 10 0 0 0 0 1
net=Iron_control:1
T 42800 48800 5 10 0 0 0 0 1
device=none
T 43500 48200 5 10 1 1 0 1 1
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
C 62400 49000 1 0 1 input-2.sym
{
T 62400 49200 5 10 0 0 0 6 1
net=Vcc5:1
T 61800 49700 5 10 0 0 0 6 1
device=none
T 61900 49100 5 10 1 1 0 1 1
value=Vcc5
}
C 62600 49000 1 0 0 ground.sym
N 61000 49300 62800 49300 4
C 57000 52400 1 0 0 icsp.sym
{
T 57200 53200 5 10 1 1 0 0 1
footprint=icsp
T 58400 53200 5 10 1 1 0 0 1
refdes=CONN4
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
C 48900 42900 1 0 0 mosfet-with-diode-1.sym
{
T 49800 43400 5 10 0 0 0 0 1
device=NPN_TRANSISTOR
T 49800 43400 5 10 1 1 0 0 1
refdes=Q1
T 50000 43000 5 10 0 1 0 0 1
footprint=DPAK
}
C 49300 42600 1 0 0 ground.sym
C 49400 45300 1 270 0 input-2.sym
{
T 49600 45300 5 10 0 0 270 0 1
net=Iron:1
T 50100 44700 5 10 0 0 270 0 1
device=none
T 50000 44400 5 10 1 1 0 7 1
value=Iron
}
C 46600 43300 1 0 0 input-2.sym
{
T 46600 43500 5 10 0 0 0 0 1
net=Iron_EN:1
T 47200 44000 5 10 0 0 0 0 1
device=none
T 46400 43400 5 10 1 1 180 7 1
value=Iron_EN
}
C 57300 49600 1 180 1 input-2.sym
{
T 57300 49400 5 10 0 0 180 6 1
net=Temp:1
T 57900 48900 5 10 0 0 180 6 1
device=none
T 57800 49500 5 10 1 1 180 1 1
value=Temp
}
C 58600 50900 1 180 0 ground.sym
C 58300 50600 1 270 0 resistor-1.sym
{
T 58700 50300 5 10 0 0 270 0 1
device=RESISTOR
T 58500 50400 5 10 1 1 0 0 1
refdes=R4
T 58500 50000 5 10 1 1 0 0 1
value=1K
T 58300 50600 5 10 0 1 0 0 1
footprint=0805
}
C 58100 50600 1 90 1 resistor-1.sym
{
T 57700 50300 5 10 0 0 270 2 1
device=RESISTOR
T 57900 50400 5 10 1 1 0 6 1
refdes=R5
T 57400 50000 5 10 1 1 0 0 1
value=100K
T 58100 50600 5 10 0 1 0 0 1
footprint=0805
}
N 58700 49700 58000 49700 4
N 58000 50600 58000 51100 4
N 58000 51100 61300 51100 4
N 61300 51100 61300 49700 4
N 61300 49700 61000 49700 4
N 58000 50900 56500 50900 4
N 58700 49100 56500 49100 4
N 56500 49100 56500 50900 4
C 46900 47500 1 0 0 input-2.sym
{
T 46900 47700 5 10 0 0 0 0 1
net=Temp_amped:1
T 47500 48200 5 10 0 0 0 0 1
device=none
T 47400 47600 5 10 1 1 0 7 1
value=Temp_amped
}
C 58700 49200 1 0 1 output-2.sym
{
T 57800 49400 5 10 0 0 0 6 1
net=Temp_amped:1
T 58500 49900 5 10 0 0 0 6 1
device=none
T 57800 49300 5 10 1 1 0 7 1
value=Temp_amped
}
C 61000 49400 1 0 0 output-2.sym
{
T 61900 49600 5 10 0 0 0 0 1
net=Temp_amped:1
T 61200 50100 5 10 0 0 0 0 1
device=none
T 61900 49500 5 10 1 1 0 1 1
value=Temp_amped
}
C 48300 47900 1 0 1 output-2.sym
{
T 47400 48100 5 10 0 0 0 6 1
net=Iron_EN:1
T 48100 48600 5 10 0 0 0 6 1
device=none
T 47400 48000 5 10 1 1 0 7 1
value=Iron_EN
}
C 48300 48100 1 0 1 output-2.sym
{
T 47400 48300 5 10 0 0 0 6 1
net=SDA:1
T 48100 48800 5 10 0 0 0 6 1
device=none
T 47400 48200 5 10 1 1 0 7 1
value=SDA
}
C 48300 47700 1 0 1 output-2.sym
{
T 47400 47900 5 10 0 0 0 6 1
net=SCL:1
T 48100 48400 5 10 0 0 0 6 1
device=none
T 47400 47800 5 10 1 1 0 7 1
value=SCL
}
C 48000 43300 1 0 0 resistor-1.sym
{
T 48300 43700 5 10 0 0 0 0 1
device=RESISTOR
T 48100 43700 5 10 1 1 180 0 1
refdes=R6
T 48500 43700 5 10 1 1 180 0 1
value=100
T 48000 43300 5 10 0 1 0 0 1
footprint=0805
}
C 48300 47100 1 0 1 output-2.sym
{
T 47400 47300 5 10 0 0 0 6 1
net=Reset:1
T 48100 47800 5 10 0 0 0 6 1
device=none
T 47400 47200 5 10 1 1 0 7 1
value=Reset
}
C 55600 52500 1 0 0 input-2.sym
{
T 55600 52700 5 10 0 0 0 0 1
net=Reset:1
T 56200 53200 5 10 0 0 0 0 1
device=none
T 56100 52600 5 10 1 1 0 7 1
value=Reset
}
C 60600 52700 1 0 1 input-2.sym
{
T 60600 52900 5 10 0 0 0 6 1
net=SDA:1
T 60000 53400 5 10 0 0 0 6 1
device=none
T 60500 52800 5 10 1 1 180 1 1
value=SDA
}
C 55600 52700 1 0 0 input-2.sym
{
T 55600 52900 5 10 0 0 0 0 1
net=SCL:1
T 56200 53400 5 10 0 0 0 0 1
device=none
T 55700 52800 5 10 1 1 180 7 1
value=SCL
}
C 55600 52900 1 0 0 input-2.sym
{
T 55600 53100 5 10 0 0 0 0 1
net=Iron_EN:1
T 56200 53600 5 10 0 0 0 0 1
device=none
T 55400 53000 5 10 1 1 180 7 1
value=Iron_EN
}
C 56400 45000 1 0 0 ymfc-go802lcd.sym
{
T 55900 46300 5 10 0 1 0 0 1
footprint=ymfc-go802lcd-connector
T 56700 47100 5 10 1 1 0 0 1
refdes=D1
T 56700 45000 5 10 1 1 0 0 1
label=8x2 COG
}
C 62500 46400 1 0 1 input-2.sym
{
T 62500 46600 5 10 0 0 0 6 1
net=SDA:1
T 61900 47100 5 10 0 0 0 6 1
device=none
T 62400 46500 5 10 1 1 180 1 1
value=SDA
}
C 62500 46600 1 0 1 input-2.sym
{
T 62500 46800 5 10 0 0 0 6 1
net=SCL:1
T 61900 47300 5 10 0 0 0 6 1
device=none
T 62400 46700 5 10 1 1 180 1 1
value=SCL
}
C 59600 46200 1 0 1 input-2.sym
{
T 59600 46400 5 10 0 0 0 6 1
net=Vcc5:1
T 59000 46900 5 10 0 0 0 6 1
device=none
T 59500 46300 5 10 1 1 180 1 1
value=Vcc5
}
C 56200 45100 1 270 1 ground.sym
C 53200 44600 1 0 0 led-1.sym
{
T 54000 45200 5 10 0 0 0 0 1
device=LED
T 54000 45000 5 10 1 1 0 0 1
refdes=LED1
T 54000 45400 5 10 0 0 0 0 1
symversion=0.1
T 53700 44800 5 10 0 1 0 0 1
footprint=LED3
}
C 52300 44700 1 0 0 resistor-1.sym
{
T 52600 45100 5 10 0 0 0 0 1
device=RESISTOR
T 52500 45000 5 10 1 1 0 0 1
refdes=R7
T 52500 44500 5 10 1 1 0 0 1
value=220
T 53000 44600 5 10 0 1 0 0 1
footprint=0805
}
C 54400 44600 1 90 0 ground.sym
C 50900 44700 1 0 0 input-2.sym
{
T 50900 44900 5 10 0 0 0 0 1
net=Vcc5:1
T 51500 45400 5 10 0 0 0 0 1
device=none
T 51400 44800 5 10 1 1 0 7 1
value=Vcc5
}
C 50900 43700 1 0 0 input-2.sym
{
T 50900 43900 5 10 0 0 0 0 1
net=Iron_EN:1
T 51500 44400 5 10 0 0 0 0 1
device=none
T 50700 43800 5 10 1 1 180 7 1
value=Iron_EN
}
C 52300 43700 1 0 0 resistor-1.sym
{
T 52600 44100 5 10 0 0 0 0 1
device=RESISTOR
T 52500 44000 5 10 1 1 0 0 1
refdes=R8
T 52700 43500 5 10 1 1 0 0 1
value=220
T 52800 43800 5 10 0 1 0 0 1
footprint=0805
}
C 53200 43600 1 0 0 led-1.sym
{
T 54000 44200 5 10 0 0 0 0 1
device=LED
T 54000 44000 5 10 1 1 0 0 1
refdes=LED2
T 54000 44400 5 10 0 0 0 0 1
symversion=0.1
T 53800 43700 5 10 0 1 0 0 1
footprint=LED3
}
C 54400 43600 1 90 0 ground.sym
C 42800 38600 1 0 1 connector10-2.sym
{
T 42100 43100 5 10 1 1 0 0 1
refdes=CONN1
T 42500 43050 5 10 0 0 0 6 1
device=CONNECTOR_10
T 42500 43250 5 10 0 0 0 6 1
footprint=din6
}
C 43100 39200 1 90 0 ground.sym
C 43100 38800 1 90 0 ground.sym
C 43100 39600 1 90 0 ground.sym
C 42800 40100 1 0 0 output-2.sym
{
T 43700 40300 5 10 0 0 0 0 1
net=Temp:1
T 43000 40800 5 10 0 0 0 0 1
device=none
T 43700 40200 5 10 1 1 0 1 1
value=Temp
}
C 44200 40500 1 0 1 input-2.sym
{
T 44200 40700 5 10 0 0 0 6 1
net=Vcc24:1
T 43600 41200 5 10 0 0 0 6 1
device=none
T 43700 40600 5 10 1 1 0 1 1
value=Vcc24
}
C 42800 41300 1 0 0 output-2.sym
{
T 43700 41500 5 10 0 0 0 0 1
net=Iron:1
T 43000 42000 5 10 0 0 0 0 1
device=none
T 43700 41400 5 10 1 1 0 1 1
value=Iron
}
C 44800 42000 1 270 0 resistor-1.sym
{
T 45200 41700 5 10 0 0 270 0 1
device=RESISTOR
T 44800 42000 5 10 0 1 0 0 1
footprint=0805
T 44500 41400 5 10 1 1 0 0 1
refdes=R2
T 45000 41300 5 10 1 1 0 0 1
value=100K
}
C 44800 40900 1 270 0 resistor-1.sym
{
T 45200 40600 5 10 0 0 270 0 1
device=RESISTOR
T 44800 40900 5 10 0 1 0 0 1
footprint=0805
T 44500 40300 5 10 1 1 0 0 1
refdes=R3
T 45000 40300 5 10 1 1 0 0 1
value=100
}
C 45000 43400 1 90 1 input-2.sym
{
T 44800 43400 5 10 0 0 90 6 1
net=Vcc5:1
T 44300 42800 5 10 0 0 90 6 1
device=none
T 45100 42900 5 10 1 1 180 1 1
value=Vcc5
}
C 44700 39700 1 0 0 ground.sym
N 44900 41100 44900 40900 4
N 44900 41000 42800 41000 4
C 50200 51900 1 180 0 connector3-2.sym
{
T 49500 50200 5 10 1 1 180 6 1
refdes=U3
T 49900 50250 5 10 0 0 180 0 1
device=CONNECTOR_3
T 49900 50050 5 10 0 0 180 0 1
footprint=DPAK_STD
T 49500 51900 5 10 1 1 0 0 1
value=7805
}
C 51600 50600 1 0 1 input-2.sym
{
T 51600 50800 5 10 0 0 0 6 1
net=Vcc24:1
T 51000 51300 5 10 0 0 0 6 1
device=none
T 51100 50700 5 10 1 1 0 1 1
value=Vcc24
}
C 50500 50900 1 90 0 ground.sym
C 50200 51400 1 0 0 output-2.sym
{
T 51100 51600 5 10 0 0 0 0 1
net=Vcc5:1
T 50400 52100 5 10 0 0 0 0 1
device=none
T 51100 51500 5 10 1 1 0 1 1
value=Vcc5
}
C 60500 46800 1 0 1 input-2.sym
{
T 60500 47000 5 10 0 0 0 6 1
net=Vcc5:1
T 59900 47500 5 10 0 0 0 6 1
device=none
T 60400 46900 5 10 1 1 180 1 1
value=Vcc5
}
C 58200 46800 1 0 0 resistor-1.sym
{
T 58500 47200 5 10 0 0 0 0 1
device=RESISTOR
T 58600 47200 5 10 1 1 180 0 1
refdes=R9
T 59000 47200 5 10 1 1 180 0 1
value=10K
T 58200 46800 5 10 0 1 90 0 1
footprint=0805
}
N 61100 46500 58200 46500 4
N 61100 46700 58200 46700 4
C 63200 47200 1 0 1 input-2.sym
{
T 63200 47400 5 10 0 0 0 6 1
net=Vcc5:1
T 62600 47900 5 10 0 0 0 6 1
device=none
T 63100 47300 5 10 1 1 180 1 1
value=Vcc5
}
C 60900 47200 1 0 0 resistor-1.sym
{
T 61200 47600 5 10 0 0 0 0 1
device=RESISTOR
T 61300 47600 5 10 1 1 180 0 1
refdes=R10
T 61700 47600 5 10 1 1 180 0 1
value=4.7K
T 60900 47200 5 10 0 1 90 0 1
footprint=0805
}
C 63200 45900 1 0 1 input-2.sym
{
T 63200 46100 5 10 0 0 0 6 1
net=Vcc5:1
T 62600 46600 5 10 0 0 0 6 1
device=none
T 63100 46000 5 10 1 1 180 1 1
value=Vcc5
}
C 60900 45900 1 0 0 resistor-1.sym
{
T 61200 46300 5 10 0 0 0 0 1
device=RESISTOR
T 61300 46300 5 10 1 1 180 0 1
refdes=R11
T 61700 46300 5 10 1 1 180 0 1
value=4.7K
T 60900 45900 5 10 0 1 90 0 1
footprint=0805
}
N 60800 46700 60800 47300 4
N 60800 47300 60900 47300 4
N 60800 46500 60800 46000 4
N 60800 46000 60900 46000 4
