/*
 * ctrl_raw.h
 *
 *  Created on: Oct 14, 2015
 */

#ifndef PC_CTRL_RAW_H_
#define PC_CTRL_RAW_H_

#include <stdio.h>
#include <termios.h>
#include <string.h>

void Ctrl_Raw_ToggleLed();

void Ctrl_Raw_IncForwardBackward();
void Ctrl_Raw_DecForwardBackward();
void Ctrl_Raw_SetForwardBackward(unsigned char value);

void Ctrl_Raw_IncLeftRight();
void Ctrl_Raw_DecLeftRight();
void Ctrl_Raw_SetLeftRight(unsigned char value);
void Ctrl_Raw_UpdateLeftRight();

void Ctrl_Raw_IncSpeed();
void Ctrl_Raw_DecSpeed();
void Ctrl_Raw_SetSpeed(unsigned char value);

void Ctrl_Raw_IncThrottle();
void Ctrl_Raw_DecThrottle();

void Ctrl_Raw_SendFrame(FILE *fd);

#endif /* PC_CTRL_RAW_H_ */
