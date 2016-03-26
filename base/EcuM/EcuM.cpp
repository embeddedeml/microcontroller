/*
 * EcuM.cpp
 *
 *  Created on: Mar 26, 2016
 */

#include "EcuM.h"

#ifndef ECUM_CFG_NROFUSER
#error "ECUM_CFG_NROFUSER not defined"
#endif

#if ECUM_CFG_NROFUSER > 8
#error "Max. 8 user supported"
#endif

static uint8_t EcuM_UserRequests;

void EcuM_Init(void)
{
  EcuM_UserRequests = 0x00U;
}

void EcuM_RequestUser(uint8_t user)
{
  if(user < ECUM_CFG_NROFUSER)
  {
    EcuM_UserRequests |= (1 << user);
  }
}

void EcuM_ReleaseUser(uint8_t user)
{
  if(user < ECUM_CFG_NROFUSER)
  {
    EcuM_UserRequests &= ~(1 << user);
  }
}

boolean EcuM_UserActive(void)
{
  return (boolean) (EcuM_UserRequests > 0x00U);
}

