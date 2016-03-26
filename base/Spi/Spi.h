/*
 * Spi.h
 *
 *  Created on: Mar 26, 2016
 */

#ifndef SPI_H_
#define SPI_H_

#include "BaseDef.h"

class Spi
{
public:
  Spi();
  virtual ~Spi();

  /* must be implemented in sub-class */
  virtual uint8_t transmitSync(uint8_t c) = 0;
};

#endif /* SPI_H_ */
