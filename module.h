/*
 * module.h
 *
 *  Created on: Oct 17, 2023
 *      Author: curtis.lin
 */

#ifndef MODULE_H_
#define MODULE_H_

void forward(void);
void backward(void);
void brake(void);
void delay(int milliseconds);
void ini_(void);
void initClock();
void initSR04();
void initButton();
void triggerSR04();
void initTimerB();
void initTimerA();
void turnleft(void);
void turnright(void);
unsigned int measureDistance();
void moveServo(int i);
#endif /* MODULE_H_ */
