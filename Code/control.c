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
#include "control.h"
#include "uart.h"
#include "stdio.h"

int kp = 1;
int ki = 1; // This integral might create initial integrated error overflow
int kd = 1;

int motor1_wanted_speed = 1;
int motor2_wanted_speed = 1;

char flag[] = "";

PID_Controller pos_control_left = {.kp = 1, .ki = 0, .kd = 0,
                                    .top_lim = 30, .bot_lim = -30};
PID_Controller pos_control_right = {.kp = 1, .ki = 0, .kd = 0,
                                    .top_lim = 30, .bot_lim = -30};
PID_Controller vel_control_left = {.kp =50, .ki = 0, .kd = 1,
                                    .top_lim = 2600, .bot_lim = -2600};
PID_Controller vel_control_right = {.kp = 50, .ki = 0, .kd = 1,
                                    .top_lim = 2600, .bot_lim = -2600};

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

void go_straight2(int speed){
    

    motor_set_speed('L',speed);
    motor_set_speed('R',speed);
    motor_control('L',current_speed1);
    motor_control('R',current_speed2);
}

void go_straight(int speed) {
    vel_control_left.target = (long)speed;
    vel_control_right.target = (long)speed;
    evaluate_controller(&vel_control_left, current_speed1);
    evaluate_controller(&vel_control_right, current_speed2);
    //sendNameValue("val", vel_control_left.value);
    drive_motor('L', vel_control_left.value);
    drive_motor('R', vel_control_right.value);
}

void stop(){
    fast_stop_motor('L');
    fast_stop_motor('R');
    POS1CNT = 0;
    POS2CNT = 0;
    longpos1 = 0;
    longpos2 = 0;
}

/*Here is another try to a more general and generic version of the PID that
 will probably work nicer in the controller cascade*/

void evaluate_controller(PID_Controller *controller, long int current_control_value) {
    
    (*controller).error = (*controller).target - current_control_value ;
    
    (*controller).integral = (*controller).integral + (*controller).error;
    
    (*controller).derivative = (*controller).error - (*controller).last_error;
    
    (*controller).last_error = (*controller).error;
    
    (*controller).value = (int)((*controller).kp * (*controller).error) + 
            ((*controller).ki * (*controller).integral) + 
            ((*controller).kd * (*controller).derivative);
    
    if ((*controller).value > (*controller).top_lim) {
        (*controller).value = (*controller).top_lim;
    } else if ((*controller).value < (*controller).bot_lim) {
        (*controller).value = (*controller).bot_lim;
    }
 
}

void initialize_controller(PID_Controller *controller, int kp, int ki, int kd,
                            int top_lim, int bot_lim, int target) {
    (*controller).kp = kp;
    (*controller).ki = ki;
    (*controller).kd = kd;
    
    (*controller).top_lim = top_lim;
    (*controller).bot_lim = bot_lim;
    (*controller).target = target;
    
    (*controller).error = 0;
    (*controller).integral = 0;
    (*controller).derivative = 0;
    (*controller).last_error = 0;
    (*controller).value = 0;
}

void turn_right(){
    
    pos_control_left.target = 932;
    pos_control_right.target = -932;
    
    evaluate_controller(&pos_control_left, (long)(longpos1 + POS1CNT));
    evaluate_controller(&pos_control_right, (long)(longpos2 + POS2CNT));
    
    //sendNameValue("pos2", POS2CNT);
    //sendNameValue("ctrl", pos_control_left.value);
    
    vel_control_left.target = pos_control_left.value;
    vel_control_right.target = pos_control_right.value;
    
    evaluate_controller(&vel_control_left, (long)current_speed1);
    evaluate_controller(&vel_control_right, (long)current_speed2);
    
    drive_motor('L', vel_control_left.value);
    drive_motor('R', vel_control_right.value);
}

void turn_left(){
    
    pos_control_left.target = -932;
    pos_control_right.target = 932;
    
    evaluate_controller(&pos_control_left, (long)(longpos1 + POS1CNT));
    evaluate_controller(&pos_control_right, (long)(longpos2 + POS2CNT));
    
    vel_control_left.target = pos_control_left.value;
    vel_control_right.target = pos_control_right.value;
    
    evaluate_controller(&vel_control_left, (long)current_speed1);
    evaluate_controller(&vel_control_right, (long)current_speed2);
    
    drive_motor('L', vel_control_left.value);
    drive_motor('R', vel_control_right.value);
}

void go_one_straight(){
    
    /*Given we take the big wheels, the number to set dist roughly calculates as:
    d*112 ; where d is the distance to be travelled in cm */
    
    int dist = 1120;
    
    pos_control_left.target = dist;
    pos_control_right.target = dist;
    
    evaluate_controller(&pos_control_left, (long)(longpos1 + POS1CNT));
    evaluate_controller(&pos_control_right, (long)(longpos2 + POS2CNT));
    
    vel_control_left.target = pos_control_left.value;
    vel_control_right.target = pos_control_right.value;
    
    evaluate_controller(&vel_control_left, (long)current_speed1);
    evaluate_controller(&vel_control_right, (long)current_speed2);
    
    drive_motor('L', vel_control_left.value);
    drive_motor('R', vel_control_right.value);
}



