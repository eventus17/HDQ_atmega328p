/********************************
* Created by Vitaliy on 5/5/20.
********************************/

#ifndef _HDQ_H
#define _HDQ_H

#include <util/delay.h>

#define FALSE 0
#define TRUE 1

//#warning REDEFINE GPIO PORT and PIN, It is not necessary to solder 10K resistor to Vcc - GPIO MODE PULL UP ( del this warning)
#define HDQ_DIR                 DDRB
#define HDQ_PORT                PORTB
#define HDQ_PIN                 PINB
#define HDQ_PIN_NUMBER          PB2
#define HDQ_PULL_UP             PORTB

/*************************
 * Read/write command mask
**************************/
#define HDQ_ADDR_MASK_READ      0x00
#define HDQ_ADDR_MASK_WRITE     0x80

/**************************************************
 * This is the number of times the slave wait loop
 * will run before we time out.
 * As far as I can tell the loop uses ~6 instructions
 * thus giving about 200uS delay which is a full bit write
***************************************************/
#define HDQ_DELAY_FAIL_TRIES    300

/**************************
 * HDQ Default timings
**************************/
#define HDQ_DELAY_THW1			45	/* 32 - 66uS  */
#define HDQ_DELAY_THW0			120	/* 70 - 145uS */

#define HDQ_DELAY_TDW1			48	/* 0.5 - 50us */
#define HDQ_DELAY_TDW0			110	/* 86 - 145uS */
#define HDQ_DELAY_TCYCD			205	/* 190 min    */

#define HDQ_DELAY_TB            250 /* Min: 190uS */
#define HDQ_DELAY_TBR           70  /* Min: 40uS  */
#define HDQ_DELAY_TCYCH         250 /* Min: 190uS Max: 250uS */

#define HDQ_DELAY_TRSPS_MAX     320 /* Max: 320uS */
#define HDQ_DELAY_BIT_TOTAL     200

void HDQ_doBreak(void);
void HDQ_writeByte(uint8_t payload);
uint8_t HDQ_write(uint8_t reg, uint8_t payload);
uint8_t HDQ_write_Verif(uint8_t reg, uint8_t payload, uint8_t verif);
uint8_t HDQ_read(uint8_t reg);

#endif