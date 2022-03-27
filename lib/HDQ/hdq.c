#include <avr/io.h>
#include "hdq.h"


void Config_GPIO_Output(void){
    HDQ_DIR |= (1<<HDQ_PIN_NUMBER);     // PB2 jako wyjście
}

void Config_GPIO_Input(void){
    HDQ_DIR &= ~(1<<HDQ_PIN_NUMBER);    // PB2 jako wejście
    HDQ_PULL_UP |= (1<<HDQ_PIN_NUMBER);	// pull-up na wejście PB2
}

void HDQ_doBreak(void) {
    Config_GPIO_Output();               // Set pin as output

    HDQ_PORT &= ~(1<<HDQ_PIN_NUMBER);   // Singal a break on the line
    _delay_us(HDQ_DELAY_TB);

    Config_GPIO_Input();                // Make sure we leave enough time for the slave to recover
    _delay_us(HDQ_DELAY_TBR);
}


void HDQ_writeByte(uint8_t payload) {
    Config_GPIO_Output();               // Set pin as output

    for (uint8_t i = 0; i < 8; i++) {
        // Start bit
        HDQ_PORT &= ~(1<<HDQ_PIN_NUMBER);   // Bring pin low
        _delay_us(HDQ_DELAY_THW1);

        // Toggle the pin for this bit
        if (payload >> i & 0x01) {    // LSB First
            HDQ_PORT |= (1<<HDQ_PIN_NUMBER);// High
        } else {
            HDQ_PORT &= ~(1<<HDQ_PIN_NUMBER);// Low
        }
        
        _delay_us(HDQ_DELAY_THW0 - HDQ_DELAY_THW1 + 5);     // Bit time

        // Stop bit
        HDQ_PORT |= (1<<HDQ_PIN_NUMBER);    // Bring the pin high
        _delay_us(HDQ_DELAY_TCYCH - HDQ_DELAY_THW0);

    }

// Make sure we leave enough time for the slave to recover
    Config_GPIO_Input();     // Release pin
    _delay_us(HDQ_DELAY_TCYCH - HDQ_DELAY_THW0);

    return;
}


uint8_t HDQ_write(uint8_t reg, uint8_t payload) {
// Singal a break
    HDQ_doBreak();

// Write the register to write
    HDQ_writeByte((reg |= HDQ_ADDR_MASK_WRITE));

// Wait for the slave to finish reading the register
    _delay_us((HDQ_DELAY_TRSPS_MAX - HDQ_DELAY_BIT_TOTAL) / 2);

// Write the payload
    HDQ_writeByte(payload);

// Wait for the slave to finish writing the payload
    _delay_us((HDQ_DELAY_TRSPS_MAX - HDQ_DELAY_BIT_TOTAL) / 2);

    Config_GPIO_Input();     // Release pin

    return TRUE;

}

uint8_t HDQ_read(uint8_t reg) {
    uint8_t result = 0;
    uint16_t maxTries = HDQ_DELAY_FAIL_TRIES; // ~128uS at 8Mhz with 8 instructions per loop

// Singal a break
    HDQ_doBreak();

// Write the register to read
    HDQ_writeByte((reg |= HDQ_ADDR_MASK_READ));

    Config_GPIO_Input();
    for (uint8_t ii = 0; ii < 8; ii++) {
        // Wait for the slave to toggle a low, or fail
        maxTries = HDQ_DELAY_FAIL_TRIES;
        while ((HDQ_PIN & (1<<HDQ_PIN_NUMBER)) != 0 && maxTries-- > 0)
            if (maxTries == 1) return 0xFF;

        // Wait until Tdsub and half or one bit has passed
        _delay_us(((HDQ_DELAY_TDW0 - HDQ_DELAY_TDW1) / 2) + HDQ_DELAY_TDW1);
        // Read the bit
        result |= (HDQ_PIN & (1<<HDQ_PIN_NUMBER))<<ii;

        // Wait until Tssub has passed
        _delay_us(HDQ_DELAY_TCYCD - HDQ_DELAY_TDW0);
    }

    _delay_us(HDQ_DELAY_TB);

    return result;

}



uint8_t HDQ_write_Verif(uint8_t reg, uint8_t payload, uint8_t verif) { // Write the payload
    HDQ_write(reg, payload);

// Verify the write
    if (payload == HDQ_read(reg)) return TRUE;

    return FALSE;

}