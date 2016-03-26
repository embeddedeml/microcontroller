/*
 * LcdIf.h
 *
 *  Created on: Mar 26, 2016
 */

#ifndef LCDIF_H_
#define LCDIF_H_

#include "BaseDef.h"
#include "Spi.h"

void LcdIf_Init(Spi *spi);
void LcdIf_MainFunction(void);

#endif /* LCDIF_H_ */
