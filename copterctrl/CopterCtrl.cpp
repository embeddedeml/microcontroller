/*
 * CopterCtrl.c
 *
 *  Created on: Oct 17, 2015
 */

#include "Pwm.h"
#include "Dio.h"
#include "Timer.h"

#include "CopterCtrl.h"

uint16_t CopterCtrl_LL_Buffer_BitState[250];
uint16_t CopterCtrl_LL_Buffer_BitLen[250];
uint8_t  CopterCtrl_LL_Buffer_LastElement;
uint8_t  CopterCtrl_LL_Buffer_Pos;

void CopterCtrl_LL_EnqueueBit(uint16_t bitlen, uint8_t state)
{
    CopterCtrl_LL_Buffer_BitState[CopterCtrl_LL_Buffer_LastElement] = state;
    CopterCtrl_LL_Buffer_BitLen[CopterCtrl_LL_Buffer_LastElement]   = bitlen;
    CopterCtrl_LL_Buffer_LastElement++;
}

void CopterCtrl_LL_Transmit(void)
{
    if(CopterCtrl_LL_Buffer_BitState[CopterCtrl_LL_Buffer_Pos] == 0)
    {
        Pwm_Channel1_Stop();
    }
    else
    {
        Pwm_Channel1_Start(27);
    }

    Timer_Timer2_Start(CopterCtrl_LL_Buffer_BitLen[CopterCtrl_LL_Buffer_Pos]);
}

void CopterCtrl_LL_Reset(void)
{
    pinMode(PIN_PWM, OUTPUT);
    Pwm_Channel1_Stop();
    CopterCtrl_LL_Buffer_Pos  = 0;
    CopterCtrl_LL_Buffer_LastElement = 0;
}

void CopterCtrl_HL_Header()
{
    CopterCtrl_LL_EnqueueBit(130,  1);
    CopterCtrl_LL_EnqueueBit(2300, 0);
    CopterCtrl_LL_EnqueueBit(2300, 1);
}

void CopterCtrl_HL_SendBit(uint8_t bit)
{
    if(bit == 0)
    {
        CopterCtrl_LL_EnqueueBit(300, 0);
        CopterCtrl_LL_EnqueueBit(800, 1);
    }
    else
    {
        CopterCtrl_LL_EnqueueBit(800, 0);
        CopterCtrl_LL_EnqueueBit(300, 1);
    }
}

void CopterCtrl_HL_SendByte(uint8_t byte)
{
    uint8_t mask = 0x80;
    while(mask > 0)
    {
        if((byte & mask) > 0)
        {
            CopterCtrl_HL_SendBit(1);
        }
        else
        {
            CopterCtrl_HL_SendBit(0);
        }
        mask = mask >> 1;
    }
}

void CopterCtrl_SendFrame(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3)
{
    CopterCtrl_LL_Reset();
    CopterCtrl_HL_Header();
    CopterCtrl_HL_SendByte(byte0);
    CopterCtrl_HL_SendByte(byte1);
    CopterCtrl_HL_SendByte(byte2);
    CopterCtrl_HL_SendByte(byte3);
    CopterCtrl_LL_Transmit();
}

void CopterCtrl_TimerCallback(void)
{
    CopterCtrl_LL_Buffer_Pos++;
    if(CopterCtrl_LL_Buffer_Pos < CopterCtrl_LL_Buffer_LastElement)
    {
        CopterCtrl_LL_Transmit();
    }
    else
    {
        CopterCtrl_LL_Reset();
    }
}

