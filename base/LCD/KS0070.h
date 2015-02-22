/*
 * KS0070.h
 *
 *  Created on: Nov 7, 2014
 *      Author: mepping
 */

#ifndef KS0070_KS0070_H_
#define KS0070_KS0070_H_

#include "StdDef.h"

/* ----------------- Defines ----------------- */

/* ----------------- Class ----------------- */

class KS0070 {
public:
	KS0070(void);

	void init(void);
	void writeByte(uint8_t byte);
	void writeString(char const *byte);
	void writeNumber(uint16_t number);
	void returnCursor(void);
	void clearDisplay(void);

private:
	/* Helper */
	void sendData(uint8_t db);
	void sendInstruction(uint8_t db);

	/* Bus Driver */
	void dataOut(uint8_t db);

	/* Variables */
};

#endif /* KS0070_KS0070_H_ */
