#ifndef EEPROM_H
#define EEPROM_H

#define SLAVE_WRITE_E     0xA0
#define SLAVE_READ_E      0xA1

void write_external_eeprom(unsigned char address, unsigned char data);
unsigned char read_external_eeprom(unsigned char address);

#endif