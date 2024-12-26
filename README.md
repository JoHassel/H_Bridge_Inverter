# H-Bridge Inverter 

## Goal: 
Basic functionality of an H-Bridge Inverter (12V DC --> 230V AC)


## Idea:
1. Microcontroller (Raspi Pico) generates a sine PWM as a control signal
2. Sine PWM goes into a gate driver (Bootstrap, one for each side) 
3. Bootstrap controls H-Bridge MOSFETs --> sine_wave AC


## Code:
* based on [sPWM_Basic/sPWM_Basic.ino](https://github.com/Irev-Dev/Arduino-Atmel-sPWM/blob/d9c89ceef080a3c18ce5a02e0e310f1f46b8f579/sPWM_Basic/sPWM_Basic.ino)


### Flowchart
![grafik](https://github.com/user-attachments/assets/0f634039-548f-4180-8023-58856f1b5842)



### Output
![PIC_9](https://github.com/user-attachments/assets/ee9a03ed-56e9-471d-a665-c2d0a5871e6e)


## Hardware:


### Why Bootstrap Circuit?
* source of high side MOSFET floats between V_AC (here: 12V AC) --> V_GS at the high side MOSFET must be high enough to safely switch on/off the MOSFET
* if low side MOSFET on, Bootstrap capacitor is chraged up to V_DC (here: 12V DC, voltage drop over bootstrap diode negligible)
* if high side MOSFET is switched on, the capacitor delivers its voltage to the control pin of the high side MOSFET

### Circuit design
* [Infineon IR2104](https://www.infineon.com/cms/de/product/power/gate-driver-ics/ir2104/) was used
* Dimensioning based on [TI Application Note](https://www.ti.com/lit/an/slua887a/slua887a.pdf?ts=1735164351528&ref_url=https%253A%252F%252Fwww.google.com%252F)
* MOSFETs: [IRFB7537](https://www.infineon.com/dgdl/Infineon-Data_Sheet_IRFS7537PBF-DS-v01_01-EN.pdf?fileId=5546d462533600a4015364c3ee2729cb)


#### Bootstrap Capacitor
$C_{boot} >= \frac{Q_{total}}{\Delta V_{HB}} = XXX$ --> [22uF 50V Elko](https://cdn-reichelt.de/documents/datenblatt/B300/A-FR_ENG_TDS.pdf)

$Q_{total} = Q_G + I_{HBS} \cdot \frac{D_{max}}{f_{sw}} + \frac{I_{HB}}{f_{sw}} $

* $Q_G = 210nC$ from [Data Sheet MOSFET](https://www.infineon.com/dgdl/Infineon-Data_Sheet_IRFS7537PBF-DS-v01_01-EN.pdf?fileId=5546d462533600a4015364c3ee2729cb)

* $I_{HBS} = 50 \mu A$ from [Data Sheet Gate Driver](https://www.infineon.com/cms/de/product/power/gate-driver-ics/ir2104/)

* $D_{max} = 1$ (will be slightly lower beacause of 520ns Dead time, 100% assumed for conservative caluclation)

* $f_{sw} = 10kHz$

* $I_ {HB} = 55 \mu A$ from [Data Sheet Gate Driver](https://www.infineon.com/cms/de/product/power/gate-driver-ics/ir2104/)



$\Delta V_{HB} = V_ {DD} − V_ {DH} − V_ {HBL} = 2V $

* $V_ {DD} = V_{DC} = 12V$

* $V_ {DH} = 1V $ from [Data Sheet 1N4148](https://www.vishay.com/docs/81857/1n4148.pdf)

* $V_{HBL} = 9V$ from [Data Sheet Gate Driver](https://www.infineon.com/cms/de/product/power/gate-driver-ics/ir2104/)




##### Bootstrap Resistor
$R_{boot} = \frac{V_{DD} - V_{Boot,Diode}}{I_{peak}} = 5.5 \Omega$ --> $5.6 \Omega$

* $V_{DD} = V_{DC} = 12V$

* $V_{Boot,Diode} = V_ {DH} = 1V $ from [Data Sheet 1N4148](https://www.vishay.com/docs/81857/1n4148.pdf)

* I_{peak} = I_{FSM} = 2A from [Data Sheet 1N4148](https://www.vishay.com/docs/81857/1n4148.pdf)



#### Gate Resistors
$R_{G,HS} = \frac{V_{Gate}}{I_{o+}} = 92 \Omega$ --> $91 \Omega$
* $V_{Gate} = V_{DC} = 12V$
* $I_{o+} >= 130mA $ from [Data Sheet Gate Driver](https://www.infineon.com/cms/de/product/power/gate-driver-ics/ir2104/)

$R_{G,LS} = \frac{V_{Gate}}{I_{o-}} = 44 \Omega$ --> $47 \Omega$
* $V_{Gate} = V_{DC} = 12V$
* $I_{o+} >= 270mA $ from [Data Sheet Gate Driver](https://www.infineon.com/cms/de/product/power/gate-driver-ics/ir2104/)


## Performance
Disclaimer: I know Micropython is not made for high performance applications, but rather for rapid prototyping and easy debuggin. I still wanted to see how far I can get with Micropython and compare it to C++. I am happy about improvement suggestions :)

| Language |	Code Version | 	f_{sin,set} in Hz | f_{sin,real} in Hz |	f_{switch,set} in kHz |

| ----------- | ----------- | ----------- | ----------- | ----------- | 

| MicroPython	| 26/12/2024	| 50	| 50	| 5 |  

| MicroPython	| 26/12/2024	| 100	| 100	| 10 |

| MicroPython	| 26/12/2024	| 500	| 105	| 50 |





## ToDos:
* use multi threading
* shorten timer callback
* Code in C++ --> compare performance to Micropython  
* Functional programming: button on MC board to activate/deactivate inverter
