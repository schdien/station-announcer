#include "gpio.h" 
#include "main.h" 
#include "LCD.h" 
#include "glyph_asset.h"

//Declare method. static methods, which can only be access in this file.
static void WriteBus(uint8_t b);
static void WriteCommand(uint8_t command);
static void WriteData(uint8_t data);
static void Init();
static void Address(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
static void IdleMode(status s);
static void PixelFormat(status s);
static void Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint8_t* color);
static uint8_t CharXY08(uint16_t x, uint16_t y, uint8_t const charColor[], const uint8_t backColor[], char index[]);
static void CharXY16(uint16_t x, uint16_t y, uint8_t const charColor[], const uint8_t backColor[], char index[]);
static void CharXY32(uint16_t x, uint16_t y, uint8_t const charColor[], const uint8_t backColor[], char index[]);
static void StrXY08(uint16_t x, uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[]);
static void StrXY16(uint16_t x, uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[]);
static void StrXY32(uint16_t x, uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[]);
static void StrCenter16(uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[]);
static void StrCenter32(uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[]);

//Define structure variable, which can be access by main.c or other source files. This variable is declared in LCD.h.
struct lcd LCD = {
	.Transmit = {WriteBus, WriteCommand, WriteData},
	.Setup = {Init, Address, IdleMode, PixelFormat},
	.Disp = {Rectangle, CharXY08, CharXY16, CharXY32, StrXY08, StrXY16, StrXY32, StrCenter16, StrCenter32},
};

//define methods

void WriteBus(uint8_t d)
{
	uint8_t b0 = 0b00000001 & d;
	uint8_t b1 = 0b00000010 & d;
	uint8_t b2 = 0b00000100 & d;
	uint8_t b3 = 0b00001000 & d;
	uint8_t b4 = 0b00010000 & d;
	uint8_t b5 = 0b00100000 & d;
	uint8_t b6 = 0b01000000 & d;
	uint8_t b7 = 0b10000000 & d;

	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET);

	if (b0) HAL_GPIO_WritePin(LCD_D0_GPIO_Port, LCD_D0_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_D0_GPIO_Port, LCD_D0_Pin, GPIO_PIN_RESET);
	if (b1) HAL_GPIO_WritePin(LCD_D1_GPIO_Port, LCD_D1_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_D1_GPIO_Port, LCD_D1_Pin, GPIO_PIN_RESET);
	if (b2) HAL_GPIO_WritePin(LCD_D2_GPIO_Port, LCD_D2_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_D2_GPIO_Port, LCD_D2_Pin, GPIO_PIN_RESET);
	if (b3) HAL_GPIO_WritePin(LCD_D3_GPIO_Port, LCD_D3_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_D3_GPIO_Port, LCD_D3_Pin, GPIO_PIN_RESET);
	if (b4) HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, GPIO_PIN_RESET);
	if (b5) HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, GPIO_PIN_RESET);
	if (b6) HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, GPIO_PIN_RESET);
	if (b7) HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET); //LCD read Bus when WR rising.
}

void WriteCommand(uint8_t command)
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
	LCD.Transmit.WriteBus(command);
}

