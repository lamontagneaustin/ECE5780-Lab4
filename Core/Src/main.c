/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
	
#include "main.h"

void SystemClock_Config(void);

/**
	* @brief Transmits char through USART3.
	* @retval None
	*/
void character_transmit(char test){

	while (!(USART3->ISR &= 0x80)){
	}
	USART3->TDR = 0x41;
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
	
	RCC->AHBENR  |= (RCC_AHBENR_GPIOBEN)|(RCC_AHBENR_GPIOCEN); // Enables the GPIOB/GPIOC clock in the RCC.
	RCC->APB1ENR |= (RCC_APB1ENR_USART3EN); // Enables the USART3 clock in the RCC.
	
	// Configuring PB10 and PB11 for Alternate Function Mode USART3 TX and RX.
	GPIOB->MODER  |=  (1 << 21) | (1 << 23);   // Sets PB10 and PB11 To Alternate Function Mode(1 bits).
	GPIOB->MODER  &= ~((1 << 20) | (1 << 22)); // Sets PB10 and PB11 To Alternate Function Mode(0 bits).
	GPIOB->AFR[1] &= ~((1 << 8)|(1 << 9)|(1 << 11)); // Selects Function Mode USART3_TX for PB10 (0 bits).
	GPIOB->AFR[1] |=  (1 << 10); // Selects Function Mode USART3_TX for PB10 (1 bits).
	GPIOB->AFR[1] &= ~((1 << 12)|(1 << 13)|(1 << 15)); // Selects Function Mode USART3_RX for PB11 (0 bits).
	GPIOB->AFR[1] |=  (1 << 14); // Selects Function Mode USART3_RX for PB11 (1 bits).

	USART3->BRR = 0x45; // Sets BRR to 69, making Baud Rate 115200 (115942 actual).
	USART3->CR1 |= (1 << 0) | (1 << 2) | (1 << 3); // USART Enable to 1, Transmitter Enable, and Reciever Enable.
	
  while (1)
  {
		character_transmit('a');
		HAL_Delay(1000);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
