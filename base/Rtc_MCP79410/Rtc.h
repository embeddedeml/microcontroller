/*
 * Rtc.h
 *
 *  Created on: 03.04.2016
 *      Author: viseml
 */

#ifndef RTC_H_
#define RTC_H_


#include "BaseDef.h"

void Rtc_Init(void);
StdReturnType Rtc_SetTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min);
StdReturnType Rtc_GetTime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *min);
StdReturnType Rtc_WriteSRAM(uint8_t addr, uint8_t data);
StdReturnType Rtc_GetSRAM(uint8_t addr, uint8_t *data);

#endif /* RTC_H_ */
