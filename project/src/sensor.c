#include "lpc24xx.h"
#include <math.h>
#define SENSOR0_STATE 0
#define SENSOR1_STATE 1

#define SENSOR0_MAX_LUMIN 9900
#define SENSOR1_LOW_LUMIN 1250

#define get_intensity(x) (exp((log(x) + 0.539) / 0.6393))


// Initialize sensors.
void setup_sensors(void) {
	
    // GPIO as default
    // set p0[0] and p0[1] as output
		// use for the 3.3v power supply
		FIO0DIR |= 3;  
}

// Initialize ADC.
void setup_ADC(void) {
	
		// Setup Pin Connect for AD0[2] as input
		PINSEL1 |= (3 << 18); // sets bits 18 and 19 to 1
		PINSEL1 &= (2 << 18); // sets bit 18 to 0 and 19 to 1   
}


// power up sensor
void power_up_sensors(int state) {
		// FSM state transfer
		// if current intensity is low and sensor = 1 -> change to sensor0
		// if current intensity is high and sensor = 0 -> change to sensor1
		// otherwise remain the state unchanged
    switch (state)
    {
        case SENSOR0_STATE:
            FIO0SET |= 1; //power_up_sensor0
            break;
        case SENSOR1_STATE:
            FIO0SET |= 2; //power_up_sensor1
            break;
        default:
            break;
    }
}


// shut down sensor
void shut_down_sensors() {
    
    FIO0CLR |= 3; //shut down both sensors
}


int FSM_state_transfer(short currData) {
    // clear the top bit to get the data
    short data = (currData << 1) >> 1;
    // get the top bit (state bit)
    int state = (currData & (1 << 15)) >> 15;
		switch (state)
    {
        case SENSOR0_STATE:
            if (data >= SENSOR0_MAX_LUMIN) state = SENSOR1_STATE;
            break;
        case SENSOR1_STATE:
            if (data <= SENSOR1_LOW_LUMIN) state = SENSOR0_STATE;
            break;
        default:
            break;
    }
    return state;
}




// currData: 16 bits
// bit 15 -> sensor number
// bit 14:0 -> data
short read_data(short currData) {
		short newData = 0;
		double temp = 0;
		// get the new state
    int newState = FSM_state_transfer(currData);
		// power up sensor
    power_up_sensors(newState);
    // power up AD0.2 & start
		// CLOCLDIV = 0x0F, sofeware controlled, operational, start, falling edge
    AD0CR = 0x09200F04;
    // wait until data ready (top bit == 1)
    while (AD0DR2 >> 31); 
    // read the data (bit 6:16)
    newData = (short) (((AD0DR2 << 16) >> 22));
		temp = ((double)newData / ((2 << 12) - 1));
		temp *= 3.3;
		newData = get_intensity(temp);
		// shut down AD0.2
		AD0CR = 0x09000F04;
		// shut down sensor
		shut_down_sensors();
    return newData & (newState << 15);
}
