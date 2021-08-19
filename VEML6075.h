/*
 * VEML6075.h
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

#ifndef INC_VEML6075_H_
#define INC_VEML6075_H_

#include "main.h"
#define VEML6075_ADDRESS		0x10

/*The datasheet has calculations only for 100ms, so this value is recommended*/
enum VEML6075_IntegrationTime{VEML6075_UV_IT_50=0x00, VEML6075_UV_IT_100=0x10, VEML6075_UV_IT_200=0x20, VEML6075_UV_IT_400=0x30, VEML6075_UV_IT_800=0x40};

typedef struct {
	I2C_HandleTypeDef* i2c_handle;
	uint16_t configReg;
} VELM6075_StateHandle;

typedef struct {
	uint16_t rawUVA;
	uint16_t rawUVB;
	uint16_t rawUVCOMP1;
	uint16_t rawUVCOMP2;
	uint16_t rawDarkCurrent;
	float UVIndex;
	float UVA;
	float UVB;
} VELM6075_DataHandle;

VELM6075_StateHandle VELM6075_Handle;
VELM6075_DataHandle VELM6075_Data;

uint8_t VELM6075_Init(VELM6075_StateHandle *VELM6075, I2C_HandleTypeDef * hi2c, uint8_t integrationTime);
uint16_t VELM6075_GetDeviceID(VELM6075_StateHandle *VELM6075);
void VELM6075_ReadData(VELM6075_StateHandle *VELM6075_State,VELM6075_DataHandle *VELM6075_Data);
void VELM6075_PowerOn(VELM6075_StateHandle *VELM6075);
void VELM6075_Shutdown(VELM6075_StateHandle *VELM6075);
float VELM6075_GetUVIndex(VELM6075_DataHandle *VELM6075_Data);
float VELM6075_GetUVB(VELM6075_DataHandle *VELM6075_Data);
float VELM6075_GetUVA(VELM6075_DataHandle *VELM6075_Data);
int32_t max(int32_t num1, int32_t num2);


#endif /* INC_VEML6075_H_ */
