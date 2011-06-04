#include <CM3_ADC.h> 

unsigned long timer;

CM3_ADC adc;

void setup()
{  
  adc.Init();   // CM3 ADC initialization
  Serial1.begin(115200);
  Serial1.println("CM3 ADC library test");
  delay(1000);
  timer = millis();
}

void loop()
{
  int ch;
  
  if((millis()- timer) > 20)
    {
    timer = millis();
    for (ch=0;ch<7;ch++)
      {
      Serial1.print(adc.Ch(ch));
      Serial1.print(",");
      }
    Serial1.println();
    }
}

