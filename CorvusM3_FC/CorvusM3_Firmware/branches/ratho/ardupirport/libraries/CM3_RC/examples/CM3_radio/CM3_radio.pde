#include <CM3_RC.h> // ArduPilot Mega RC Library
int RCPin = 39;

void setup()
{
  disableDebugPorts();  
  pinMode(RCPin, OUTPUT);   
  digitalWrite(RCPin, HIGH);
  
	CM3_RC.Init();	 // CM3 Radio initialization
	Serial1.begin(115200);
	Serial1.println("CM3 RC library test");
	delay(1000);
}

void loop()
{
	// New radio frame? (we could use also if((millis()- timer) > 20)
	if (CM3_RC.GetState() == 1){
		Serial1.print("CH:");
		for(int i = 0; i < 8; i++){
			Serial1.print(CM3_RC.InputCh(i));	// Print channel values
			Serial1.print(",");
			CM3_RC.OutputCh(i, CM3_RC.InputCh(i)); // Copy input to Servos
		}
		Serial1.println();
	}
}