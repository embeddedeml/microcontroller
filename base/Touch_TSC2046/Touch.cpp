/*
 * Touch.cpp
 *
 *  Created on: Mar 25, 2016
 */

#include "Touch.h"
#include "Dio.h"

#define PREC_TOUCH_CONST 20

#define PixSizeX  -14.58
#define PixSizeY  -10.625
#define PixOffsX  3800
#define PixOffsY  3800

static void Touch_WriteData(uint8_t c);
static uint16_t Touch_ReadData(void);

static uint16_t Touch_X;
static uint16_t Touch_Y;
static Spi *Touch_Spi;

void Touch_Init(Spi *spi)
{
  Touch_Spi = spi;

  pinMode(TOUCH_CS, OUTPUT);
  pinMode(TOUCH_IRQ, INPUT);
  digitalWrite(TOUCH_CS, HIGH);
}

static void Touch_WriteData(uint8_t c)
{
  Touch_Spi->transmitSync(c);
}

static uint16_t Touch_ReadData(void)
{
  uint16_t data;
  uint16_t mdata;
  uint16_t ldata;

  mdata = Touch_Spi->transmitSync(0);
  ldata = Touch_Spi->transmitSync(0);
  data = (ldata >> 3) | (mdata << 5);

  return (data);
}

void Touch_Read(void)
{
  uint32_t tx = 0;
  uint32_t ty = 0;

  digitalWrite(TOUCH_CS, LOW);

  for(uint8_t i = 0; i < PREC_TOUCH_CONST; i++)
  {
    Touch_WriteData(0x90);
    ty += Touch_ReadData();

    Touch_WriteData(0xD0);
    tx += Touch_ReadData();
  }

  digitalWrite(TOUCH_CS, HIGH);

  Touch_X = tx / PREC_TOUCH_CONST;
  Touch_Y = ty / PREC_TOUCH_CONST;
}

boolean Touch_DataAvailable()
{
  return (boolean) (!digitalRead(TOUCH_IRQ));
}

uint16_t Touch_GetX(void)
{
  int16_t value;

  value = ((Touch_X - PixOffsX) / PixSizeX);

  if(value < 0)
  {
    value = 0;
  }

  return ((uint16_t) value);
}

uint16_t Touch_GetY(void)
{
  int16_t value;
  value = ((Touch_Y - PixOffsY) / PixSizeY);
  if(value < 0)
  {
    value = 0;
  }

  return ((uint16_t) value);
}
