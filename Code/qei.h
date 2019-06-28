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
#ifdef __cplusplus

extern "C"
{
#endif


#ifdef __cplusplus
}
#endif
void init_QEI();
void calculate_speed(void);
#endif /* QEI_H */
