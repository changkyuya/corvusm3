# Regler #

als Regler können z.B. umgebaute China-Regler mit Quax-UART Software verwendet werden.

Für meinen Kopter werden die 4-Fach-Regler von Klempnertommy verwendet.
(http://www.semotech.de)

http://4.bp.blogspot.com/_6-4DWYoFTNY/TLnCW5uxhyI/AAAAAAAAARA/S7_RABqwFzg/s1600/R0014026.JPG

## Das Protokoll sieht 1-12 Regler vor ##
bei 4 Regler: **F5 00 00 00 00**<br />
F5 ... Start<br />
00 - F0 ... Motorwerte (wobei nur 0x00-0xC8 (0-200) verwendet wird, Standgas ca. 0x20 (32))

Die Regler sind dabei wie folgt nummeriert: <br />
1 ... vorne<br />
2 ... hinten<br />
3 ... rechts<br />
4 ... links