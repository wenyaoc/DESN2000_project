#include "lpc24xx.h"

// Initialize DAC.
// This should be the same as the previous exercise.
void setup_DAC(void) {
    // Setup Pin Connect for AOUT
    PINSEL1 |= (3 << 20); // sets bits 21 and 20 to 1
    PINSEL1 &= (2 << 20); // sets bit 21 to 1 and 20 to 0

    // port 0, pin 26 should output an analogue signal
    FIO0PIN |= (1 << 26);
}
