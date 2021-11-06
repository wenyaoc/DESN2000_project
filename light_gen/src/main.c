#include "lpc24xx.h"
#include "DAC.h"
#include "timer.h"

extern void sdelay(unsigned int delay_in_s);
extern void setup_DAC(void);



int main(void) {

	int curr = 0;
	// Setup the DAC 
	setup_DAC();
	
	// change the output voltage
	while(1) {
		sdelay(600);
		// update DACR each 10 mins
		DACR = curr << 6;
		// increase counter
		curr++;
		if (curr > 0x3FF) curr = 0;
	}

}
