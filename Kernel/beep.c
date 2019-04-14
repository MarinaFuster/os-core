/* Beeper functions*/
#include <stdint.h>
void _beep_start(uint16_t freq);
void _beep_stop();

void beep(uint32_t nFrequence, unsigned char duration) {
_beep_start(1193180 / nFrequence);
}

void nosound() {
_beep_stop();
}
