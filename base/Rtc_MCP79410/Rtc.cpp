/*
 * Rtc.cpp
 *
 *  Created on: 03.04.2016
 *      Author: viseml
 */


#include "Rtc.h"
#include "Twi.h"

#define RTC_RTCADDR         (0x6FU)  /* 7bit address */
#define RTC_BAUDRATE        (100000) /* 100kHz */

#define RTC_OFFSET_SRAM     (0x20U)

#define RTC_REGISTER_SEC    (0x00U)
#define RTC_REGISTER_MIN    (0x01U)
#define RTC_REGISTER_HOUR   (0x02U)
#define RTC_REGISTER_DAY    (0x03U)
#define RTC_REGISTER_DATE   (0x04U)
#define RTC_REGISTER_MONTH  (0x05U)
#define RTC_REGISTER_YEAR   (0x06U)
#define RTC_REGISTER_CTRL   (0x07U)

#define RTC_MASK_MIN        (0x7FU)
#define RTC_MASK_HOUR       (0x3FU) /* 24h-format */
#define RTC_MASK_DATE       (0x3FU)
#define RTC_MASK_MONTH      (0x1FU)
#define RTC_MASK_YEAR       (0xFFU)

#define RTC_BIT_VBATEN      (0x08U)
#define RTC_BIT_ST          (0x80U)
#define RTC_BIT_SQWE        (0x40U)
#define RTC_BIT_DAYMON      (0x01U) /* Monday */

static uint8_t Rtc_Dec2Bcd(uint8_t num):
static uint8_t Rtc_Bcd2Dec(uint8_t num);
static StdReturnType Rtc_WriteByte(uint8_t device, uint8_t address, uint8_t data);
static StdReturnType Rtc_ReadByte(uint8_t device, uint8_t address, uint8_t *data);

void Rtc_Init(void)
{
  uint8_t data;

  Twi_Init(RTC_BAUDRATE);

  /* Check for VBATEN flag. If unset, initialize RTC with default date */
  if(Rtc_ReadByte(RTC_RTCADDR, RTC_REGISTER_DAY, &data) == E_OK)
  {
    if((data & RTC_BIT_VBATEN) != RTC_BIT_VBATEN)
    {
      /* Initialize RTC */
      (void) Rtc_SetTime(16, 01, 01, 00, 00);
    }
  }
}

StdReturnType Rtc_SetTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min)
{
  StdReturnType retVal = E_OK;

  min   = (Rtc_Dec2Bcd(min) & RTC_MASK_MIN);
  hour  = (Rtc_Dec2Bcd(hour) & RTC_MASK_HOUR);
  day   = (Rtc_Dec2Bcd(day) & RTC_MASK_DATE);
  month = (Rtc_Dec2Bcd(month) & RTC_MASK_MONTH);
  year  = (Rtc_Dec2Bcd(year) & RTC_MASK_YEAR);

  retVal |= Rtc_WriteByte(RTC_RTCADDR, RTC_REGISTER_SEC,   0);                                /* Stop RTC */
  retVal |= Rtc_WriteByte(RTC_RTCADDR, RTC_REGISTER_DAY,   RTC_BIT_VBATEN | RTC_BIT_DAYMON);  /* Enable VBatt */
  retVal |= Rtc_WriteByte(RTC_RTCADDR, RTC_REGISTER_CTRL,  RTC_BIT_SQWE);                     /* Activate 1Hz MFP output */

  retVal |= Rtc_WriteByte(RTC_RTCADDR, RTC_REGISTER_MIN,   min);
  retVal |= Rtc_WriteByte(RTC_RTCADDR, RTC_REGISTER_HOUR,  hour);
  retVal |= Rtc_WriteByte(RTC_RTCADDR, RTC_REGISTER_DATE,  day);
  retVal |= Rtc_WriteByte(RTC_RTCADDR, RTC_REGISTER_MONTH, month);
  retVal |= Rtc_WriteByte(RTC_RTCADDR, RTC_REGISTER_YEAR,  year);
  retVal |= Rtc_WriteByte(RTC_RTCADDR, RTC_REGISTER_SEC,   RTC_BIT_ST);    /* Start RTC */

  return retVal;
}

StdReturnType Rtc_GetTime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *min)
{
  StdReturnType retVal = E_OK;

  retVal |= Rtc_ReadByte(RTC_RTCADDR, RTC_REGISTER_MIN,   &min);
  retVal |= Rtc_ReadByte(RTC_RTCADDR, RTC_REGISTER_HOUR,  &hour);
  retVal |= Rtc_ReadByte(RTC_RTCADDR, RTC_REGISTER_DATE,  &day);
  retVal |= Rtc_ReadByte(RTC_RTCADDR, RTC_REGISTER_MONTH, &month);

  min   = (Rtc_Bcd2Dec(min   & RTC_MASK_MIN));
  hour  = (Rtc_Bcd2Dec(hour  & RTC_MASK_HOUR));
  day   = (Rtc_Bcd2Dec(day   & RTC_MASK_DATE));
  month = (Rtc_Bcd2Dec(month & RTC_MASK_MONTH));
  year  = (Rtc_Bcd2Dec(year  & RTC_MASK_YEAR));

  return retVal;
}

StdReturnType Rtc_WriteSRAM(uint8_t addr, uint8_t data)
{
  return Rtc_WriteByte(RTC_RTCADDR, addr + RTC_OFFSET_SRAM, data);
}

StdReturnType Rtc_GetSRAM(uint8_t addr, uint8_t *data)
{
  return Rtc_ReadByte(RTC_RTCADDR, addr + RTC_OFFSET_SRAM, data);
}

static StdReturnType Rtc_WriteByte(uint8_t device, uint8_t address, uint8_t data)
{
  StdReturnType retVal = E_OK;

  retVal |= Twi_Start(device, TWI_WRITE);
  retVal |= Twi_Write(address);
  retVal |= Twi_Write(data);
  retVal |= Twi_Stop();

  return E_OK;
}

static StdReturnType Rtc_ReadByte(uint8_t device, uint8_t address, uint8_t *data)
{
  StdReturnType retVal = E_OK;

  retVal |= Twi_Start(device, TWI_WRITE);
  retVal |= Twi_Write(address);
  retVal |= Twi_Start(device, TWI_READ);
  retVal |= Twi_ReadNack(data);
  retVal |= Twi_Stop();

  return retVal;
}

static uint8_t Rtc_Dec2Bcd(uint8_t num)
{
  uint8_t units = num % 10;
  uint8_t tens = num / 10;
  return (tens << 4) | units;
}

static uint8_t Rtc_Bcd2Dec(uint8_t num)
{
  uint8_t units = num % 10;
  uint8_t tens = num / 10;
  return (tens << 4) | units;
}
