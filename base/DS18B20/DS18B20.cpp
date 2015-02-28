/*
 * DS18B20.cpp
 *
 *  Created on: Nov 7, 2014
 */

#include "DS18B20.h"

#include "Dio.h"
#include <util/delay.h>
#include <util/crc16.h>

/* ----------------- PIN Macros ----------------- */

#define DS18B20_BUS_REQUEST()  	pinMode(PIN_DS18B20, OUTPUT)
#define DS18B20_BUS_RELEASE()	pinMode(PIN_DS18B20, INPUT)
#define DS18B20_BUS_INIT()		digitalWrite(PIN_DS18B20, LOW)
#define DS18B20_BUS_READ()		digitalRead(PIN_DS18B20)

/* ----------------- Commands ----------------- */

/* ROM commands */
#define DS18B20_CMD_SEARCHROM	(0xF0U)
#define DS18B20_CMD_READROM		(0x33U)
#define DS18B20_CMD_MATCHROM	(0x55U)
#define DS18B20_CMD_SEARCHROM	(0xF0U)
#define DS18B20_CMD_SKIPROM		(0xCCU)
#define DS18B20_CMD_ALARMSEARCH	(0xECU)
/* FUNCTION commands */
#define DS18B20_CMD_CONVERT		(0x44U)
#define DS18B20_CMD_WRITESP		(0x4EU)
#define DS18B20_CMD_READSP		(0xBEU)
#define DS18B20_CMD_COPYSP		(0x48U)
#define DS18B20_CMD_RECALL		(0xB8U)
#define DS18B20_CMD_READPS		(0xB4U)

/**
 * @brief Class constructor for multi-slave bus
 *
 * @param romCode   64bit ROM code of slave
 */
DS18B20::DS18B20(c objectonst uint8_t romCode[])
{
    uint8_t i;

    this->SingleDrop = false;
    for(i=0;i<8;i++)
    {
        this->RomCode[i] = romCode[i];
    }
}

/**
 * @brief Class constructor for single-slave bus
 */
DS18B20::DS18B20(void)
{
    this->SingleDrop = true;
}

/**
 * @brief Returns ROM code of slave
 *
 * @param[out] romCode   64bit ROM code
 */
void DS18B20::getRomCode(uint8_t romCode[])
{
    uint8_t i;

    for(i = 0; i < 8; i++)
    {
        romCode[i] = this->RomCode[i];
    }
}

/* ----------------- Device Commands ----------------- */
/**
 * @brief Reads ROM code of slave
 *
 * API can be only used for single-slave bus.
 *
 * @return E_OK         ROM code successfully received
 * @return E_NOT_OK     Error occurred
 */
StdReturnType DS18B20::readRomCode(void)
{
    uint8_t i;
    uint8_t calculatedCrc;
    StdReturnType retVal = E_NOT_OK;

    if((this->SingleDrop == true) && (this->reset() == E_OK))
    {
        /* request ROM code */
        this->writeByte(DS18B20_CMD_READROM);

        /* read reponse and calculate CRC */
        calculatedCrc = 0x00U;
        for(i = 0; i < 7; i++)
        {
            this->RomCode[i] = this->readByte();
            calculatedCrc = _crc_ibutton_update(calculatedCrc, this->RomCode[i]);
        }

        /* compare CRC */
        this->RomCode[7] = this->readByte();
        if(calculatedCrc == this->RomCode[7])
        {
            retVal = E_OK;
        } else
        {
            retVal = E_NOT_OK;
        }
    }

    return retVal;
}

/**
 * @brief Starts conversion
 *
 * Application must wait long enough (100-900ms) after API call
 * before next command.
 *
 * @return  E_OK
 */
StdReturnType DS18B20::startConversion(void)
{
    /* init sequence */
    this->reset();
    this->selectDevice();

    /* start conversion */
    this->writeByte(DS18B20_CMD_CONVERT);

    /* no recovery time, application must wait long enough (100-900ms) */
    return E_OK;
}

/**
 * @brief Reads temperature from slave
 *
 * @param[out] temperature   Received temperature
 *
 * @return  E_OK        Temperature valid
 * @return  E_NOT_OK    Temperature invalid
 */
StdReturnType DS18B20::readTemperature(uint16_t *temperature)
{
    uint8_t tmpH, tmpL;
    uint8_t scratchpad[8];
    StdReturnType retVal;

    if(this->readScratchpad(scratchpad) == E_OK)
    {
        tmpL = scratchpad[0] >> 4;
        tmpH = (scratchpad[1] & 0x07);
        *temperature = (tmpH << 4) | tmpL;
        retVal = E_OK;
    } else
    {
        retVal = E_NOT_OK;
    }

    return retVal;
}

/**
 * @brief Writes new values to scratchpad
 *
 * @param config    New value for config register
 * @param tH        New value for temperature (high) register
 * @param tL        New value for temperature (low) register
 *
 * @return  E_OK        New values written successful
 * @return  E_NOT_OK    Error occurred
 */
