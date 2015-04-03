/*
 * Dem.h
 *
 *  Created on: Mar 3, 2015
 */

#ifndef DEM_DEM_H_
#define DEM_DEM_H_

#include "BaseDef.h"
#include "Dem_Cfg.h"

typedef uint8_t Dem_EventStatusType;
#define DEM_EVENTSTATUS_PASSED  (0U)
#define DEM_EVENTSTATUS_FAILED  (1U)


extern void Dem_Init(void);
extern StdReturnType Dem_ClearEventMemory(void);
extern StdReturnType Dem_SetEventStatus(uint8_t eventId, Dem_EventStatusType eventStatus);
extern StdReturnType Dem_GetEventStatus(uint8_t eventId, Dem_EventStatusType *eventStatus);
extern StdReturnType Dem_GetOccurrenceCounter(uint8_t eventId, uint8_t *counter);

#endif /* DEM_DEM_H_ */
