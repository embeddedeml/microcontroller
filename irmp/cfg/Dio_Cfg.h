/*
 * Dio_Cfg.h
 *
 *  Created on: Nov 2, 2014
 *      Author: mepping
 */

#ifndef CFG_DIO_CFG_H_
#define CFG_DIO_CFG_H_

/*
 * High nibble: Port
 * 0: A (not available)
 * 1: B
 * 2: C
 * 3: D
 * 4: E
 * 5: F
 *
 * Low nibble: Pin 0-7
 *
 */

#define SS   (0x10) /* PB0 */
#define MOSI (0x12) /* PB2 */
#define MISO (0x13) /* PB3 */
#define SCK  (0x11) /* PB1 */

#define PIN_LED     (0x27) /* PC7 */
#define PIN_DS18B20 (0x46) /* PE6 */

#define PIN_KS0070_RS	(0x57) /* PF7 - A0 */
#define PIN_KS0070_EN	(0x56) /* PF6 - A1 */
#define PIN_KS0070_DB4	(0x55) /* PF5 - A2 */
#define PIN_KS0070_DB5	(0x54) /* PF4 - A3 */
#define PIN_KS0070_DB6	(0x51) /* PF1 - A4 */
#define PIN_KS0070_DB7	(0x50) /* PF0 - A5 */


#endif /* CFG_DIO_CFG_H_ */
