#include "lpc24xx.h"
#include "timer.h"
#include "sensor.h"
#include <stdlib.h>

#define READING_PERIOD 300 // 300s -> 5min
#define MAX_DATA 105120 // data for 1 year

extern void sdelay(unsigned int delay_in_s);
extern void setup_ADC(void);
extern void setup_sensors(void);
extern short read_data(short currData);


int main(void) {
	short *data = calloc(MAX_DATA, sizeof(short));
	int dataNum = 0; // total data in memory
	short currData = 0;
	
	// Setup the ADC 
	setup_ADC();
	// Setup sensors
	setup_sensors();
	
	while(1) {
		// delay for 5 min
		sdelay(READING_PERIOD);
		// read data
		currData = read_data(currData);
		// store data
		data[dataNum] = currData;
		dataNum = (dataNum < (MAX_DATA - 1)) ? dataNum++ : 0;
	}
}
