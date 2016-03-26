/*
 * EcuM.h
 *
 *  Created on: Mar 26, 2016
 */

#ifndef ECUM_H_
#define ECUM_H_

#include "BaseDef.h"
#include "EcuM_Cfg.h"

void EcuM_Init(void);
void EcuM_RequestUser(uint8_t user);
void EcuM_ReleaseUser(uint8_t user);
boolean EcuM_UserActive(void);

#endif /* ECUM_H_ */