void WriteData(uint8_t data)
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);
	LCD.Transmit.WriteBus(data);
}
void Init()
{
	HAL_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, GPIO_PIN_SET);//rising edge
	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET);//rising edge
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);//1 data 0 com
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//0 active
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);//0 active

	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(15);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(15);                   //到此为硬重置

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); //根据MCU 8080-I 8bit通信接口操作规范，设置引脚的对应状�?��??
	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); //片�?�有�??????????????


	LCD.Transmit.WriteCommand(0xCB);    //  指令Power Control A
	LCD.Transmit.WriteData(0x39);
	LCD.Transmit.WriteData(0x2C);
	LCD.Transmit.WriteData(0x00);
	LCD.Transmit.WriteData(0x34);   //设置 Vcore=1.6V
	LCD.Transmit.WriteData(0x02);   //设置DDVDH=5.6V

	LCD.Transmit.WriteCommand(0xCF);   // 指令Power Control B
	LCD.Transmit.WriteData(0x00);
	LCD.Transmit.WriteData(0XC1);
	LCD.Transmit.WriteData(0X30);

	LCD.Transmit.WriteCommand(0xE8);   //指令 Driver timing Congrol A
	LCD.Transmit.WriteData(0x85);
	LCD.Transmit.WriteData(0x00);
	LCD.Transmit.WriteData(0x78);

	LCD.Transmit.WriteCommand(0xEA);  //指令 Driver timing Congrol B
	LCD.Transmit.WriteData(0x00);
	LCD.Transmit.WriteData(0x00);

	LCD.Transmit.WriteCommand(0xED);  //指令 Power on sequence control
	LCD.Transmit.WriteData(0x64);
	LCD.Transmit.WriteData(0x03);
	LCD.Transmit.WriteData(0X12);
	LCD.Transmit.WriteData(0X81);

	LCD.Transmit.WriteCommand(0xF7);  //指令Pump ratio control
	LCD.Transmit.WriteData(0x20); //DDVDH=2*VCL

	LCD.Transmit.WriteCommand(0xC0);    //Power control
	LCD.Transmit.WriteData(0x23);   //VRH[5:0]  GVDD=4.6V

	LCD.Transmit.WriteCommand(0xC1);    //Power control
	LCD.Transmit.WriteData(0x10);   //SAP[2:0];BT[3:0]

	LCD.Transmit.WriteCommand(0xC5);    //VCM control  1
	LCD.Transmit.WriteData(0x3e);   //Contrast  VCOMH=3.45V VCOML=-1.5V
	LCD.Transmit.WriteData(0x28);

	LCD.Transmit.WriteCommand(0xC7);    //VCM control2
	LCD.Transmit.WriteData(0x86);   //--

	LCD.Transmit.WriteCommand(0x36);    // Memory Access Control
	LCD.Transmit.WriteData(0xc8);  //
	//LCD_writeData(0x08);  //

	LCD.Transmit.WriteCommand(0x3A);    //指令Pixel Format Set
	LCD.Transmit.WriteData(0x66);   //RGB 接口和MCU接口模式的像素数据格式为18bit/pixel

	LCD.Transmit.WriteCommand(0xB1);    //Frame Rate Control (B1h)（In Normal Mode /Full colors �??????????????
	LCD.Transmit.WriteData(0x00);
	LCD.Transmit.WriteData(0x18);  //79HZ(frame rate)

	LCD.Transmit.WriteCommand(0xB6);    // Display Function Control
	LCD.Transmit.WriteData(0x08);   // Interval Scan
	LCD.Transmit.WriteData(0x82);   //底背景为白屏�?????????????? 5 frams Scan Cycle 02black
	LCD.Transmit.WriteData(0x27);   //320 line



	LCD.Transmit.WriteCommand(0x11);    //Exit Sleep
	HAL_Delay(120);             //必须120ms的延�??????????????

	LCD.Transmit.WriteCommand(0x29);    //Display on
	//LCD.Setup.IdleMode(off);
}

void Address(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD.Transmit.WriteCommand(0x2a);
	LCD.Transmit.WriteData(x1 >> 8);   //设定屏幕数据操作区域的列首地�??????????????数据，先写�?16bit数据位的高位
	LCD.Transmit.WriteData(x1);      //写入16bit数据位的低位
	LCD.Transmit.WriteData(x2 >> 8);   //设定屏幕数据操作区域的列尾地�??????????????数据，先写�?16bit数据位的高位
	LCD.Transmit.WriteData(x2);      //写入16bit数据位的低位
	LCD.Transmit.WriteCommand(0x2b);
	LCD.Transmit.WriteData(y1 >> 8);     //设定屏幕数据操作区域的行首地�??????????????数据，，先写�??????????????16bit数据位的高位
	LCD.Transmit.WriteData(y1);         //写入16bit数据位的低位
	LCD.Transmit.WriteData(y2 >> 8);      //设定屏幕数据操作区域的行尾地�??????????????数据，，先写�??????????????16bit数据位的高位
	LCD.Transmit.WriteData(y2);         //写入16bit数据位的低位
}

void IdleMode(status s)
{
	if(s==on) LCD.Transmit.WriteCommand(0x39);
	if(s==off) LCD.Transmit.WriteCommand(0x38);
}

