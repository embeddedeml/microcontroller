/*
 * Spi.cpp
 *
 *  Created on: Mar 25, 2016
 */

#include "HwSpi.h"
#include "Dio.h"

HwSpi::HwSpi()
{
  pinMode(SPI_CLK,  OUTPUT);
  pinMode(SPI_MOSI, OUTPUT);
  pinMode(SPI_MISO, INPUT);

  /* set baudrate to fOsc/2 */
  SPSR = (1 << SPI2X);

  /* enable SPI in master mode */
  SPCR = (1 << SPE) | (1 << MSTR);
}

uint8_t HwSpi::transmitSync(uint8_t c)
{
  /* write data to transmission buffer */
  SPDR = c;

  /* wait for transmission */
  while(!(SPSR & (1<<SPIF)));

  /* return received data */
  return SPDR;
}

HwSpi::~HwSpi()
{

}
