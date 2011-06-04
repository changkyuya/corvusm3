#ifndef CM3_RC_h
#define CM3_RC_h

#define NUM_CHANNELS 8
#define MIN_PULSEWIDTH 900
#define MAX_PULSEWIDTH 2100

#define MOTORID1 0		
#define MOTORID2 1		
#define MOTORID3 2		
#define MOTORID4 3		

//#define MINTHROTTLE 1300 // for Turnigy Plush ESCs 10A
#define MINTHROTTLE 1120 // for Super Simple ESCs 10A

//#define SERIAL_SUM_PPM  // for specific receiver with only one PPM sum signal, on digital PIN 2
//#define PPM_ORDER         PITCH,YAW,THROTTLE,ROLL,MODE,AUX1,AUX2,AUX3 //For Graupner/Spektrum
#define PPM_ORDER         ROLL,PITCH,THROTTLE,YAW,MODE,AUX1,AUX2,AUX3 //For Graupner/Spektrum


// alias for RC
#define ROLL 1
#define PITCH 2
#define YAW 4
#define THROTTLE 3
#define MODE 5
#define AUX1 6
#define AUX2 7
#define AUX3 8


// These constants won't change.  They're used to give names
// to the pins used:
//int analogOutPin[12];

#include <inttypes.h>

class CM3_RC_Class
{
  private:
  public:
	CM3_RC_Class();
	void Init();
	void OutputCh(unsigned char ch, uint16_t pwm);
	uint16_t InputCh(unsigned char ch);
	unsigned char GetState();
};

extern CM3_RC_Class CM3_RC;

#endif
