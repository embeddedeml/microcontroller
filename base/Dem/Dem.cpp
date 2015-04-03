/*
 * Dem.cpp
 *
 *  Created on: Mar 3, 2015
 */

#include "Dem.h"
#include <avr/eeprom.h>

/* Defines */
#define DEM_MAX_OCCURRENCECOUNTER   (32U)

/* EEPROM variables */
Dem_EventStatusType Dem_EventStatus[DEM_CFG_NROFEVENTS] EEMEM;
uint8_t Dem_OccurrenceCounter[DEM_CFG_NROFEVENTS] EEMEM;

void Dem_Init(void)
{

}

StdReturnType Dem_ClearEventMemory(void)
{
    uint8_t i;

    for(i=0;i<DEM_CFG_NROFEVENTS;i++)
    {
        eeprom_update_byte(&(Dem_EventStatus[i]), 0x00U);
        eeprom_update_byte(&(Dem_OccurrenceCounter[i]), 0x00U);
    }

    return E_OK;
}

StdReturnType Dem_SetEventStatus(uint8_t eventId, Dem_EventStatusType eventStatus)
{
    StdReturnType retVal;
    uint8_t currentOccurrenceCounter;

    if(eventId >= DEM_CFG_NROFEVENTS)
    {
        retVal = E_NOT_OK;
    }
    else
    {
        currentOccurrenceCounter = eeprom_read_byte(&(Dem_OccurrenceCounter[eventId]));

        if(currentOccurrenceCounter < DEM_MAX_OCCURRENCECOUNTER)
        {
            if(eventStatus == DEM_EVENTSTATUS_FAILED)
            {
                currentOccurrenceCounter++;
            }

            eeprom_update_byte(&(Dem_EventStatus[eventId]), eventStatus);
            eeprom_update_byte(&(Dem_OccurrenceCounter[eventId]), currentOccurrenceCounter);
        }
        else
        {
            /* do not update byte to protect eeprom */
        }
    }

    return retVal;
}

StdReturnType Dem_GetEventStatus(uint8_t eventId, Dem_EventStatusType *eventStatus)
{
    StdReturnType retVal;

    if(eventId >= DEM_CFG_NROFEVENTS)
    {
        retVal = E_NOT_OK;
    }
    else
    {
        *eventStatus = eeprom_read_byte(&(Dem_EventStatus[eventId]));
        retVal = E_OK;
    }

    return retVal;
}

StdReturnType Dem_GetOccurrenceCounter(uint8_t eventId, uint8_t *counter)
{
    StdReturnType retVal;

    if(eventId >= DEM_CFG_NROFEVENTS)
    {
        retVal = E_NOT_OK;
    }
    else
    {
        *counter = eeprom_read_byte(&(Dem_OccurrenceCounter[eventId]));
        retVal = E_OK;
    }

    return retVal;
}
