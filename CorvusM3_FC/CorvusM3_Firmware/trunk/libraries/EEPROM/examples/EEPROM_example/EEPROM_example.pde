#include <EEPROM.h>

int ledPin =  21;    // LED connected to digital pin 21 - PC13
const char HELP_MSG[] = "Press :\r\n" \
			" 0 display configuration\r\n" \
			" 1 set configuration to 0x801F000 / 0x801F800 / 0x400 (RB MCU)\r\n" \
			" 2 set configuration to 0x801F000 / 0x801F800 / 0x800 (ZE/RE MCU)\r\n" \
			" 3 write/read variable\r\n" \
			" 4 increment address\r\n" \
			" 5 display pages top/bottom\r\n" \
			" 6 initialize EEPROM\r\n" \
			" 7 format EEPROM\r\n";
uint16 DataWrite = 0;
uint16 AddressWrite = 0x10;

void setup()
{
	// initialize the digital pin as an output:
	pinMode(ledPin, OUTPUT);
  Serial1.begin(115200);
	Serial1.print(HELP_MSG);
}

void loop()
{
	uint16 Status;
	uint16 Data;

	while (Serial1.available())
	{
		char cmd = (char)Serial1.read();
		Serial1.println(cmd);
		if (cmd == '0')
		{
			DisplayConfig();
		}
		else if (cmd == '1')
		{
			EEPROM.PageBase0 = 0x801F000;
			EEPROM.PageBase1 = 0x801F800;
			EEPROM.PageSize  = 0x400;
			DisplayConfig();
		}
		else if (cmd == '2')
		{
			EEPROM.PageBase0 = 0x801F000;
			EEPROM.PageBase1 = 0x801F800;
			EEPROM.PageSize  = 0x800;
			DisplayConfig();
		}
		else if (cmd == '3')
		{
			Status = EEPROM.write(AddressWrite, DataWrite);
			Serial1.print("EEPROM.write(0x");
			Serial1.print(AddressWrite, HEX);
			Serial1.print(", 0x");
			Serial1.print(DataWrite, HEX);
			Serial1.print(") : Status : ");
			Serial1.println(Status, HEX);

			Status = EEPROM.read(AddressWrite, &Data);
			Serial1.print("EEPROM.read(0x");
			Serial1.print(AddressWrite, HEX);
			Serial1.print(", &..) = 0x");
			Serial1.print(Data, HEX);
			Serial1.print(" : Status : ");
			Serial1.println(Status, HEX);

			++DataWrite;
		}
		else if (cmd == '4')
		{
			++AddressWrite;
		}
		else if (cmd == '5')
		{
			DisplayPages(0x20);
			DisplayPagesEnd(0x20);
		}
		else if (cmd == '6')
		{
			Status = EEPROM.init();
			Serial1.print("EEPROM.init() : ");
			Serial1.println(Status, HEX);
			Serial1.println();
		}
		else if (cmd == '7')
		{
			Status = EEPROM.format();
			Serial1.print("EEPROM.format() : ");
			Serial1.println(Status, HEX);
		}
		else
			Serial1.print(HELP_MSG);
	}
	digitalWrite(ledPin, HIGH);
	delay(500);
	digitalWrite(ledPin, LOW);
	delay(500);
}

void DisplayConfig(void)
{
	Serial1.print  ("EEPROM.PageBase0 : 0x");
	Serial1.println(EEPROM.PageBase0, HEX);
	Serial1.print  ("EEPROM.PageBase1 : 0x");
	Serial1.println(EEPROM.PageBase1, HEX);
	Serial1.print  ("EEPROM.PageSize  : 0x");
	Serial1.print  (EEPROM.PageSize, HEX);
	Serial1.print  (" (");
	Serial1.print  (EEPROM.PageSize, DEC);
	Serial1.println(")");
}

void DisplayHex(uint16 value)
{
	if (value <= 0xF)
		Serial1.print("000");
	else if (value <= 0xFF)
		Serial1.print("00");
	else if (value <= 0xFFF)
		Serial1.print("0");
	Serial1.print(value, HEX);
}

void DisplayPages(uint32 endIndex)
{
	Serial1.println("Page 0     Top         Page 1");

	for (uint32 idx = 0; idx < endIndex; idx += 4)
	{
		Serial1.print  (EEPROM.PageBase0 + idx, HEX);
		Serial1.print  (" : ");
		DisplayHex(*(uint16*)(EEPROM.PageBase0 + idx));
		Serial1.print  (" ");
		DisplayHex(*(uint16*)(EEPROM.PageBase0 + idx + 2));
		Serial1.print  ("    ");
		Serial1.print  (EEPROM.PageBase1 + idx, HEX);
		Serial1.print  (" : ");
		DisplayHex(*(uint16*)(EEPROM.PageBase1 + idx));
		Serial1.print  (" ");
		DisplayHex(*(uint16*)(EEPROM.PageBase1 + idx + 2));
		Serial1.println();
	}
}

void DisplayPagesEnd(uint32 endIndex)
{
	Serial1.println("Page 0     Bottom      Page 1");

	for (uint32 idx = EEPROM.PageSize - endIndex; idx < EEPROM.PageSize; idx += 4)
	{
		Serial1.print  (EEPROM.PageBase0 + idx, HEX);
		Serial1.print  (" : ");
		DisplayHex(*(uint16*)(EEPROM.PageBase0 + idx));
		Serial1.print  (" ");
		DisplayHex(*(uint16*)(EEPROM.PageBase0 + idx + 2));
		Serial1.print  ("    ");
		Serial1.print  (EEPROM.PageBase1 + idx, HEX);
		Serial1.print  (" : ");
		DisplayHex(*(uint16*)(EEPROM.PageBase1 + idx));
		Serial1.print  (" ");
		DisplayHex(*(uint16*)(EEPROM.PageBase1 + idx + 2));
		Serial1.println();
	}
}