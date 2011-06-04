/*
 * SpectrumRecive.h
 *
 *  Created on: 9. jan. 2011
 *      Author: develop
 */

#ifndef CM3_SpektrumRC_H_
#define CM3_SpektrumRC_H_


class CM3_SpektrumRC{
public:
	void regByte(int byte);
	int getThro();
	int getPitch();
	int getYaw();
	int getRoll();
	int getMode();
	int getAux1();
	int getAux2();
	int getAux3();
	unsigned char GetState();
private:
	int spektrumByte[32];
  int spektrumByteCount;
  int channel[8];
  unsigned int time;
  unsigned int time_old;
};


#endif /* CM3_SpektrumRC_H_ */
