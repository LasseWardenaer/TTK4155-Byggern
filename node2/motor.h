#ifndef MOTOR_H
#define MOTOR_H


#include <stdint.h>


#define DIR         PIO_PD10
#define EN          PIO_PD9
#define SEL         PIO_PD2
#define NOT_RST     PIO_PD1
#define NOT_OE      PIO_PD0

#define DO0_IDX     1

#define MOTOR_TIMER_FREQ    50


void motor_init(void);


void motor_disable(void);


void motor_enable(void);


void motor_run_slider(int reference);


int motor_read_encoder(void);


void motor_run_microbit();

#endif
