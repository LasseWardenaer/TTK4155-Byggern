#include "game.h"
#include "adc.h"
#include "user_input.h"
#include "motor.h"
#include "servo_driver.h"
#include "solenoid.h"
#include "sam/sam3x/include/sam.h"
#include <stdint.h>
#include "../node1/user_input.h"


#define F_OSC           84E6
#define TC0_CLK0        F_OSC / 2
#define FREQ            100

#define IR_TRESHOLD     1000

#define IRQ_TC0_priority 2


static unsigned int score;
static unsigned int counting_flag;
unsigned int controller_select = 0;


static struct user_input_data {
    int joystick_x;
    int joystick_y;
    int slider_left;
    int slider_right;
    int button_left;
    int button_right;
} user_data;


void game_timer_init() {
    // initiate TC0 channel 0
    // enable clock for TC0:    DIV = 0 (clk = MCK), CMD = 0 (read), PID = 27 (TC0)
    PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_TC0 << PMC_PCR_PID_Pos);
    PMC->PMC_PCER0 |= 1 << (ID_TC0);

    // enable timer counter channel
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;

    // set clock to MCK/2 = 42 MHz, capture mode with reset trigger on compare match with RC
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 | TC_CMR_CPCTRG;

    // set match frequency to 100 Hz
    TC0->TC_CHANNEL[0].TC_RC = TC0_CLK0 / FREQ;

    // enable RC compare match interrupt
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;

    // enable NVIC interrupt
    NVIC_EnableIRQ(ID_TC0);
    NVIC_SetPriority(TC0_IRQn, IRQ_TC0_priority);

    // TESTING FREQUENCY
    PIOA->PIO_PER |= PIO_PA16;
    PIOA->PIO_OER |= PIO_PA16;
}


void game_init() {
    score = 0;
    game_timer_init();
    game_timer_disable();
}


void game_set_controller(unsigned int controller){
    controller_select = controller;
}


void game_count_score() {
    uint16_t ir_level = adc_read();

    if ((ir_level < IR_TRESHOLD) && !counting_flag) {
        ++score;
        counting_flag = 1;
    }

    else if (ir_level > IR_TRESHOLD) {
        counting_flag = 0;
    }

    printf("Current score: %d \r\n", score);
}


void game_set_user_data(char* data) {
    user_data.joystick_x = joystick_scale_x(data[0]);
    user_data.joystick_y = joystick_scale_y(data[1]);
    user_data.slider_left = slider_scale_left(data[2]);
    user_data.slider_right = slider_scale_right(data[3]);
    user_data.button_left = data[4];
    user_data.button_right = data[5];
}


static void game_run() {
    printf("x: %d \r\n", user_data.joystick_x);
    if(controller_select == USE_JOYSTICK){
        motor_run_slider(user_data.slider_right);
        servo_set_position(user_data.joystick_x);
        solenoid_run_button(user_data.button_right);
    }
    else if(controller_select == USE_MICROBIT_CONTROLLER){
        const button_pressed = user_input_microbit_button_pressed();
        motor_run_microbit();
        servo_set_position(0);
        solenoid_run_button(button_pressed);
    }
}


void game_timer_enable(){
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void solenoid_run_microbit_button();
void game_timer_disable(){
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;
}


void TC0_Handler(void) {
    game_run();

    // clear interrupt flag
    TC0->TC_CHANNEL[0].TC_SR;
}
