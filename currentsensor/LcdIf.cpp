/*
 * LcdIf.cpp
 *
 *  Created on: Mar 26, 2016
 */

#include "LcdIf.h"
#include "LCD_ILI9341.h"
#include "Touch.h"
#include "Dio.h"
#include <string.h>

/* Inputs */
#include "AdcIf.h"

#define LCDIF_TIMER_INIT  7

static void LcdIf_Update(void);
static void LcdIf_WriteText(const unsigned char *text, uint8_t len);
static void LcdIf_WriteNumber(uint16_t value);

static LCD_ILI9341 lcd(LCD_DC, LCD_RST, LCD_CS);
static uint8_t LcdIf_Timer;

static const unsigned char LcdIf_Text_Voltage[] PROGMEM =
{
  "Spannung: "
};

static const unsigned char LcdIf_Text_Charge[] PROGMEM =
{
  "Lade-Strom: "
};

static const unsigned char LcdIf_Text_Discharge[] PROGMEM =
{
  "Entlade-Strom: "
};

void LcdIf_Init(Spi *spi)
{
  /* initialize LCD */
  pinMode(LCD_LED, OUTPUT);

  lcd.setSpi(spi);
  lcd.begin();
  lcd.setTextColor(ILI9341_BLACK, ILI9341_LIGHTGREY);
  lcd.setTextSize(2);

  /* initialize touch screen driver */
  Touch_Init(spi);
  Touch_Read();

  LcdIf_Timer = 0x00U;
}

void LcdIf_MainFunction(void)
{
  if(Touch_DataAvailable() == 1)
  {
    digitalWrite(LCD_LED, HIGH);
    LcdIf_Timer = LCDIF_TIMER_INIT;
    Touch_Read();

    /*
     unsigned int  i,j;
     Touch_Read();
     i = Touch_GetX();
     j = Touch_GetY();
     lcd.drawLine(i, j, i, j, ILI9341_BLACK);
     */
  }

  if(LcdIf_Timer > 0x00U)
  {
    LcdIf_Timer--;
    LcdIf_Update();
  }
  else
  {
    digitalWrite(LCD_LED, LOW);
  }
}

static void LcdIf_Update(void)
{
  uint16_t value;

  /* Voltage */
  value = AdcIf_GetValue(0);
  lcd.setCursor(10, 10);
  LcdIf_WriteText(LcdIf_Text_Voltage, sizeof(LcdIf_Text_Voltage));
  LcdIf_WriteNumber(value);
  lcd.write('V');

  /* Charge Current */
  value = AdcIf_GetValue(1);
  lcd.setCursor(10, 50);
  LcdIf_WriteText(LcdIf_Text_Charge, sizeof(LcdIf_Text_Charge));
  LcdIf_WriteNumber(value);
  lcd.write('A');

  /* Charge Current */
  value = AdcIf_GetValue(2);
  lcd.setCursor(10, 100);
  LcdIf_WriteText(LcdIf_Text_Discharge, sizeof(LcdIf_Text_Discharge));
  LcdIf_WriteNumber(value);
  lcd.write('A');
}

static void LcdIf_WriteText(const unsigned char *text, uint8_t len)
{
  uint8_t i;
  unsigned char currentChar;

  for(i = 0; i < len; i++)
  {
    currentChar = pgm_read_byte(text + i);
    lcd.write(currentChar);
  }
}

static void LcdIf_WriteNumber(uint16_t value)
{
  char buffer[5 + 1];
  uint8_t i;
  uint8_t len;

  itoa(value, buffer, 10);

  /* fill leading empty space */
  len = 5 - strlen(buffer);
  while(len > 0)
  {
    lcd.write(' ');
    len--;
  }

  /* print number */
  i = 0;
  while(buffer[i] != 0)
  {
    lcd.write(buffer[i]);
    i++;
  }
}
