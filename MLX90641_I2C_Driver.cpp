/**
 * @copyright (C) 2017 Melexis N.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
 
 /* 
 !! This file has been drafted but the sensor hasn't arrived yet so 
 the code is yet to be tested !!
 */
#include "MLX90641_I2C_Driver.h"
#include "Arduino.h"
#include "Wire.h"

void MLX90641_I2CInit()
{   
    Wire.begin();
}

void MLX90641_I2CFreqSet(int freq)
{
	Wire.setClock(1000*freq);
}

int MLX90641_I2CRead(uint8_t slaveAddr, uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data)
{           									
	char cmd[2] = {0,0};
    uint16_t *p; 
    p = data;
	
	int total_bytes = 2*nMemAddressRead;
	const int div = 128;
	int factor = (int)(total_bytes)/div;
	int remainder = (total_bytes)%div;
	if(remainder!=0) factor++;
	
	for(int j = 0 ; j < factor ; j++){
		uint16_t address = startAddress+(j*div)/2;
		cmd[0] = address >> 8;
		cmd[1] = address & 0x00FF;

		Wire.endTransmission();
		delayMicroseconds(5);
		Wire.beginTransmission(slaveAddr);
		
		int written1 = Wire.write(cmd[0]);
		int written2 = Wire.write(cmd[1]);
		
		int end = Wire.endTransmission(0);
		if(end == 2 || end == 3) return -1;
		if(end == 1 || end == 4) return -2;
			
		int num_bytes = div;
		if(j == (factor-1) && remainder!=0) num_bytes = remainder;
			
		num_bytes = Wire.requestFrom((int)slaveAddr,num_bytes);		// update number of bytes that were read
		if(!num_bytes) return -1;
		
		for(int i = 0 ; i < num_bytes/2 ; i++){
			if(Wire.available()){		
				 uint16_t high = Wire.read()<<8;
				 uint16_t low = Wire.read();
				 *p++ = high + low;
			}
		}
	}
    return 0;   
} 

int MLX90641_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data)
{
	char cmd[4] = {0,0,0,0};
	static uint16_t dataCheck;

    cmd[0] = writeAddress >> 8;
    cmd[1] = writeAddress & 0x00FF;
    cmd[2] = data >> 8;
    cmd[3] = data & 0x00FF;
	
	Wire.endTransmission();
	Wire.beginTransmission(slaveAddr);
	
	delayMicroseconds(5);

	Wire.write(cmd,4);
	
	Wire.endTransmission(); 
    
    if(MLX90641_I2CRead(slaveAddr,writeAddress,1, &dataCheck) == -1) return -1;
    
    if ( dataCheck != data)
    {
        return -2;
    }    
    
    return 0;
}

