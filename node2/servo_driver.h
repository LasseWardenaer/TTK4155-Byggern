/**
 * @file 
 * @brief Module for servo functionality
 */


#ifndef NODE2_SERVO_H
#define NODE2_SERVO_H


/**
 * @brief Initializes servo by setting PWM-signal, 
 * and sets position of servo to zero
 */
void servo_init();


/**
 * @brief Sets position of servo by adjusting PWM duty-cycle
 * 
 * @param x Desired position of servo in interval [-100, 100]
 */
void servo_set_position(int x);


#endif