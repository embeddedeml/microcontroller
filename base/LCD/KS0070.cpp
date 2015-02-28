/*
 * KS0070.cpp
 *
 *  Created on: Nov 7, 2014
 */

#include "KS0070.h"

#include "Dio.h"
#include <util/delay.h>

/* Levels */
#define KS0070_RS_CMD				(LOW)
#define KS0070_RS_DATA				(HIGH)

/* Waiting times in ms */
#define KS0070_WAITUS_EN			(50)
#define KS0070_WAITMS_SOFTRESET		(20)
#define KS0070_WAITMS_CMD			(5)
#define KS0070_WAITMS_DATA			(5)

/* ----------------- Commands ----------------- */
#define KS0070_DATA_SOFTRESET		(0x03)
#define KS0070_DATA_4BITMODE		(0x02)

/* set function */
#define KS0070_CMD_SETFUNC			(0x20)

#define KS0070_SETFUNC_4BITMODE		(0x00)
#define KS0070_SETFUNC_1LINE		(0x00)
#define KS0070_SETFUNC_2LINE		(0x08)
#define KS0070_SETFUNC_5X10			(0x00)
#define KS0070_SETFUNC_5X7			(0x04)

/* display */
#define KS0070_CMD_DISPLAY			(0x08)

#define KS0070_DISPLAY_DISPLAYOFF	(0x00)
#define KS0070_DISPLAY_DISPLAYON	(0x04)
#define KS0070_DISPLAY_CURSOROFF	(0x00)
#define KS0070_DISPLAY_CURSORON		(0x02)
#define KS0070_DISPLAY_BLINKOFF		(0x00)
#define KS0070_DISPLAY_BLINKON		(0x01)

/* Entry mode */
#define KS0070_CMD_ENTRYMODE		(0x04)

#define KS0070_ENTRYMODE_CURSORINC	(0x02)
#define KS0070_ENTRYMODE_CURSORDEC	(0x00)
#define KS0070_ENTRYMODE_DISPSHIFT	(0x01)
#define KS0070_ENTRYMODE_DISPNSHIFT	(0x00)

/* other */
#define KS0070_CMD_CLEARDISPLAY		(0x01)
#define KS0070_CMD_RETURNHOME		(0x02)

KS0070::KS0070(void)
{
}

/* ----------------- Device Commands ----------------- */
/**
 * @brief Initializes display
 *
 * API needs several ms for execution.
 */
void KS0070::init(void)
{
    /* init pins */
    digitalWrite(PIN_KS0070_EN, LOW);

    pinMode(PIN_KS0070_RS, OUTPUT);
    pinMode(PIN_KS0070_EN, OUTPUT);
    pinMode(PIN_KS0070_DB4, OUTPUT);
    pinMode(PIN_KS0070_DB5, OUTPUT);
    pinMode(PIN_KS0070_DB6, OUTPUT);
    pinMode(PIN_KS0070_DB7, OUTPUT);

    /* reset sequence */
    this->dataOut(KS0070_DATA_SOFTRESET);
    _delay_ms(KS0070_WAITMS_SOFTRESET);
    this->dataOut(KS0070_DATA_SOFTRESET);
    _delay_ms(KS0070_WAITMS_SOFTRESET);
    this->dataOut(KS0070_DATA_SOFTRESET);
    _delay_ms(KS0070_WAITMS_SOFTRESET);

    /* 4bit mode */
    this->dataOut(KS0070_DATA_4BITMODE);
    _delay_ms(KS0070_WAITMS_CMD);

    /* init settings */
    this->sendInstruction( KS0070_CMD_DISPLAY |
    KS0070_DISPLAY_BLINKON |
    KS0070_DISPLAY_CURSORON |
    KS0070_DISPLAY_DISPLAYON);

    this->sendInstruction( KS0070_CMD_ENTRYMODE |
    KS0070_ENTRYMODE_CURSORINC |
    KS0070_ENTRYMODE_DISPNSHIFT);

    this->sendInstruction(KS0070_CMD_CLEARDISPLAY);
    this->sendInstruction(KS0070_CMD_RETURNHOME);
}

/**
 * @brief Writes byte to display
 *
 * @param byte  Value to write
 */
void KS0070::writeByte(uint8_t byte)
{
    this->sendData(byte);
}

/**
 * @brief Writes string to display
 *
 * @param bytes     String with terminating \0.
 */
void KS0070::writeString(char const *bytes)
{
    while(*bytes != '\0')
    {
        this->sendData(*bytes);
        bytes++;
    }
}

/**
 * @brief Writes decimal number to display
 *
 * @param number    Value to write
 */
void KS0070::writeNumber(uint16_t number)
{
    uint16_t divisor;
    uint8_t digit;
    uint8_t leadingZero;

    divisor = 10000U;
    leadingZero = 0U;
    while(divisor > 0)
    {
        digit = (number / divisor);
        if(digit > 0U)
        {
            leadingZero = 1U;
            this->writeByte(digit + '0');
        } else if(leadingZero == 1U)
        {
            this->writeByte(digit + '0');
        }

        number -= digit * divisor;
        divisor /= 10;
    }

    if(leadingZero == 0U)
    {
        this->writeByte('0');
    }
}

/**
 * @brief Returns cursor to beginning of line
 */
void KS0070::returnCursor(void)
{
    this->sendInstruction(KS0070_CMD_RETURNHOME);
}

/**
 * @brief Clears complete display
 */
void KS0070::clearDisplay(void)
{
    this->sendInstruction(KS0070_CMD_CLEARDISPLAY);
}

/* ----------------- Helper ----------------- */
void KS0070::sendInstruction(uint8_t db)
{
    digitalWrite(PIN_KS0070_RS, KS0070_RS_CMD);

    this->dataOut(db >> 4);
    this->dataOut(db);

    _delay_ms(KS0070_WAITMS_CMD);
}

void KS0070::sendData(uint8_t db)
{
    digitalWrite(PIN_KS0070_RS, KS0070_RS_DATA);

    this->dataOut(db >> 4);
    this->dataOut(db);

    _delay_ms(KS0070_WAITMS_DATA);
}

/* ----------------- Bus Driver ----------------- */

void KS0070::dataOut(uint8_t db)
{
    digitalWrite(PIN_KS0070_DB4, (db & 0x01U));
    digitalWrite(PIN_KS0070_DB5, (db & 0x02U) >> 1);
    digitalWrite(PIN_KS0070_DB6, (db & 0x04U) >> 2);
    digitalWrite(PIN_KS0070_DB7, (db & 0x08U) >> 3);

    digitalWrite(PIN_KS0070_EN, HIGH);
    _delay_us(KS0070_WAITUS_EN);
    digitalWrite(PIN_KS0070_EN, LOW);
    _delay_us(KS0070_WAITUS_EN);
}
