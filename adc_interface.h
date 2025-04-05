#ifndef _ADT_ADC_H_
#define	_ADT_ADC_H_

void AD_Init();
//Pre: whichChannel is within the range of 0 and 12.
//Post: Prepares the microcontroller to acquire from the channel 
//      whichChannel.

void AD_StartConversion(void);
//Pre: --
//Post: Gives the command to the microcontroller to start the conversion.

void selectChannel(unsigned char whichChannel);

char AD_SampleAvailable(void);
//Pre: -- 
//Post: Returns 1 if the conversion is done, 0 otherwise.

int AD_GetSample_10bits(void);
//Pre: AD_SampleAvailable().
//Post: Returns the value of the last converted sample.

unsigned char AD_GetSample_8bits(void);



#endif	

