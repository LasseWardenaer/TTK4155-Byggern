#include "controller.h"
#include "gpio.h"
#include "button.h"


#define THRESHOLD  200

#define PIN_0   3
#define PIN_1   2
#define PIN_2   1


void controller_init_pins(){
    GPIO->PIN_CNF[PIN_0] = 1;
    GPIO->PIN_CNF[PIN_1] = 1;
    GPIO->PIN_CNF[PIN_2] = 1;
}


const ACC_DIR controller_get_dir(const int16_t* x_pt){
    if(*x_pt < -THRESHOLD){
        return ACC_RIGHT;
    }
    if(*x_pt > THRESHOLD){
        return ACC_LEFT;
    }
    else{
        return ACC_MIDDLE;
    }
}


void controller_set_pin_high(ACC_DIR* acc_dir){
    switch(*acc_dir){
        case(ACC_LEFT):
            GPIO->OUTSET = (1 << PIN_0);
            GPIO->OUTCLR = (1 << PIN_1);
            break;

        case(ACC_RIGHT):
            GPIO->OUTSET = (1 << PIN_1);
            GPIO->OUTCLR = (1 << PIN_0);
            break;

        case(ACC_MIDDLE):
            GPIO->OUTCLR = (1 << PIN_1);
            GPIO->OUTCLR = (1 << PIN_0);
            break;

    }
}


void controller_send_dir(int16_t* x_pt){

    ACC_DIR acc_dir = controller_get_dir(x_pt);

    controller_set_pin_high(&acc_dir);
}


void controller_send_button_pressed(){
    if(button_b_pressed()){
        GPIO->OUTSET = (1 << PIN_2);
    }
    else{
        GPIO->OUTCLR = (1 << PIN_2);
    }
}
