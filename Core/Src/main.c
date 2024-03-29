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

char globalTemp1;
char globalTemp2;
char globalFlag;

/**
	* @brief Transmits char through USART3.
	* @retval None
	*/
void character_transmit(char transmit_char){

	while (!(USART3->ISR &= 0x80)){
	}
	USART3->TDR = transmit_char;
}

/**
	* @brief Transmits string through USART3.
	* @retval None
	*/
void string_transmit(char transmit_string[]){
	int i = 0;
	while(transmit_string[i] != 0){
		character_transmit(transmit_string[i]);
	i++;
	}
	return;
}

/**
	* @brief Toggles RED LED.
	* @retval None
	*/
void toggleRED(void){
	// Toggle Pin PC6 (RED).
	if(GPIOC->IDR & 0x40){
		GPIOC->BSRR |= (1 << 22); // Resets State of PC6.
	}
	else{
		GPIOC->BSRR |= (1 << 6); // Sets State of PC6.
	}
}

/**
	* @brief Sets RED LED.
	* @retval None
	*/
void setRED(char value){
	if(value == '1'){
		GPIOC->BSRR |= (1 << 6); // Sets State of PC6.
	}
	else if(value == '0'){
		GPIOC->BSRR |= (1 << 22); // Resets State of PC6.
	}
}

/**
	* @brief Toggles BLUE LED.
	* @retval None
	*/
void toggleBLUE(void){
	// Toggle Pin PC7 (BLUE).
	if(GPIOC->IDR & 0x80){
		GPIOC->BSRR |= (1 << 23); // Resets State of PC7.
	}
	else{
		GPIOC->BSRR |= (1 << 7); // Sets State of PC7.
	}
}

/**
	* @brief Sets BLUE LED.
	* @retval None
	*/
void setBLUE(char value){
	if(value == '1'){
		GPIOC->BSRR |= (1 << 7); // Sets State of PC7.
	}
	else if(value == '0'){
		GPIOC->BSRR |= (1 << 23); // Resets State of PC7.
	}
}

/**
	* @brief Toggles ORANGE LED.
	* @retval None
	*/
void toggleORANGE(void){
	// Toggle Pin PC8 (ORANGE).
	if(GPIOC->IDR & 0x100){
		GPIOC->BSRR |= (1 << 24); // Resets State of PC8.
	}
	else{
		GPIOC->BSRR |= (1 << 8); // Sets State of PC8.
	}
}

/**
	* @brief Sets ORANGE LED.
	* @retval None
	*/
void setORANGE(char value){
	if(value == '1'){
		GPIOC->BSRR |= (1 << 8); // Sets State of PC8.
	}
	else if(value == '0'){
		GPIOC->BSRR |= (1 << 24); // Resets State of PC8.
	}
}

/**
	* @brief Toggles GREEN LED.
	* @retval None
	*/
void toggleGREEN(void){
	// Toggle Pin PC9 (GREEN).
	if(GPIOC->IDR & 0x200){
		GPIOC->BSRR |= (1 << 25); // Resets State of PC9.
	}
	else{
		GPIOC->BSRR |= (1 << 9); // Sets State of PC9.
	}
}

/**
	* @brief Sets GREEN LED.
	* @retval None
	*/
void setGREEN(char value){
	if(value == '1'){
		GPIOC->BSRR |= (1 << 9); // Sets State of PC9.
	}
	else if(value == '0'){
		GPIOC->BSRR |= (1 << 25); // Resets State of PC9.
	}
}

void RESETFLAGS(void){
	string_transmit(" CMD:");
	globalFlag = 0x0;
}