StdReturnType DS18B20::writeScratchpad(uint8_t config, uint8_t tH, uint8_t tL)
{
    uint8_t scratchpad[8];
    StdReturnType retVal = E_OK;

    /* init sequence */
    this->reset();
    this->selectDevice();

    /* write scratchpad */
    this->writeByte(DS18B20_CMD_WRITESP);
    this->writeByte(tH);
    this->writeByte(tL);
    this->writeByte(config);

    /* read scratchpad */
    if(this->readScratchpad(scratchpad) == E_OK)
    {
        if(scratchpad[2] != tH)
        {
            retVal = E_NOT_OK;
        }
        if(scratchpad[3] != tL)
        {
            retVal = E_NOT_OK;
        }
        if(scratchpad[4] != config)
        {
            retVal = E_NOT_OK;
        }
    } else
    {
        retVal = E_NOT_OK;
    }

    return retVal;
}

/**
 * @brief Reads all registers of scratchpad
 *
 * @param[out] content  Content of scratchpad (8 bytes)
 *
 * @return  E_OK        Content valid
 * @return  E_NOT_OK    Content invalid
 */
StdReturnType DS18B20::readScratchpad(uint8_t *content)
{
    uint8_t i;
    uint8_t calculatedCrc, receivedCrc;
    StdReturnType retVal;

    /* init sequence */
    this->reset();
    this->selectDevice();

    /* read scratchpad */
    this->writeByte(DS18B20_CMD_READSP);

    calculatedCrc = 0x00U;
    for(i = 0; i < 8; i++)
    {
        content[i] = this->readByte();
        calculatedCrc = _crc_ibutton_update(calculatedCrc, content[i]);
    }

    /* compare CRC */
    receivedCrc = this->readByte();
    if(calculatedCrc == receivedCrc)
    {
        retVal = E_OK;
    } else
    {
        retVal = E_NOT_OK;
    }

    return retVal;
}

/**
 * @brief Requests slave to copy scratchpad to EEPROM
 *
 * Application must wait long enough (20-30ms) before next command.
 *
 * @return E_OK
 */
StdReturnType DS18B20::copyScratchpad(void)
{
    /* init sequence */
    this->reset();
    this->selectDevice();

    /* write scratchpad to EEPROM */
    this->writeByte(DS18B20_CMD_COPYSP);

    /* no recovery time, application must wait long enough (20-30ms)*/
    return E_OK;
}
/* ----------------- Helper ----------------- */

void DS18B20::selectDevice(void)
{
    uint8_t i;

    if(this->SingleDrop == true)
    {
        this->writeByte(DS18B20_CMD_SKIPROM);
    } else
    {
        this->writeByte(DS18B20_CMD_MATCHROM);
        for(i = 0; i < 8; i++)
        {
            this->writeByte(this->RomCode[i]);
        }
    }
}
/* ----------------- Bus Driver ----------------- */

StdReturnType DS18B20::reset(void)
{
    StdReturnType retVal;

    /* generate reset pulse */
    DS18B20_BUS_INIT();
    DS18B20_BUS_REQUEST();
    _delay_us(500);
    DS18B20_BUS_RELEASE();

    /* wait for presence pulse */
    _delay_us(70);

    /* read presence pulse */
    if(DS18B20_BUS_READ() == LOW)
    {
        /* presence detected */
        retVal = E_OK;
    } else
    {
        /* no presence detected */
        retVal = E_NOT_OK;
    }

    /* recovery */
    _delay_us(300);

    return retVal;
}

void DS18B20::writeByte(uint8_t value)
{
    uint8_t i;

    for(i = 0; i < 8; i++)
    {
        this->writeBit(value & 0x01);
        value = value >> 1;
    }
}

uint8_t DS18B20::readByte(void)
{
    uint8_t value, mask, i;

    value = 0x00U;
    mask = 0x01U;
    for(i = 0; i < 8; i++)
    {
        if(this->readBit() == 0x01)
        {
            value |= mask;
        }
        mask = mask << 1;
    }

    return value;
}

uint8_t DS18B20::readBit(void)
{
    uint8_t value;

    /* init */
    DS18B20_BUS_REQUEST();
    _delay_us(1);

    /* read bit */
    DS18B20_BUS_RELEASE();
    _delay_us(10);
    value = DS18B20_BUS_READ();

    /* recovery */
    _delay_us(70);

    return value;
}

void DS18B20::writeBit(uint8_t value)
{
    /* init */
    DS18B20_BUS_REQUEST();
    _delay_us(10);

    /* value */
    if(value == 0x00U)
    {
        _delay_us(60);
        DS18B20_BUS_RELEASE();
    } else
    {
        DS18B20_BUS_RELEASE();
        _delay_us(60);
    }

    /* recovery */
    _delay_us(5);
}
