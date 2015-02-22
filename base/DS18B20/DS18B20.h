/*
 * DS18B20.h
 *
 *  Created on: Nov 7, 2014
 *      Author: mepping
 */

#ifndef DS18B20_DS18B20_H_
#define DS18B20_DS18B20_H_

#include "StdDef.h"

/* ----------------- Defines ----------------- */

#define DS18B20_RESULOTION_9B	(0x1FU)
#define DS18B20_RESULOTION_10B	(0x3FU)
#define DS18B20_RESULOTION_11B	(0x5FU)
#define DS18B20_RESULOTION_12B	(0x7FU)

/* ----------------- Class ----------------- */

class DS18B20 {
public:
	DS18B20(const uint8_t romCode[]);
	DS18B20(void);

	StdReturnType readRomCode(void);
	StdReturnType writeScratchpad(uint8_t config, uint8_t tH, uint8_t tL);
	StdReturnType readScratchpad(uint8_t *content);
	StdReturnType copyScratchpad(void);
	StdReturnType readTemperature(uint16_t * temperature);
	StdReturnType startConversion(void);

	void getRomCode(uint8_t romCode[]);

private:
	/* Helper */
	void selectDevice(void);

	/* Bus Driver */
	StdReturnType reset(void);
	uint8_t readBit(void);
	uint8_t readByte(void);
	void writeBit(uint8_t value);
	void writeByte(uint8_t value);

	/* Variables */
	uint8_t RomCode[8];
	uint8_t SingleDrop;
};

#endif /* DS18B20_DS18B20_H_ */
