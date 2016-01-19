/*
 * CopterCtrl.h
 *
 *  Created on: Oct 17, 2015
 */

#ifndef COPTERCTRL_H_
#define COPTERCTRL_H_

#include "BaseDef.h"

void CopterCtrl_SendFrame(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3);
void CopterCtrl_TimerCallback(void);

#endif /* COPTERCTRL_H_ */
