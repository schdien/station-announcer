/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "audio.h"
#include "LCD.h"
#include "JQ8900.h"
#include "system_asset.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int flagMode=0;
int flagIn=0;
int flagOut=0;
int flagTips=0;
int flagRst=1;

typedef enum{
	  normal,
	  setting
}Mode;
Mode mode=normal;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  uint8_t routeNow=0;
  uint8_t routeNum=sizeof(Route)/sizeof(struct route);

  uint8_t upNow=1;//bus direction: 0 down 1up
  uint8_t upNext=1;//bus direction: 0 down 1up
  uint8_t stationNow=0;
  uint8_t stationNext=1;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  LCD.Setup.Init();
  LCD.Disp.Rectangle(0,0,320,240,Color.Black);
  LCD.Disp.Rectangle(137,57,184,75,Color.White);
  LCD.Disp.StrCenter16(58, 1, Color.Black, Color.White, Route[routeNow].Title);
  JQ8900_commandData(0x13,5,1);//volumn set:5

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  switch(mode)
	  {
	  	  case setting:
	  		  if(flagMode)
	  		  {
	  			  mode=normal;
	  			  flagRst=1;
	  			  HAL_Delay(200);
	  			  flagMode=0;
	  		  }
  			  if(flagOut)
  			  {
  				  if(routeNow==routeNum-1) routeNow=0;
  				  else routeNow++;
  	  			  LCD.Disp.Rectangle(137,57,184,75,Color.White);
  	  			  LCD.Disp.StrCenter16(58, 1, Color.Black, Color.White, Route[routeNow].Title); //display bus route
  	  			  flagOut=0;
  			  }
  			  break;
	  	  case normal:
	  		  if(stationNow==0) upNow=1;
	  		  if(stationNow==Route[routeNow].StationNum-1) upNow=0;
	  		  if(stationNext==0) upNext=1;
	  		  if(stationNext==Route[routeNow].StationNum-1) upNext=0;


	  		  if(flagRst)
	  		  {
	  			  stationNow=0;
	  			  stationNext=1;
	  			  Audio.Setup.Stop();
	  			  LCD.Disp.CharXY16(5,220,Color.Green, Color.Black,"S:X");
	  			  LCD.Disp.StrXY08(25,220,1,Color.Cyan, Color.Black,"V:3");
	  			  LCD.Disp.StrXY08(42,220,1,Color.Cyan, Color.Black,"S:60");
	  			  LCD.Disp.StrXY08(25,228,1,Color.White, Color.Black,"2018051404013");
	  			  char stationNowStr[3];
	  			  itoa(stationNow+1,stationNowStr,10);
	  			  LCD.Disp.StrXY16(295,220,0,Color.White, Color.Black,stationNowStr);
	  			  LCD.Disp.Rectangle(137,57,184,75,Color.White);
	  			  LCD.Disp.Rectangle(0,90,319,153,Color.Black); //clear previous station display
	  			  LCD.Disp.StrCenter16(58, 1, Color.Black, Color.White, Route[routeNow].Title); //display bus route
	  			  LCD.Disp.StrCenter32(90, 1, Color.White, Color.Black, Route[routeNow].Station[stationNow]); //display station now
	  			  LCD.Disp.StrCenter16(137, 1, Color.White, Color.Black, Route[routeNow].Station[stationNext]); //display station next
	  			  flagRst=0;
	  		  }
	  		  if(flagMode)
	  		  {
	  			  mode=setting;
	  			  Audio.Setup.Stop();
	  			  LCD.Disp.CharXY16(5,220,Color.Cyan, Color.Black,"S:X");
	  			  flagMode=0;
	  		  }

	  		  if(flagIn)
	  		  {
	  			  LCD.Disp.CharXY16(5,220,Color.Green, Color.Black,"S:X");
	  			  Audio.Report.In(stationNow+1);
	  			  flagIn=0;
	  		  }

	  		  if(flagOut)
	  		  {
	  			  if(upNow) stationNow++;
	  			  else stationNow--;
	  			  if(upNext) stationNext++;
	  			  else stationNext--;
	  			  char stationNowStr[3];
	  			  itoa(stationNow+1,stationNowStr,10);
	  			  LCD.Disp.CharXY16(5,220,Color.Red, Color.Black,"S:X");
	  			  LCD.Disp.StrXY16(295,220,0,Color.White, Color.Black,stationNowStr);
	  			  LCD.Disp.Rectangle(0,90,319,153,Color.Black); //clear previous station display
	  			  LCD.Disp.StrCenter32(90, 1, Color.White, Color.Black, Route[routeNow].Station[stationNow]); //display station now
	  			  LCD.Disp.StrCenter16(137, 1, Color.White, Color.Black, Route[routeNow].Station[stationNext]); //display station next
	  			  Audio.Report.Out(stationNext+1);
	  			  flagOut=0;
	  		  }

	  		  if(flagTips)
	  		  {
	  			  Audio.Report.Tips();
	  			  flagTips=0;
	  		  }
	  		  flagMode=0;
	  		  break;
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//if(HAL_GPIO_ReadPin(JQ8900_BUSY_GPIO_Port,JQ8900_BUSY_Pin)==GPIO_PIN_RESET)
	switch(GPIO_Pin)
	{
		case B_RST_Pin: flagRst=1;  break;
		case B_SET_Pin: flagMode=1; break;
		case B_UP_Pin:  flagOut=1;  break;
		case B_RHT_Pin: flagTips=1; break;
		//case B_LFT_Pin: flagOut=1;  break;
		case B_MID_Pin: flagIn=1;     break;
		//case B_LFT_Pin: flag; break;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
