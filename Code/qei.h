/* 
 * File:   qei.h
 * Author: Gyri
 *
 * Created on June 11, 2019, 1:22 PM
 */

#ifndef QEI_H
#define QEI_H
#include "xc.h"

//Motor no. 1 is the LEFT motor
extern long int longpos1; // Declare long position count overflow variable
extern int current_speed1; // Declare variable in which the current speed is stored
extern long int old_count1;
extern long int new_count1;

//Motor no. 2 is the RIGHT motor
extern long int longpos2; // Declare long position count overflow variable
extern int current_speed2; // Declare variable in which the current speed is stored
extern long int old_count2;
extern long int new_count2; 

//Left motor
#define GET_ENCODER_1(POSITION_1) (POSITION_1=longpos1+POSCNT)
//Right motor
#define GET_ENCODER_2(POSITION_2) (POSITION_2=longpos2+POS2CNT)

#ifdef __cplusplus

extern "C"
{
#endif


#ifdef __cplusplus
}
#endif
void init_QEI();
void calculate_speed(char motor);
long int calculate_distance();
#endif /* QEI_H */
