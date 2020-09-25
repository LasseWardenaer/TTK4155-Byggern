#include "interrupt.h"
#include <avr/io.h>

#define TOP 79


void interrupt_joystick_init() {
    //joystick
    // enable INT1 interrupt vector
    GICR |= (1 << INT1);

    // interrupt on falling edge
    MCUCR |= (1 << ISC11);
    MCUCR &= ~(1 << ISC10);

    // set INT1 as input
    DDRD &= ~(1 << PD3);
}


void interrupt_timer_init(){
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    // set compare match register for 60hz
    OCR1A = 2000;
    
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);

    // set prescaler 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // enable timer compare interrupt
    TIMSK |= (1 << OCIE1A);
}
