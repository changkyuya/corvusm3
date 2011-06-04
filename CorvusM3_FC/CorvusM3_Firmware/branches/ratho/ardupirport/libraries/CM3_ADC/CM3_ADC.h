#ifndef CM3_ADC_H
#define CM3_ADC_H


class CM3_ADC
{
  public:
	CM3_ADC();  // Constructor
	virtual void Init();
	virtual int Ch(unsigned char ch_num);     
    int Update(void); 
  private:
   int analog_port[10];
};



#endif