void PixelFormat(status s)
{
	LCD.Transmit.WriteCommand(0x3A);
	if(s==bits16) LCD.Transmit.WriteData(0x55);
	if(s==bits18) LCD.Transmit.WriteData(0x66);
}

void Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint8_t* color)
{
	LCD.Setup.Address(y1, x1, y2 - 1, x2 - 1);
	LCD.Transmit.WriteCommand(0x2c);
	int i, j;
	for (i = 0;i < (x2 - x1) * (y2 - y1);i++)
		for (j = 0;j < 3;j++) LCD.Transmit.WriteData(color[j]);
}

uint8_t CharXY08(uint16_t x,uint16_t y, const uint8_t charColor[], const uint8_t backColor[], char index[])
{
	uint8_t assetsNum08=sizeof(Char08)/sizeof(struct char08);
	uint8_t width=0;
	uint8_t n, i, j, k;
	for (n = 0;n < assetsNum08;n++)
	{
		if (Char08[n].Index == index[0])
		{
			LCD.Setup.Address(y, x, y + 7, x + 5);//range of x axis can larger than char's width
			LCD.Transmit.WriteCommand(0x2c);
			for (i = 0;i<3||Char08[n].Glyph[i]!='\0';i++,width++)//'\0'==0, i<3 for preventing Glyph[i<3]==0=='0'.
			{
				for (j = 0;j < 8;j++)
					for (k = 0;k < 3;k++)
						if (Char08[n].Glyph[i] & (1 << (7 - j)))
							LCD.Transmit.WriteData(charColor[k]);
						else LCD.Transmit.WriteData(backColor[k]);
			}
		}
	}
	return width;
}

void CharXY16(uint16_t x, uint16_t y, const uint8_t charColor[], const uint8_t backColor[], char index[])
{
	uint8_t assetsNum16 = sizeof(Char16) / sizeof(struct char16);
	uint8_t width=0;
	//uint8_t assetsNum1608 = sizeof(Char1608) / sizeof(struct char1608);
	uint8_t n, i, j, k;
	uint8_t cn = 0;
	for (n = 0;n < assetsNum16;n++)
	{
		if ((Char16[n].Index[0] == index[0]) && (Char16[n].Index[1] == index[1]) && (Char16[n].Index[2] == index[2]))
		{
			//cn = 1; //字符为中文
			LCD.Setup.Address(y, x, y + 15, x + 15);
			LCD.Transmit.WriteCommand(0x2c);
			for (i = 0;i < 32;i++,width++)
			{
				for (j = 0;j < 8;j++)
					for (k = 0;k < 3;k++)
						if (Char16[n].Glyph[i] & (1 << (7 - j)))
							LCD.Transmit.WriteData(charColor[k]);
						else LCD.Transmit.WriteData(backColor[k]);
			}
			width=width/2;
		}
	}
	/*
	if (!cn) //不是中文
	{
		for (n = 0;n < assetsNum1608;n++)
		{
			if (Char1608[n].Index == index[0])
			{
				LCD.Setup.Address(y, x, y + 15, x + 7);
				LCD.Transmit.WriteCommand(0x2c);
				for (i = 0;i < 16;i++)
				{
					for (j = 0;j < 8;j++)
						for (k = 0;k < 3;k++)
							if (Char1608[n].Glyph[i] & (1 << (7 - j)))
								LCD.Transmit.WriteData(charColor[k]);
							else LCD.Transmit.WriteData(backColor[k]);
				}
			}
		}
	}
	*/
}

