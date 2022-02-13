# I2C DMA

This Note focuses on ADC peripherals. How to initialize it and some notes for error countering

## Notes:

- You can configure I2C easily with DMA and use it easily, remember not to run multiple I2C function at the same time, it would force the temprorary data line to release, which can cause crack. 
=> Therefore you should wait for some miliseconds for the dataline to be stable and then start other task.

- **Config DMA (both directions) into Normal mode:** I2C can easily get error if you config DMA into **circular mode**, it would cause STM32 to continuously ask Slave to read, and cause data line conflict.

- [Some explanations about how HAL library works with I2C commands](https://stackoverflow.com/questions/38230248/how-do-i-use-the-stm32cubef4-hal-library-to-read-out-the-sensor-data-with-i2c?fbclid=IwAR2iq2CyQFAnMB6CFuYCcMLlap_2DaNZ8RYe8ZNhoJq8qPlUsn3GF-69CPg) 

## Example Codes:
[Final24C04Interface](./Final24C04Interface/) is my mini project which interface with EEPROM24C04 using I2C standards
