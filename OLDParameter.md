# Parameter #
Möglich wären 255 Parameter - es sind vorerst 2 Sets mit je 100 Parameter geplant wobei der Parameter 00 das Startset angibt.
Es werden alle Parameter geladen und können jederzeit umgeschaltet werden.
Alle Parameter sind 16bit Integer. Mögliche Werte 0-65535.

Werden die Parameter mit einem anderen Tool eingegeben bitte führende 0 verwenden! Jeden Parameter 5 Stellig! Andernfalls kommt es öfter zu falsch lesen -> Grund wurde noch nicht gefunden.

| Nr | Parameter | Default | mögliche Werte | Info |
|:---|:----------|:--------|:---------------|:-----|
| 00 | Para-Set  | 0       | 0              | Im Moment nur 0 berücksichtigt. Dieser Parameter gibt das Startset an. Besser gesagt die Verschiebung (wenn 100 dann wäre der Parameter 01 auf 101 zu finden - also Setting 2) |
| 01 | Debugoutput | 0       | 0...Debug aus<br />1...Empfängersignale<br />2...ADC Werte (6x Sensoren, Spannungsmessung)<br />4...Sensoren (Gyro,ACC)<br />8...RC und Motoren<br />16...Kompass<br />32...PID XYZ Werte | Bitfeld Kombinationen möglich |
| 02 | HAL       | 1       | 0...RC Empfänger<br />1...PC Steuerung<br />2...Mixer Motor On<br />4...Kompass ON  | Bitfeld Kombinationen möglich<br /> PPM oder Spektrum wird automatisch erkannt. PC Steuerung nur möglich wenn kein Empfang und Parameter gesetzt ist |
| 03 | Akku      | 2236    |                | Einstellung in Punkte vom 12bit ADC. Spannungsteiler (10K auf +, 2K2 auf GND = Spannung/5,55). 1 Volt entspricht 223,64 Punte. Default = 2236 = 10 Volt. |
| 04 | SAL       | 1       | 0...HH Regelung<br />1...ACC Regelung<br />2...ACC Regelung COMP2 | Bitfeld Kombinationen möglich<br />4 bit für Regler |
| 05 | ACC-X-ZERO | 20470   |                | Horizontalwert X-Achse für ACC - ADC x 10 |
| 06 | ACC-Y-ZERO | 20330   |                | Horizontalwert Y-Achse für ACC - ADC x 10 |
| 07 | ACC-Z-ZERO | 24450   |                | Horizontalwert Z-Achse für ACC - ADC x 10 |
| 08 | SMOOTH\_ACC | 0       | 0-999          | Glättung zum letzten Wert (0 wäre immer letzter Wert)  |
| 09 | STICK\_FACTOR | 30      | 1-50           | Stick Faktor für HH und Gier |
| 10 | SMOOTH\_RC | 0       | 0-99           | Glättung zum letzten Wert (0 wäre immer letzter Wert) |
| 11 | GYRO\_X\_90 | 2200    |                | Gyro Anpassung für 90 Grad Drehung |
| 12 | GYRO\_Y\_90 | 2200    |                | Gyro Anpassung für 90 Grad Drehung |
| 13 | GYRO\_Z\_90 | 2200    |                | Gyro Anpassung für 90 Grad Drehung |
| 14 | ACC\_FORCE | 100     | 0-1000         | Wirkung vom ACC auf den Gyro (0 ist nur Gyro)<br />Bei Comp2 Filter 500 = 5Hz |
| 15 | GYRO\_CORR | 10      | 0-1000         | Berichtigung Gyro-Zero über ACC |
| 16 | SERVO0\_CH | 5       |                | Kanalmapping für Servo 0 |
| 17 | SERVO1\_CH | 6       |                | Kanalmapping für Servo 1 |
| 18 | SERVO2\_CH | 7       |                | Kanalmapping für Servo 2 |
| 19 | SERVO3\_CH | 8       |                | Kanalmapping für Servo 3 |
| 20 | CAM\_ROLL | 20500   |                | Rollausgleich für Servo, <br />1. Stelle Servo0-3, <br />2. Stelle invertiert (0,1),<br />3.-5. Faktor (500) |
| 21 | CAM\_NICK | 30500   |                | Nickausgleich für Servo, <br />1. Stelle Servo0-3, <br />2. Stelle invertiert (0,1),<br />3.-5. Faktor (500) |
| 22 | MIN\_GAS  | 32      |                | Minimalgas für Regler/Motoren |
| 23 | P\_X      | 1000    | 0-65535        | Proportionalanteil Roll |
| 24 | I\_X      |         | 0-65535        | Proportionalanteil Nick |
| 25 | D\_X      | 3000    | 0-65535        | Integralanteil Roll |
| 26 | P\_Y      | 1000    | 0-65535        | Integralanteil Nick |
| 27 | I\_Y      |         | 0-65535        | Differenzialanteil Roll |
| 28 | D\_Y      | 3000    | 0-65535        | Differenzialanteil Nick |
| 29 | P\_Z      | 1000    | 0-65535        | Proportionalanteil Gier |
| 30 | I\_Z      | 500     | 0-65535        | Integralanteil Gier |
| 31 | D\_Z      |         | 0-65535        | Differenzialanteil Gier |
| 32 | COMP\_CORR\_X |         |                | Korrekturwert Kompass X Achse |
| 33 | COMP\_CORR\_Y |         |                | Korrekturwert Kompass Y Achse |
| 34 | COMP\_CORR\_Z |         |                | Korrekturwert Kompass Z Achse |
| 35 | COMP\_DECL |         |                | Declination (Abweichung zu magnetisch Nord) Grad mit 2 Nachkomma |
| 36 | COMP\_FORCE | 500     | 0-10000        | Wirkung vom Kompass auf den Gier Gyro (0 ist nur Gyro) |
| 37 | MOTOR\_MIXER\_MOT1A | 25600   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Pitch 100/Roll 0) |
| 38 | MOTOR\_MIXER\_MOT1B | 58596   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Nick -100/Yaw -100) |
| 39 | MOTOR\_MIXER\_MOT2A | 25600   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Pitch 100/Roll 0) |
| 40 | MOTOR\_MIXER\_MOT2B | 25828   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Nick 100/Yaw -100) |
| 41 | MOTOR\_MIXER\_MOT3A | 25700   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Pitch 100/Roll 100) |
| 42 | MOTOR\_MIXER\_MOT3B | 100     |                | Motormischer 2 Werte (-100/100) je 8 Bit (Nick 0/Yaw 100) |
| 43 | MOTOR\_MIXER\_MOT4A | 25828   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Pitch 100/Roll -100) |
| 44 | MOTOR\_MIXER\_MOT4B | 100     |                | Motormischer 2 Werte (-100/100) je 8 Bit (Nick 0/Yaw 100) |
| 45 | MOTOR\_MIXER\_MOT5A | 25600   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Pitch 100 wenn Motor nicht benutzt/Roll 0) |
| 46 | MOTOR\_MIXER\_MOT5B | 0       |                | Motormischer 2 Werte (-100/100) je 8 Bit (Nick/Yaw) |
| 47 | MOTOR\_MIXER\_MOT6A | 25600   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Pitch/Roll) |
| 48 | MOTOR\_MIXER\_MOT6B | 0       |                | Motormischer 2 Werte (-100/100) je 8 Bit (Nick/Yaw) |
| 49 | MOTOR\_MIXER\_MOT7A | 25600   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Pitch/Roll) |
| 50 | MOTOR\_MIXER\_MOT7B | 0       |                | Motormischer 2 Werte (-100/100) je 8 Bit (Nick/Yaw) |
| 51 | MOTOR\_MIXER\_MOT8A | 25600   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Pitch/Roll) |
| 52 | MOTOR\_MIXER\_MOT8B | 0       |                | Motormischer 2 Werte (-100/100) je 8 Bit (Nick/Yaw) |
| 53 | MOTOR\_MIXER\_MOT9A | 25600   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Pitch/Roll) |
| 54 | MOTOR\_MIXER\_MOT9B | 0       |                | Motormischer 2 Werte (-100/100) je 8 Bit (Nick/Yaw) |
| 55 | MOTOR\_MIXER\_MOT10A | 25600   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Pitch/Roll) |
| 56 | MOTOR\_MIXER\_MOT10B | 0       |                | Motormischer 2 Werte (-100/100) je 8 Bit (Nick/Yaw) |
| 57 | MOTOR\_MIXER\_MOT11A | 25600   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Pitch/Roll) |
| 58 | MOTOR\_MIXER\_MOT11B | 0       |                | Motormischer 2 Werte (-100/100) je 8 Bit (Nick/Yaw) |
| 59 | MOTOR\_MIXER\_MOT12A | 25600   |                | Motormischer 2 Werte (-100/100) je 8 Bit (Pitch/Roll) |
| 60 | MOTOR\_MIXER\_MOT12B | 0       |                | Motormischer 2 Werte (-100/100) je 8 Bit (Nick/Yaw) |