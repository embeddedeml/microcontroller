/*
 * SDLogger.cpp
 *
 *  Created on: Apr 7, 2015
 */

#include "SDLogger.h"
#include <string.h>
#include <stdlib.h>

SDLogger::SDLogger(const char* path)
{
    strncpy(this->Path, path, SDLOGGER_MAX_PATHLEN);
    this->Path[SDLOGGER_MAX_PATHLEN-1] = 0; /* set explicit terminating null, if path is too long */

    this->LoggingPos = 0x00U;
}

StdReturnType SDLogger::appendNumber(uint16_t number)
{
    StdReturnType retVal = E_NOT_OK;
    uint8_t i = 0;
    char output[6];

    itoa(number, output, 10);
    while(output[i] != 0)
    {
        retVal = this->appendChar(output[i]);
        if(retVal != E_OK)
        {
            break;
        }
        i++;
    }

    return retVal;
}

StdReturnType SDLogger::appendChar(char character)
{
    this->Buffer[this->LoggingPos] = character;
    this->LoggingPos++;
    return this->flush(0);
}

StdReturnType SDLogger::force(void)
{
    return this->flush(SDLOGGER_BUFFERSIZE);
}

StdReturnType SDLogger::flush(uint16_t minReserve)
{
    UINT bw;
    StdReturnType retVal = E_NOT_OK;

    if(this->LoggingPos >= (uint16_t)(SDLOGGER_BUFFERSIZE - minReserve))
    {
        f_mount(&this->FatFs, "", 0);
        if (f_open(&this->Fil, this->Path, FA_WRITE | FA_OPEN_ALWAYS) == FR_OK)
        {
            if(f_lseek(&this->Fil, f_size(&this->Fil)) == FR_OK)
            {
                f_write(&this->Fil, (const void*) this->Buffer, this->LoggingPos, &bw);
                f_close(&this->Fil);
                if (bw == this->LoggingPos)
                {
                    retVal = E_OK;
                }
            }
        }
        this->LoggingPos = 0x00U;
    }

    return retVal;
}
