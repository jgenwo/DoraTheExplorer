/* 
 * File:   qei.h
 * Author: gyrireiersen
 *
 * Created on June 11, 2019, 1:22 PM
 */

#ifndef QEI_H
#define QEI_H
#include "xc.h"

extern long int longpos; // Declare long position count overflow variable
extern int current_speed; // Declare variable in which the current speed is stored
extern long int old_count;
extern long int new_count;

extern long int longpos2; // Declare long position count overflow variable
extern int current_speed2; // Declare variable in which the current speed is stored
extern long int old_count;
extern long int new_count;
extern long int dist; 

#ifdef __cplusplus

extern "C"
{
#endif


#ifdef __cplusplus
}
#endif
void init_QEI();
void calculate_speed(char motor);
void calculate_distance();
#endif /* QEI_H */
