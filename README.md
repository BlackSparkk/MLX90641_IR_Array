# mlx90641-library adapted for Arduino Zero

This library takes the official Melexis library found here:

https://github.com/melexis/mlx90641-library

and adapts it specifically for the Arduino Zero / SAMD21. 

From datasheet:
The MLX90641 is a fully calibrated 16x12 pixels
thermal IR array in an industry standard 4-lead
TO39 package with digital interface.
The MLX90641 contains 192 FIR pixels. An
ambient sensor is integrated to measure the
ambient temperature of the chip and supply
sensor to measure the VDD. The outputs of all
sensors IR, Ta and VDD are stored in internal RAM
and are accessible through I2C.

This branch of the MLX90641 library is mostly filling the in the 
MLX90641_I2C_Driver.cpp file using the Arduino Wire library.

The I2CRead function has been written to work around the fact that the 
RingBuffer in the Wire library can only hold 256 bytes in its buffer and 
therefore the read function requests 128 bytes at a time.

Don't forget your pullup resistors (1K recommended)

Default I2C slave address of device is 0x33 but this may not be the case 
for your device.

 
NOTE:

1) It will not function on the Arduino Uno / ATmega328 as there isn't 
sufficient space on the stack to dump the MLX90641 EEPROM (823 words), 
which is how 
the Melexis library extracts all the required calibration data.

2) In theory this library can be used with the Arduino Mega / ATmega2560 
but I have not tested it



