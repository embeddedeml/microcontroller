/*
 * Touch.h
 *
 *  Created on: Mar 25, 2016
 */

#ifndef TOUCH_H_
#define TOUCH_H_

#include "BaseDef.h"
#include "Spi.h"

void Touch_Init(Spi *spi);
void Touch_Read(void);
boolean Touch_DataAvailable();
uint16_t Touch_GetX(void);
uint16_t Touch_GetY(void);

#endif /* TOUCH_H_ */
