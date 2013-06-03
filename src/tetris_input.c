//
//  tetris_input.c
//  Tetris
//
//  Created by Blake on 29/05/13.
//
//

#include <stdio.h>
#include "keypad.h"
#include "serialio.h"
#include "external_interrupt.h"

#ifdef AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#endif

char tetris_input_read(void){
    char input = 0;
    
    /* Check for button pushes and/or serial input. */
    input = keypad_button_pushed();
    if(!input) {
        input = button_pushed();
        if(input >= 0 && input <= 3) {
            /* Button was pushed (0 to 3) - turn these into lower case
             * characters, i.e. 0 to 'a', 1 to 'b' etc.
             */
            input += 97;	/* ASCII code for 'a' */
        } else {
            /* No button was pushed */
            input = 0;
        }
    }
    if(!input && serial_input_available()) {
        /* No keypad or push button was pushed, but serial input is
         * available. Fetch it. We can use the serial port
         * to emulate the push buttons etc below.
         */
        input = fgetc(stdin);
        clear_serial_input_buffer();
    }
    
    return input;
}