void SOLVEPROBLEM(void){
	if(globalFlag == 0x2){
			if(globalTemp1 == 'r' | globalTemp1 == 'R'){
				if(globalTemp2 == '2'){
					toggleRED();
				}
				else if(globalTemp2 == '1' | globalTemp2 == '0'){
					setRED(globalTemp2);
				}
				else{string_transmit(" UNKNOWN COMMAND.");}
				RESETFLAGS();
			}
			else if(globalTemp1 == 'b' | globalTemp1 == 'B'){
				if(globalTemp2 == '2'){
					toggleBLUE();
				}
				else if(globalTemp2 == '1' | globalTemp2 == '0'){
					setBLUE(globalTemp2);
				}
				else{string_transmit(" UNKNOWN COMMAND.");}
				RESETFLAGS();
			}
			else if(globalTemp1 == 'g' | globalTemp1 == 'G'){
				if(globalTemp2 == '2'){
					toggleGREEN();
				}
				else if(globalTemp2 == '1' | globalTemp2 == '0'){
					setGREEN(globalTemp2);
				}
				else{string_transmit(" UNKNOWN COMMAND.");}
				RESETFLAGS();
			}
			else if(globalTemp1 == 'o' | globalTemp1 == 'O'){
				if(globalTemp2 == '2'){
					toggleORANGE();
				}
				else if(globalTemp2 == '1' | globalTemp2 == '0'){
					setORANGE(globalTemp2);
				}
				else{string_transmit(" UNKNOWN COMMAND.");}
				RESETFLAGS();
			}
			else{
				string_transmit(" UNKNOWN COMMAND.");
				RESETFLAGS();
			}
		}
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	char temp;
	globalTemp1 = 0x0;
	globalTemp2 = 0x0;
	globalFlag = 0x0;
	
  HAL_Init();
  SystemClock_Config();
	
	RCC->AHBENR  |= (RCC_AHBENR_GPIOBEN)|(RCC_AHBENR_GPIOCEN); // Enables the GPIOB/GPIOC clock in the RCC.
	RCC->APB1ENR |= (RCC_APB1ENR_USART3EN); // Enables the USART3 clock in the RCC.
	
	// Configures GPIOC Pins 8 and 9 (ORANGE LED and GREEN LED)
	GPIOC->MODER   |=  (1 << 16) | (1 << 18);
	GPIOC->OTYPER  &= ~((1 << 8) | (1 << 9));
	GPIOC->OSPEEDR &= ~((1 << 16) | (1 << 18));
	GPIOC->PUPDR   &= ~((1 << 16) | (1 << 17) | (1 << 18) | (1 << 19));
	
	// Configures GPIOC Pins 6 and 7 (RED LED and BLUE LED)
	GPIOC->MODER   |=  (1 << 12) | (1 << 14);
	GPIOC->OTYPER  &= ~((1 << 6) | (1 << 7));
	GPIOC->OSPEEDR &= ~((1 << 12) | (1 << 14));
	GPIOC->PUPDR   &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15));
	
	// Configuring PB10 and PB11 for Alternate Function Mode USART3 TX and RX.
	GPIOB->MODER  |=  (1 << 21) | (1 << 23);   // Sets PB10 and PB11 To Alternate Function Mode(1 bits).
	GPIOB->MODER  &= ~((1 << 20) | (1 << 22)); // Sets PB10 and PB11 To Alternate Function Mode(0 bits).
	GPIOB->AFR[1] &= ~((1 << 8)|(1 << 9)|(1 << 11)); // Selects Function Mode USART3_TX for PB10 (0 bits).
	GPIOB->AFR[1] |=  (1 << 10); // Selects Function Mode USART3_TX for PB10 (1 bits).
	GPIOB->AFR[1] &= ~((1 << 12)|(1 << 13)|(1 << 15)); // Selects Function Mode USART3_RX for PB11 (0 bits).
	GPIOB->AFR[1] |=  (1 << 14); // Selects Function Mode USART3_RX for PB11 (1 bits).

	USART3->CR1 |= (1 << 5); // RXNE(Recieve Register Not Empty) interrupt enable to 1. 
	USART3->BRR = 0x45; // Sets BRR to 69, making Baud Rate 115200 (115942 actual).
	USART3->CR1 |= (1 << 0) | (1 << 2) | (1 << 3); // USART Enable to 1, Transmitter Enable, and Reciever Enable.
	
	NVIC_EnableIRQ(USART3_4_IRQn);
	
	string_transmit(" CMD:");
  while (1)
  {
		//character_transmit('~');
		//string_transmit("Hello World!");
		//HAL_Delay(1000);
		
		/*
		if(USART3->ISR &= 0x20){ // If USART_RDR has data.
			temp = USART3->RDR;
			if(temp == 'r' | temp == 'R'){
				toggleRED();
			}
			else if(temp == 'b' | temp == 'B'){
				toggleBLUE();
			}
			else if(temp == 'g' | temp == 'G'){
				toggleGREEN();
			}
			else if(temp == 'o' | temp == 'O'){
				toggleORANGE();
			}
			else{
				string_transmit("Non-Allowed Keystroke. Type 'r', 'g', 'b', or 'o'.");
			}
		}
		*/
		SOLVEPROBLEM();
	}
}

void USART3_4_IRQHandler(void) {
	if(globalFlag == 0x0){
		globalTemp1 = USART3->RDR;
		character_transmit(globalTemp1);
		globalFlag = 0x1;
	}
	else if(globalFlag == 0x1){
		globalTemp2 = USART3->RDR;
		character_transmit(globalTemp2);
		globalFlag = 0x2;
	}
	else{ globalFlag = 0x0; }
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
