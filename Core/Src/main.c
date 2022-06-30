#include "main.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

void RCC_config(void);
void GPIO_config(void);
void ADC_config(void);
uint8_t read_ADC(void);

int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  RCC_config();
  GPIO_config();
  ADC_config();

  while (1)
  {
	  read_ADC();
  }
}

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{

	__HAL_RCC_GPIOH_CLK_ENABLE();

}

void GPIO_config(void){

	RCC->AHB1ENR |= 0X00000001; // SET CLOCK PA0

	GPIOA->MODER |= 0X00000003; // SET ANALOG MODE
	GPIOA->OTYPER |= 0X00000000; // SET PUSH-PULL
	GPIOA->OSPEEDR |= 0X00000003; // SET VERY HİGH FREQ
	GPIOA->PUPDR |= 0X00000000; // SET NO PULL UP-DOWN

}

void ADC_config(void){

	RCC->APB2ENR |= (1<<8); // ADC CLOCK ENABLE

	ADC1->CR1 |= 0X02000000; // SET RESOLUTİON  8-BİT
	ADC1->CR2 |= 0X00000001; // SET ON ADC
	ADC1->SMPR2 |= 0X00000003; // SET ON 56 CYCLE
	ADC->CCR |= 0X00010000; // FOR THE 36 MHZ USE (84 MHZ / 4 =21 MHZ)
}

uint8_t read_ADC(void){

	ADC1->CR2 |= (1<<30); // START OF COVERTİON
	while(!(ADC1->SR & 0x000000002)); // CONTROL FLAG İS SET OR NOT


	return ADC1->DR;
}

void RCC_config(void){

	RCC->CR |= 0x00001000; //HSE ON
	while(!(RCC->CR |= (1<<17))); // WHİLE FLAG ACVTİVE
	RCC->CR |= (1<<19); // CSS ACTİVE
	RCC->PLLCFGR |= (1<<22); // SET HSE TO PLL
	RCC->PLLCFGR |= (1<<3); // SET PLLM İS 4
	RCC->PLLCFGR |= 0X000000A0; // SET PLLM İS 160
	RCC->PLLCFGR =0X00000000; // SET PLLP İS 2
	RCC->CFGR = 0X00000000; // AHB1 = 1
	RCC->CFGR |= (1<<15); // APB2 = 2
	RCC->CFGR |= 0X00014000; // APB1 = 4
	RCC->CIR |= 0X00080000; // CLEAR FLAG
	RCC->CIR |= 0X00800000; // CLEAR CSS
}

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