void CharXY32(uint16_t x, uint16_t y, const uint8_t charColor[], const uint8_t backColor[], char index[])
{
	uint8_t assetsNum3232 = sizeof(Char3232) / sizeof(struct char3232);
	//uint8_t assetsNum3216=sizeof(num3216);
	uint8_t n, i, j, k;
	//uint8_t cn=0;
	for (n = 0;n < assetsNum3232;n++)
	{
		if ((Char3232[n].Index[0] == index[0]) && (Char3232[n].Index[1] == index[1]) && (Char3232[n].Index[2] == index[2]))
		{
			//cn=1; //字符为中文
			LCD.Setup.Address(y, x, y + 31, x + 31);
			LCD.Transmit.WriteCommand(0x2c);
			for (i = 0;i < 128;i++)
			{
				for (j = 0;j < 8;j++)
					for (k = 0;k < 3;k++)
						if (Char3232[n].Glyph[i] & (1 << (7 - j)))
							LCD.Transmit.WriteData(charColor[k]);
						else LCD.Transmit.WriteData(backColor[k]);
			}
		}
	}
	/*
	if(!cn) //字符不是中文
	{
		LCD_addressSet(y, x, y + 31, x + 15);
		LCD_writeCommand(0x2c);
		for(n=0;n<assetsNum1608;n++)
		{
			if(num1608[n]==index[0])
				for (i = 1;i <= 64;i++)
				{
					for (j = 0;j < 8;j++)
						for (k = 0;k < 3;k++)
							if (num1608[n+i]& (1 << (7 - j)))
								LCD_writeData(charColor[k]);
							else LCD_writeData(backColor[k]);
				}
		}
	}
	*/
}

void StrXY08(uint16_t x, uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[])
{
	uint8_t i;
	uint16_t xi = x;
	uint8_t width;
	char index[3];
	for (i = 0;str[i] != 0;)
	{
		index[0] = str[i];//数字占1字节
		width= LCD.Disp.CharXY08(xi, y, charColor, backColor, index);
		xi += spacing + width;
		i++;
	}

}

void StrXY16(uint16_t x, uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[])
{
	uint8_t i;
	uint16_t xi = x;
	char index[3]={0,0,0};
	for (i = 0;str[i] != 0;)
	{
		if (str[i] > ':')//中文
		{
			index[0] = str[i];
			index[1] = str[i + 1];
			index[2] = str[i + 2];//中文占3字节
			LCD.Disp.CharXY16(xi, y, charColor, backColor, index);
			xi += spacing + 16;
			i += 3;
		}
		else//数字和符号
		{
			index[0] = str[i];//数字占1字节
			LCD.Disp.CharXY16(xi, y, charColor, backColor, index);
			xi += spacing + 8;
			i++;
		}
	}
}

void StrXY32(uint16_t x, uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[])
{
	uint8_t i;
	uint16_t xi = x;
	char index[3];
	for (i = 0;str[i] != 0;)
	{
		if (str[i] > ':')//中文
		{
			index[0] = str[i];
			index[1] = str[i + 1];
			index[2] = str[i + 2];//中文占3字节
			LCD.Disp.CharXY32(xi, y, charColor, backColor, index);
			xi += spacing + 32;
			i += 3;
		}
		/*
		else//数字和符号
		{
			index[0]=str[i];//数字占1字节
			LCD_dispChar32(xi, yi, charColor, backColor, index);
			if(vertical) yi+=spacing+32;
			else xi+=spacing+16;
			i++;
		}
		*/
	}
}

void StrCenter16(uint16_t y, uint8_t spacing, const uint8_t charColor[], const uint8_t backColor[], char str[])
{
	uint8_t cnNum = 0;
	uint8_t numNum = 0;
	uint8_t x, i;
	for (i = 0;str[i] != 0;)
	{
		if (str[i] > ':') {
			cnNum++;
			i += 3;
		}
		else {
			numNum++;
			i++;
		}
	}
	x = 160 - (cnNum * 16 + numNum * 8 + spacing * (cnNum + numNum - 1)) / 2;
	LCD.Disp.StrXY16(x, y, spacing, charColor, backColor, str);
}

void StrCenter32(uint16_t y, uint8_t spacing, uint8_t const charColor[], const uint8_t backColor[], char str[])
{
	uint8_t cnNum = 0;
	uint8_t numNum = 0;
	uint8_t x, i;
	for (i = 0;str[i] != 0;)
	{
		if (str[i] > ':') {
			cnNum++;
			i += 3;
		}
		else {
			numNum++;
			i++;
		}
	}
	x = 160 - (cnNum * 32 + numNum * 16 + spacing * (cnNum + numNum - 1)) / 2;
	LCD.Disp.StrXY32(x, y, spacing, charColor, backColor, str);
}
