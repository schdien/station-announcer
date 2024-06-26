#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "audio.h"

static void WriteCommandData(uint8_t command, uint8_t data[]);
static void In(uint8_t stationNow);
static void Out(uint8_t stationNext);
static void Tips();
static void Stop();

struct audio Audio={
		.Transmit={WriteCommandData},
		.Setup={Stop},
		.Report={In,Out,Tips},
};


void WriteCommandData(uint8_t command, uint8_t data[])
{
	uint8_t dataLen=sizeof(data);
	uint8_t buffer[4 + dataLen];
	buffer[0] = 0xaa;
	buffer[1] = command;
	buffer[2] = dataLen;
	buffer[3 + dataLen] = buffer[0] + buffer[1] + buffer[2];
	int i;
	for (i = 0;i < dataLen;i++)
	{
		buffer[3 + i] = data[i];
		buffer[3 + dataLen] += data[i];
	}
	HAL_UART_Transmit(&huart1, buffer, 4 + dataLen, 0xffff);
	//HAL_StatusTypeDef a = HAL_UART_Transmit_IT(&huart1,buffer,4+dataLen);

}

void In(uint8_t stationNow)
{
	uint8_t data[4]={0x30,0x30+stationNow,0x30,0x38};
	Audio.Transmit.WriteCommandData(0x1b,data);
}

void Out(uint8_t stationNext)
{
	uint8_t data[6]={0x30,0x39,0x30,0x30+stationNext,0x30,0x3a};
	Audio.Transmit.WriteCommandData(0x1b,data);
}

void Tips()
{
	uint8_t data[2]={0x00,0x01};
	Audio.Transmit.WriteCommandData(0x07,data);
}

void Stop()
{
	Audio.Transmit.WriteCommandData(0x04,NULL);
}
