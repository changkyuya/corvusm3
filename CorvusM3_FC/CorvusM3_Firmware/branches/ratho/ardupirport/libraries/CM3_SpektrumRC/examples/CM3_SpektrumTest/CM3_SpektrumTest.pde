#include <CM3_SpektrumRC.h>

CM3_SpektrumRC reciver;
int RCPin = 39; //spektrum bind pin

void setup(){
  disableDebugPorts();  
  pinMode(RCPin, OUTPUT);   
  digitalWrite(RCPin, HIGH);
  
  Serial1.begin(115200); 
  Serial2.begin(115200); 
}

void loop()
{
  while (Serial2.available()) {
    reciver.regByte(Serial2.read());
  }
  
  delay(2); // simulate mainloop
  
    Serial1.print(reciver.getThro());
    Serial1.print("\t");
    Serial1.print(reciver.getRoll());
    Serial1.print("\t");
    Serial1.print(reciver.getPitch());
    Serial1.print("\t");
    Serial1.print(reciver.getYaw());
    Serial1.println();

  

  
}