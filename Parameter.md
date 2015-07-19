# Parameter #

|Nr|Name|Default|Bemerkung|
|:-|:---|:------|:--------|
|1 |KP\_QUAD\_ROLL|4.0    |PIPrate für ACC Mode|
|2 |KI\_QUAD\_ROLL|0.15   |PIPrate für ACC Mode|
|3 |STABLE\_MODE\_KP\_RATE\_ROLL|1.2    |PIPrate für ACC Mode|
|4 |KP\_QUAD\_NICK|4.0    |PIPrate für ACC Mode|
|5 |KI\_QUAD\_NICK|0.15   |PIPrate für ACC Mode|
|6 |STABLE\_MODE\_KP\_RATE\_NICK|1.2    |PIPrate für ACC Mode|
|7 |KP\_QUAD\_YAW|3.0    |PIPrate für ACC Mode|
|8 |KI\_QUAD\_YAW|0.15   |PIPrate für ACC Mode|
|9 |STABLE\_MODE\_KP\_RATE\_YAW|2.4    |PIPrate für ACC Mode|
|10|STABLE\_MODE\_KP\_RATE|0.2    |NOT USED NOW|
|11|acc\_offset\_x|2057   |ACC Neutralwert Roll|
|12|acc\_offset\_y|2044   |ACC Neutralwert Nick|
|13|acc\_offset\_z|2052   |ACC Neutralwert Z (wird um 407 korrigiert bei Autocalibrierung)|
|14|Kp\_ROLLNICK|0.14   |ACC Korrekturfaktor P in DCM|
|15|Ki\_ROLLNICK|0.15   |ACC Korrekturfaktor I in DCM|
|16|Kp\_YAW|1.0    |Kompass Korrekturfaktor P in DCM|
|17|Ki\_YAW|0.2    |Kompass Korrekturfaktor I in DCM|
|18|XMITFACTOR|0.32   |Drehfaktor für HH Mode|
|19|Kp\_RateRoll?|1.95   |PID für HH Mode|
|20|Ki\_RateRoll?|0.0    |PID für HH Mode|
|21|Kd\_RateRoll?|0.0    |PID für HH Mode|
|22|Kp\_RateNick?|1.95   |PID für HH Mode|
|23|Ki\_RateNick?|0.0    |PID für HH Mode|
|24|Kd\_RateNick?|0.0    |PID für HH Mode|
|25|Kp\_RateYaw?|3.2    |PID für HH Mode|
|26|Ki\_RateYaw?|0.0    |PID für HH Mode|
|27|Kd\_RateYaw?|0.0    |PID für HH Mode|
|28|roll\_mid|1520   |Neutralwert RC Roll|
|29|nick\_mid|1500   |Neutralwert RC Roll|
|30|yaw\_mid|1522   |Neutralwert RC Roll|
|31|MIN\_PITCH|1178   |min Wert für Pitch|
|32|BAT\_WARNING|10.0   |Schwellwert in Volt|
|33|STICK\_TO\_ANGLE\_FACTOR|7.0    |Stick Faktor (divisor)|
|34|YAW\_STICK\_TO\_ANGLE\_FACTOR|1150.0 |Stick Faktor (divisor)|
|35|TUNING\_PARA|0      |Parameter der verändert werden soll (2 gleichzeitig möglich aabb)|
|36|TUNING\_MIN|0      |min Parameter Wert bei Remote 1000 Aux2|
|37|TUNING\_MAX|0      |max Parameter Wert bei Remote 2000 Aux2|
|38|CHANNEL\_ASSIGNMENT|3102   |8 Digits for each channel (Roll,Nick,Yaw,Pitch)|
|39|MIN\_GAS|60     |Map from 1000-2000 to 0-200|
|40|GYRO\_GAIN\_X|0.9    |         |
|41|GYRO\_GAIN\_Y|0.9    |         |
|42|GYRO\_GAIN\_Z|0.9    |         |
|43|MAX\_MOTOR\_OUTPUT|200    |         |
|44|MOT\_CONF|0      |0 = + (front pointing towards front motor), 1 = X (front between front and right motor), 2 = X (front pointing towards front motor)|

![http://2.bp.blogspot.com/-PeuMfhxVw6M/Te-d-XUPJkI/AAAAAAAAAUc/HEWFNnJOJcQ/s1600/cm3_3.jpg](http://2.bp.blogspot.com/-PeuMfhxVw6M/Te-d-XUPJkI/AAAAAAAAAUc/HEWFNnJOJcQ/s1600/cm3_3.jpg)