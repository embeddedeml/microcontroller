/*
 * Dio_Cfg.h
 *
 *  Created on: Nov 2, 2014
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

#define PIN_LED     (0x27) /* PC7 */
#define PIN_LCD     (0x32) /* PD2 D0 */
#define PIN_OW      (0x57) /* PF7 A0 */
#define PIN_SD      (0x33) /* PD3 D1 */

#define PIN_KS0070_RS	(0x16) /* PB6 - IO10 */
#define PIN_KS0070_RW	(0x15) /* PB5 - IO9 */
#define PIN_KS0070_EN	(0x14) /* PB4 - IO8 */
#define PIN_KS0070_DB4	(0x46) /* PE6 - D7 */
#define PIN_KS0070_DB5	(0x37) /* PD7 - D6 */
#define PIN_KS0070_DB6	(0x26) /* PC6 - D5 */
#define PIN_KS0070_DB7	(0x34) /* PD4 - D4 */

#define PIN_DO  (0x13) /* PB3 - DO/MISO */
#define PIN_CK  (0x11) /* PB1 - CLK/SCK */
#define PIN_DI  (0x12) /* PB2 - DI/MOSI */
#define PIN_CS  (0x10) /* PB0 - CS/SS   */

#define PIN_PWM  (0x15) /* PB5 - PWM   */

#if 0
#define PIN_DS18B20 (0x46) /* PE6 */
#endif

#endif /* CFG_DIO_CFG_H_ */
