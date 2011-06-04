


#include "CM3_SpektrumRC.h"
#include "WProgram.h"

volatile unsigned char radio_status_spek=0;


void CM3_SpektrumRC::regByte(int byte){
    
      time = micros();
      
      spektrumByte[spektrumByteCount] = byte;
      
	//Serial1.write(spektrumByte[spektrumByteCount]);
      
	  // we have new frame
      if ((time - time_old)>5000) {
          if (spektrumByteCount >= 31) {
              spektrumByteCount = 0; 
				
			  //first 2 bytes are unknown
              for (int i = 1; i < 16; i++)
              {
				//2. frame ... first 2 bytes are unknown
                if (i != 8)
				{
					int temp = (spektrumByte[i*2] * 256) + spektrumByte[(i*2)+1];
					if (temp > 0 && temp < 1024) { channel[2] = temp + 988;} // nick
					if (temp > 1024 && temp < 2048) {channel[0] = temp - 12;} // throttle
					if (temp > 2048 && temp < 3072) {channel[1] = temp - 1036;} // roll
					if (temp > 3072 && temp < 4096) {channel[3] = temp - 2060;}  //yaw
					if (temp > 4096 && temp < 5120) {channel[4] = temp - 3084;}
					if (temp > 5120 && temp < 6144) {channel[5] = temp - 4108;}
					if (temp > 7168 && temp < 8192) {channel[6] = temp - 6156;}
					if (temp > 8192 && temp < 9216) {channel[7] = temp - 7180;}
				}
			}
              radio_status_spek = 0;
          }  
      }
      
      spektrumByteCount++;
      radio_status_spek++;      
      time_old = time;
}

int CM3_SpektrumRC::getThro(){
	return channel[0];
}

int CM3_SpektrumRC::getPitch(){
	return channel[2];
}

int CM3_SpektrumRC::getYaw(){
	return channel[3];
}

int CM3_SpektrumRC::getRoll(){
	return channel[1];
}

int CM3_SpektrumRC::getMode(){
	return channel[4];
}

int CM3_SpektrumRC::getAux1(){
	return channel[5];
}

int CM3_SpektrumRC::getAux2(){
	return channel[6];
}

int CM3_SpektrumRC::getAux3(){
	return channel[7];
}

unsigned char CM3_SpektrumRC::GetState(void)
{
  if (micros() - time_old > 50000)
  {
	radio_status_spek = 0;
  }
  return(radio_status_spek);
}