/* 
 * File:   sensor.h
 * Author: schul
 *
 * Created on 25. Juni 2019, 09:47
 */

#ifndef SENSOR_H
#define	SENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif

int distance();
int rightWall();
int leftWall();
int middle();
#ifdef	__cplusplus
}
#endif

#endif	/* SENSOR_H */

