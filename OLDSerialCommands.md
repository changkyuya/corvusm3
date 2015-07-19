# Serial Commands - OLD #

## Input ##
Nach jeder Eingabe CR LF (\r\n) - Befehle maximal 80 Zeichen!

**Commands**
  * ca ... set ACC Zero
  * cg ... set Gyro Zero

**Debug**
  * d0 ... Debug off
  * dr ... Toggle Receiver on/off
  * da ... Toggle Sensor RAW on/off
  * ds ... Toggle Sensor Data on/off
  * dm ... Target Angles - Motors on/off
  * dc ... Toggle Compass Raw Values and Degree on/off
  * dp ... Toggle PID Werte

**Parameter SET**<br />
  * sa ... Speichern der ACC-Ebenenwerte (auch im Flash)
Format: s 00(-99, 2-stelliger Parameter) : 12345 (0 bis 65535) \r\n
  * s00:0 ... Parameterset 0 oder 100 (Set 2)
  * s01:12345 ... Debug (1.Receiver,)
**Parameter FLASH**<br />
Format: f 00(-99, 2-stelliger Parameter) : 12345 (0 bis 65535) \r\n
  * f00:0 ... Parameterset 0 oder 100 (Set 2)
  * f01:12345 ... Debug (1.Receiver,)
**Parameter PRINT**<br />
Format: p 00(-99, 2-stelliger Parameter)
  * p00 ... Parameterset 0 oder 100 (Set 2)
  * p01 ... Debug (1.Receiver,)
**Parameter READ from FLASH**<br />
Format: r 00(-99, 2-stelliger Parameter)
  * [r00](https://code.google.com/p/corvusm3/source/detail?r=00) ... Parameterset 0 oder 100 (Set 2)
  * [r01](https://code.google.com/p/corvusm3/source/detail?r=01) ... Debug (1.Receiver,)

**Spezial Commands**
  * pa ... Print all Parameter
  * # ... lade Standard Parameter und schreibe in Flash
  * `*` ... speichert alle Parameter ins Flash
  * ~1234:1234:1234:1234 ... RC Werte über PC (4CH)
  * <010:150:120:200 ... Motor Werte 1-4 000-200
  * = ... UART1 <=> UART3
  * ? ... Help

## Output ##

### Debug ###
**Receiver**<br />
Output all 100ms, Status 1=OK, CH Bereich 1000-2000<br />
`R-conf-[0/1-PPM/Spektrum]:Status 0/1:[CH1]:[CH2]:[CH3]:[CH4]:[CH5]:[CH6]:[CH7]:[CH8]:\r\n`<br />
**Sensor**<br />
Output all 100ms, 0-3,3V 12 Bit Aufloesung 0-4095<br />
`G-Gyro-ACC:1:[Gyro X]:[Gyro Y]:[Gyro Z]:[ACC X]:[ACC Y]:[ACC Z]:[VOLT]:\r\n`<br />