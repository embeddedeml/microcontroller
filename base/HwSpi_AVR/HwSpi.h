/*
 * Spi.h
 *
 *  Created on: Mar 25, 2016
 */

#ifndef HWSPI_H_
#define HWSPI_H_

#include "BaseDef.h"
#include "Spi.h"

class HwSpi : public Spi
{
public:
  HwSpi();
  ~HwSpi();

  uint8_t transmitSync(uint8_t c);
};

#endif /* HWSPI_H_ */
