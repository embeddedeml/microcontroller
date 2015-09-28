/*
 * SDLogger.h
 *
 *  Created on: Apr 7, 2015
 */

#ifndef SD_SDLOGGER_H_
#define SD_SDLOGGER_H_

#include "BaseDef.h"
#include "ff.h"

#define SDLOGGER_MAX_PATHLEN    (16)
#define SDLOGGER_BUFFERSIZE     (256)

class SDLogger
{
public:
    SDLogger(const char* path);

    StdReturnType appendNumber(uint16_t number);
    StdReturnType appendChar(char character);
    StdReturnType flush(uint16_t minReserve);
    StdReturnType force(void);

private:
    char Path[SDLOGGER_MAX_PATHLEN];
    char Buffer[SDLOGGER_BUFFERSIZE];
    uint16_t LoggingPos;

    FATFS FatFs;        /* FatFs work area needed for each volume */
    FIL Fil;            /* File object needed for each open file */
};

#endif /* SD_SDLOGGER_H_ */
