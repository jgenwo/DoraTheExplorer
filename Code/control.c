/*
 * File:   motor_control.c
 * Author: Steff
 *
 * Created on 19. Juni 2019, 16:39
 * Changed by Adrian on 5-7-2019
 */


#include "xc.h"
#include "pwm.h"
#include "gpio.h"
#include "qei.h"

int kp = 1;
int ki = 1; // This integral might create initial integrated error overflow
int kd = 1;

int motor1_wanted_speed = 1;
int motor2_wanted_speed = 1;

// Function to set the target speed of motor 
void motor_set_speed(char motor, int wanted_speed)
{
    if (motor == 'L'){
        motor1_wanted_speed = wanted_speed;
    }
    else if (motor == 'R'){
        motor2_wanted_speed = wanted_speed;
    }
}

// Function to be periodically called to actually control motor
void motor_control(char motor, int current_angular_speed)
{
    // initialize needed local variables once
    static int error = 0;
    static int integral = 0;
    static int derivative = 0;
    static int last_error = 0;
    static int pwm = 0;
    
    // Calculate the current error
    if (motor == 'L'){
        error = motor1_wanted_speed - current_angular_speed;
    }
    else if (motor == 'R'){
        error = motor2_wanted_speed - current_angular_speed;
    }
    
    // Integrate the errors over time by summing them up every step
    integral = integral + error;
    
    if (integral > 2000){
        integral = 2000;
    } else if (integral < -2000){
        integral = -2000;
    }
    
    // Take the derivative by substracting the last time step error from 
    // the current error
    derivative = error - last_error;
    
    // Calculate desired pwm dutycycle by adding up the different factors
    pwm = (kp * error) + (ki * integral) + (kd * derivative);    
    
    // Catch too low or high pwm values
    if (pwm > 2000) {
        pwm = 2000;
    }
    else if (pwm < -2000) {
        pwm = -2000;
    }
    
    // If pwm is positive, run clockwise, otherwise counter clockwise
    if (pwm > 0) {
        drive_motor_forward(motor, pwm);
    }
    else {
        drive_motor_backward(motor, -pwm);
    }
}

void go_straight(int speed){
    calculate_speed('L'); // Call function from qei.c to calculate current speed
    calculate_speed('R'); // Call function from qei.c to calculate current speed2

    motor_set_speed('L',speed); 
    motor_set_speed('R',speed);
    motor_control('L',current_speed);
    motor_control('R',current_speed2);
}

void stop(){
    fast_stop_motor('L');
    fast_stop_motor('R');
    POS1CNT = 0;
    POS2CNT = 0;
}

