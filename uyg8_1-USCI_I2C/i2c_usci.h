#ifndef _I2C_USCI_H
#define _I2C_USCI_H

void setup_pins(void);
void i2c_usci_init(const char);
void i2c_usci_start_tx(void); 
void i2c_usci_start_rx(void); 
void i2c_usci_stop(void); 
void i2c_usci_write(const char* data,const char address,const int length); 
unsigned char* i2c_usci_receive(const char adress,const int length);
unsigned char  i2c_usci_notready(void);

#endif /* _I2C_USCI_H */