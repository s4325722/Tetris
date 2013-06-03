/*
 * project.c - main file 
 *
 * Original version by Peter Sutton. Modified by <YOUR NAME(S) HERE>
 */

#include "keypad.h"
#include "timer2.h"
#include "score.h"
#include "external_interrupt.h"
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "serialio.h"
#include "terminalio.h"

#ifdef AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#else
#include <unistd.h>
/* For non AVR build, we remove our interrupt enable */
#define sei(arg)
#endif

#include "tetris.h"
#include "tetris_input.h"

/*
 * Function prototypes - these are defined below main()
*/
void initialise_hardware(void);
void timer_activities(void);
void splash_screen(void);

uint32_t ts = 0;
tetris_game* pGame = NULL;

/*
 * main -- Main program.
 */
int main(void) {
#ifndef AVR
    sleep(5);
#endif
	initialise_hardware();
	splash_screen();
    //hide_cursor();
    //clear_terminal();
    
    pGame = tetris_game_create();
    
    (void)button_pushed();
	(void)keypad_button_pushed();
	clear_serial_input_buffer();
    
    while(1){
        tetris_game_run(pGame);
        //tetris_game_display(pGame);
        
#ifndef AVR
        if(get_clock_ticks() - ts > 1000 || pGame->updated){
            ts = get_clock_ticks();
            tetris_game_display(pGame);
        }
#endif
    }
}

/* Function to be called on every interrupt of timer 2 (every ms). Note that
 * interrupts are still disabled when this function is called.
 */
void timer_activities(void) {
	
	/* Check the next column of our keypad for any button pushes. */
	check_keypad_column();
    
    if(pGame != NULL)
         tetris_game_display(pGame);
}

void initialise_hardware(void) {
	/* Initialise hardware modules (interrupts, data direction
	 * registers etc. This should only need to be done once.
	 */
	
	/* Initialise serial IO - baud rate of 19200 and no echo */
	init_serial_stdio(19200, 0);

	/* Initialise the LED board display */
	//init_led_display();
	
	/* Initialise the keypad. We don't provide a handler, we'll
	 * rely on asking the keypad module for the last button pushed.
	 */
	init_keypad(NULL);

	/* Initialise the timer which gives us clock ticks
	 * to time things by. We specify that the timer_activities()
	 * function above should be called on every interrupt.
	 */
	init_timer2(timer_activities);
	
	/* Initialise our external button interrupts */
	init_external_interrupts();
	
	/*
	 * Turn on interrupts (needed for timer, serialIO etc. to work)
	 */
	sei();
}

void splash_screen(void) {
	//clear_terminal();
	printf("TETRIS - CSSE2010/CSSE7201 Project\n");
}


