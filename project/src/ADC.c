#include "lpc24xx.h"
#include "sensor.h"

extern short read_data(short currData);

// Initialize ADC.
void setup_ADC(void) {
	
	// Setup Pin Connect for AD0[2] as input
	PINSEL1 |= (3 << 18); // sets bits 18 and 19 to 1
	PINSEL1 &= (2 << 18); // sets bit 18 to 0 and 19 to 1   
}

// currData: 16 bits
// bit 15 -> sensor number
// bit 14:0 -> data
short read_data(short currData) {
    int newState = FSM_state_transfer(currData);
    power_up_sensor(newState);
    // power up A/D & start
    AD0CR = 0x09200F04;
    // wait until data ready
    while (AD0DR0 & (1 << 31)); 
    // read the data


}