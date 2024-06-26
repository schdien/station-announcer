#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "JQ8900.h"

void JQ8900_commandData(uint8_t command, uint8_t data[], uint8_t dataLen)
{
	uint8_t buffer[4 + dataLen];
	buffer[0] = 0xaa;
	buffer[1] = command;
	buffer[2] = dataLen;
	buffer[3 + dataLen] = buffer[0] + buffer[1] + buffer[2];
	int i;
	for (i = 0;i < dataLen;i++)
	{
		buffer[3 + i] = data[i];
		buffer[3 + dataLen] += data[i];//��У��λ
	}
	HAL_UART_Transmit(&huart1, buffer, 4 + dataLen, 0xffff);
	//HAL_StatusTypeDef a = HAL_UART_Transmit_IT(&huart1,buffer,4+dataLen);

}

void JQ8900_inReport(uint8_t stationNow)
{
	uint8_t data[4]={0x30,0x30+stationNow,0x30,0x38};
	JQ8900_commandData(0x1b,data,4);
}

void JQ8900_outReport(uint8_t stationNext)
{
	uint8_t data[6]={0x30,0x39,0x30,0x30+stationNext,0x30,0x3a};
	JQ8900_commandData(0x1b,data,6);
}
