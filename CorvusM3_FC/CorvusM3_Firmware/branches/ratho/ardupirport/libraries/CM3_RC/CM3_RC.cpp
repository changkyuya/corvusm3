#include "CM3_RC.h"

#include "WProgram.h"
#define PPMSUM_IN 6 ///TR: Change PA8
volatile unsigned char radio_status_rc=0;
volatile unsigned char sync=0;
static int analogOutPin[12];
volatile unsigned int currentChannel = 0;
static unsigned int last = 0;


//void rxInt(void);

// ******************
// rc functions
// ******************
#define MINCHECK 1100
#define MAXCHECK 1900

volatile uint16_t rcPinValue[12] = {0,0,1000,0,1500,1500,1500,1000,0,0,0,0};; // interval [1000;2000]
//static uint16_t rcData[12] ; // interval [1000;2000]
//static int16_t rcCommand[12] = {0,0,0,0,0}; // interval [-500;+500]
//static int16_t rcHysteresis[12] ;
//static int16_t rcData4Values[12][4];

//static float rcRate;
//static float rcExpo;

// ***PPM SUM SIGNAL***
static uint8_t rcChannel[12] = {PPM_ORDER};
volatile uint16_t rcValue[12] = {1500,1500,1500,1500,1500,1500,1500,1500,1500,1500,1500,1500}; // interval [1000;2000]


void rxInt(void) {
volatile unsigned int now;
volatile unsigned int diff;
//int i;

  //Serial1.print("."); 
  now = micros();
  diff = now - last;
  last = now;
  //Serial.println(diff);
  //diff=diff*4; // Se si usa il timer0 come pwm in uscita e quindi con un altro coefficiente di equivalenza  che e' di 4* se si usa micros e penso /4 con delay per avere lo stesso valore.
  //Serial.println(diff);
  if(diff>3000  &&  diff<21000) // Sincro del frame 
      {
      currentChannel = 0;
	  radio_status_rc=0;
	  rcValue[currentChannel] = diff;
      currentChannel++;
	  sync=1;
      }
  else 
  	 //if(diff<=2200 && diff>=900) 
     if (sync==1) 
     {
          rcValue[currentChannel] = diff;
          currentChannel++;
		  if(diff<=2200 && diff>=900)radio_status_rc++;
	}
	 if (currentChannel>8)
	 {
		 //currentChannel=0;	
		 sync=0;
		 radio_status_rc=0;
	 }
}


// Constructors ////////////////////////////////////////////////////////////////

CM3_RC_Class::CM3_RC_Class()
{
}

// Public Methods //////////////////////////////////////////////////////////////
void CM3_RC_Class::Init(void)
{
  analogOutPin[MOTORID1] = 12; ///TR: Change // Connected to PPM output1 // TIMER2
  analogOutPin[MOTORID2] = 13; ///TR: Change // Connected to PPM output2 // TIMER2
  analogOutPin[MOTORID3] = 14; ///TR: Change //  Connected to PPM output3 // TIMER1
  analogOutPin[MOTORID4] = 15; ///TR: Change //  Connected to PPM output4 // TIMER1

  
  pinMode(analogOutPin[MOTORID1],PWM);
  pinMode(analogOutPin[MOTORID2],PWM);
  pinMode(analogOutPin[MOTORID3],PWM);
  pinMode(analogOutPin[MOTORID4],PWM);

  pinMode(PPMSUM_IN,INPUT);
  attachInterrupt(PPMSUM_IN, rxInt, RISING);
}

void CM3_RC_Class::OutputCh(unsigned char ch, uint16_t pwm)
{

  //pwm=map(pwm, 1000, 2000, 0, 200);
  
  analogWrite(analogOutPin[ch], pwm);

}

uint16_t CM3_RC_Class::InputCh(unsigned char ch)
{
  uint16_t data;
  data = rcValue[rcChannel[ch]]; 
  return data; // We return the value correctly copied when the IRQ's where disabled
}

unsigned char CM3_RC_Class::GetState(void)
{
  return(radio_status_rc);
}

// make one instance for the user to use
CM3_RC_Class CM3_RC;
