#include "lpc24xx.h"

#define PRE_SCALE (72000000-1);


// udelay should delay the processor for 'delay_in_s' number of seconds
void sdelay(unsigned int delay_in_s) {
	// 1us -> 72
	// 1ms -> 72000
	// 1s	-> 72000000 < 4294967296(MAX_INT)
	T0PR = PRE_SCALE; 
	T0MR0 = delay_in_s;
	T0MCR |= 4; 	// stop when TC = MR0	
	T0TCR = 2; 		// reset timer
	T0TCR = 1; 		// start timer;

	while ((T0TCR & 1) > 0); // check if the timer is enable
	
	T0TCR = 2; 		// reset timer	
}
	

