#include <xc.h>
#include "EEPROM.h"
#include "i2c.h"

void write_external_eeprom(unsigned char address, unsigned char data)
{
    i2c_start();

    i2c_write(SLAVE_WRITE_E);
    i2c_write(address);
    i2c_write(data);

    i2c_stop();

    /* EEPROM write cycle delay */
    for (unsigned int i = 0; i < 3000; i++);
}

unsigned char read_external_eeprom(unsigned char address)
{
    unsigned char data;

    i2c_start();

    /* Send EEPROM write address */
    i2c_write(SLAVE_WRITE_E);

    /* Send memory address */
    i2c_write(address);

    /* Repeated START */
    i2c_rep_start();

    /* Send EEPROM read address */
    i2c_write(SLAVE_READ_E);

    /* Read one byte and send NACK */
    data = i2c_read(1);

    i2c_stop();

    return data;
}