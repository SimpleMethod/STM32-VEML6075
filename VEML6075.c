/*
 * VEML6075.c
 *
 *  Created on: 19.08.2021
 *      Author: SimpleMethod
 *
 *Copyright 2021 SimpleMethod
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy of
 *this software and associated documentation files (the "Software"), to deal in
 *the Software without restriction, including without limitation the rights to
 *use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *of the Software, and to permit persons to whom the Software is furnished to do
 *so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *THE SOFTWARE.
 ******************************************************************************
 */

#include <VEML6075.h>


uint8_t VELM6075_Init(VELM6075_StateHandle *VELM6075, I2C_HandleTypeDef * hi2c, uint8_t integrationTime)
{
	VELM6075->i2c_handle=hi2c;
	VELM6075->configReg|=0x00;
	VELM6075->configReg|=integrationTime;
	HAL_I2C_Mem_Write(VELM6075->i2c_handle, VEML6075_ADDRESS<<1, 0x00, 1, (uint8_t*)&VELM6075->configReg, 2, HAL_MAX_DELAY);
	if(VELM6075_GetDeviceID(VELM6075)==0x26)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	VELM6075_PowerOn(VELM6075);
}

uint16_t VELM6075_GetDeviceID(VELM6075_StateHandle *VELM6075)
{
	uint16_t data;
	HAL_I2C_Mem_Read(VELM6075->i2c_handle, VEML6075_ADDRESS<<1, 0x0C, 1, (uint8_t*)&data, 2, HAL_MAX_DELAY);
	return data;
}

void VELM6075_ReadData(VELM6075_StateHandle *VELM6075_State,VELM6075_DataHandle *VELM6075_Data)
{
	uint16_t config=0;
	HAL_I2C_Mem_Read(VELM6075_State->i2c_handle, VEML6075_ADDRESS<<1, 0x00, 1, (uint8_t*)&config, 2, HAL_MAX_DELAY);
	HAL_I2C_Mem_Read(VELM6075_State->i2c_handle, VEML6075_ADDRESS<<1, 0x07, 1, (uint8_t*)&VELM6075_Data->rawUVA, 2, HAL_MAX_DELAY);
	HAL_I2C_Mem_Read(VELM6075_State->i2c_handle, VEML6075_ADDRESS<<1, 0x09, 1, (uint8_t*)&VELM6075_Data->rawUVB, 2, HAL_MAX_DELAY);
	HAL_I2C_Mem_Read(VELM6075_State->i2c_handle, VEML6075_ADDRESS<<1, 0x0A, 1, (uint8_t*)&VELM6075_Data->rawUVCOMP1, 2, HAL_MAX_DELAY);
	HAL_I2C_Mem_Read(VELM6075_State->i2c_handle, VEML6075_ADDRESS<<1, 0x0B, 1, (uint8_t*)&VELM6075_Data->rawUVCOMP2, 2, HAL_MAX_DELAY);
	HAL_I2C_Mem_Read(VELM6075_State->i2c_handle, VEML6075_ADDRESS<<1, 0x08, 1, (uint8_t*)&VELM6075_Data->rawDarkCurrent, 2, HAL_MAX_DELAY);
}

void VELM6075_Shutdown(VELM6075_StateHandle *VELM6075)
{
	 uint16_t config=0;
	 HAL_I2C_Mem_Read(VELM6075->i2c_handle, VEML6075_ADDRESS<<1, 0x00, 1, (uint8_t*)&config, 2, HAL_MAX_DELAY);
	 config |= 0x01;
	 HAL_I2C_Mem_Write(VELM6075->i2c_handle, VEML6075_ADDRESS<<1, 0x00, 1, (uint8_t*)&config, 2, HAL_MAX_DELAY);
	 VELM6075->configReg=config;

}
void VELM6075_PowerOn(VELM6075_StateHandle *VELM6075)
{
	 uint16_t config=0;
	 HAL_I2C_Mem_Read(VELM6075->i2c_handle, VEML6075_ADDRESS<<1, 0x00, 1, (uint8_t*)&config, 2, HAL_MAX_DELAY);
	 config &= ~0x01;
	 HAL_I2C_Mem_Write(VELM6075->i2c_handle, VEML6075_ADDRESS<<1, 0x00, 1, (uint8_t*)&config, 2, HAL_MAX_DELAY);
	 VELM6075->configReg=config;
}


float VELM6075_GetUVA(VELM6075_DataHandle *VELM6075_Data)
{

	float raw_COMP1;
	float raw_COMP2;
	float UVValue;
	raw_COMP2=max(VELM6075_Data->rawUVCOMP2- VELM6075_Data->rawDarkCurrent, 0);
	raw_COMP1=max(VELM6075_Data->rawUVCOMP1- VELM6075_Data->rawDarkCurrent, 0);
	UVValue=max(VELM6075_Data->rawUVA - VELM6075_Data->rawDarkCurrent, 0);
	raw_COMP1=(raw_COMP1*2.22)/1.0;
	raw_COMP2=(raw_COMP2*1.33)/1.0;
	UVValue=max(UVValue-raw_COMP2,0);
	UVValue=max(UVValue-raw_COMP1,0);
	VELM6075_Data->UVA=UVValue;
	return UVValue;
}
float VELM6075_GetUVB(VELM6075_DataHandle *VELM6075_Data)
{
	float raw_COMP1;
	float raw_COMP2;
	float UVValue;
	raw_COMP2=max(VELM6075_Data->rawUVCOMP2- VELM6075_Data->rawDarkCurrent, 0);
	raw_COMP1=max(VELM6075_Data->rawUVCOMP1- VELM6075_Data->rawDarkCurrent, 0);
	UVValue=max(VELM6075_Data->rawUVB - VELM6075_Data->rawDarkCurrent, 0);
	raw_COMP1=(raw_COMP1*2.95)/1.0;
	raw_COMP2=(raw_COMP2*1.74)/1.0;
	UVValue=max(UVValue-raw_COMP2,0);
	UVValue=max(UVValue-raw_COMP1,0);
	VELM6075_Data->UVB=UVValue;
	return UVValue;
}
float VELM6075_GetUVIndex(VELM6075_DataHandle *VELM6075_Data)
{
	float UVIndex=((VELM6075_Data->UVA*0.001461)+(VELM6075_Data->UVB*0.002591))/2.0;
	VELM6075_Data->UVIndex=UVIndex;
	return UVIndex;
}


int32_t max(int32_t num1, int32_t num2)
{
    return (num1 > num2 ) ? num1 : num2;
}

