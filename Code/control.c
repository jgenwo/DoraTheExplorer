/*
 * File:   motor_control.c
 * Author: Steff
 *
 * Created on 19. Juni 2019, 16:39
 */


#include "xc.h"
#include "pwm.h"
#include "gpio.h"

int kp = 1;
int ki = 1;
int kd = 1;

float motor1_wanted_speed = 1;
float motor2_wanted_speed = 1;

// Function to set the target speed of motor 1
void motor1_set_speed(float wanted_speed)
{
    motor1_wanted_speed = wanted_speed;
}

// Function to set the target speed of motor 2
void motor2_set_speed(float wanted_speed)
{
    motor2_wanted_speed = wanted_speed;
}

// Function to be periodically called to actually control motor 1
void motor1_control(float current_angular_speed)
{
    
    // initialize needed local variables once
    static float error = 0;
    static float integral = 0;
    static float derivative = 0;
    static float last_error = 0;
    static int pwm = 0;
    
    // Calculate the current error
    error = motor1_wanted_speed - current_angular_speed;
    
    // Integrate the errors over time by summing them up every step
    integral = integral + error;
    
    // Take the derivative by substracting the last time step error from 
    // the current error
    derivative = error - last_error;
    
    // Calculate desired pwm dutycycle by adding up the different factors
    pwm = (int)((kp * error) + (ki * integral) + (kd * derivative));
    pwm = (int)pwm;
    
    
    // Catch too low or high pwm values
    if (pwm > 2000) {
        pwm = 2000;
    }
    else if (pwm < -2000) {
        pwm = -2000;
    }
    
    // If pwm is positive, run clockwise, otherwise counter clockwise
    if (pwm > 0) {
        drive_motor_1_forward(pwm);
    }
    else {
        drive_motor_1_backward(-pwm);
    }
}

// Function to be periodically called to actually control motor 2
void motor21_control(float current_angular_speed)
{
    
    // initialize needed local variables once
    static float error = 0;
    static float integral = 0;
    static float derivative = 0;
    static float last_error = 0;
    static int pwm = 0;
    
    // Calculate the current error
    error = motor2_wanted_speed - current_angular_speed;
    
    // Integrate the errors over time by summing them up every step
    integral = integral + error;
    
    // Take the derivative by substracting the last time step error from 
    // the current error
    derivative = error - last_error;
    
    // Calculate desired pwm dutycycle by adding up the different factors
    pwm = (int)((kp * error) + (ki * integral) + (kd * derivative));
    pwm = (int)pwm;
    
    
    // Catch too low or high pwm values
    if (pwm > 2000) {
        pwm = 2000;
    }
    else if (pwm < -2000) {
        pwm = -2000;
    }
    
    // If pwm is positive, run clockwise, otherwise counter clockwise
    if (pwm > 0) {
        drive_motor_2_forward(pwm);
    }
    else {
        drive_motor_2_backward(-pwm);
    }
}
