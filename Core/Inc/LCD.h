#ifndef __LCD_H__
#define __LCD_H__

typedef enum
{
	on,
	off,
	bits16,
	bits18
}status;

struct lcd{
	/*
	struct port {
		GPIO_TypeDef* RD;
		GPIO_TypeDef* WR;
		GPIO_TypeDef* CS;
		GPIO_TypeDef* RS;
		GPIO_TypeDef* RST;
		GPIO_TypeDef* D0;
		GPIO_TypeDef* D1;
		GPIO_TypeDef* D2;
		GPIO_TypeDef* D3;
		GPIO_TypeDef* D4;
		GPIO_TypeDef* D5;
		GPIO_TypeDef* D6;
		GPIO_TypeDef* D7;
	}Port;
	struct pin {
		uint16_t RD;
		uint16_t WR;
		uint16_t CS;
		uint16_t RS;
		uint16_t RST;
		uint16_t D0;
		uint16_t D1;
		uint16_t D2;
		uint16_t D3;
		uint16_t D4;
		uint16_t D5;
		uint16_t D6;
		uint16_t D7;
	}Pin;
	*/
	struct {  // unnamed structure
		void (*WriteBus) (uint8_t b);
		void (*WriteCommand) (uint8_t command);
		void (*WriteData) (uint8_t data);
	}Transmit;
	struct {
		void (*Init)();
		void (*Address)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		void (*IdleMode)(status s);
		void (*PixelFormat)(status s);
	}Setup;
	struct {
		void (*Rectangle)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint8_t* color);
		//void (*Dot)(uint16_t x, uint16_t y, uint8_t color);
		uint8_t (*CharXY08)(uint16_t x, uint16_t y, uint8_t const charColor[], const uint8_t backColor[], char index[]);
		void (*CharXY16)(uint16_t x, uint16_t y, const uint8_t charColor[], const uint8_t backColor[], char index[]);
		void (*CharXY32)(uint16_t x, uint16_t y, const uint8_t charColor[], const uint8_t backColor[], char index[]);
		void (*StrXY08)(uint16_t x, uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[]);
		void (*StrXY16)(uint16_t x, uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[]);
		void (*StrXY32)(uint16_t x, uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[]);
		void (*StrCenter16)(uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[]);
		void (*StrCenter32)(uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[]);
	}Disp;
}LCD;

#endif
