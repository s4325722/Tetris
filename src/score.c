/*
 * score.c
 *
 * Written by Peter Sutton
 */

#include "score.h"
#ifdef AVR
#include "seven_seg_display.h"
#endif

uint16_t score;

void init_score(void) {
	score = 0;
#ifdef AVR
    seven_seg_set(0);
#endif
}

void add_to_score(uint16_t value) {
	score += value;
#ifdef AVR
    seven_seg_set(score);
#endif
}

uint16_t get_score(void) {
	return score;
}
