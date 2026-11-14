/* USER CODE BEGIN Header */
/**
  * @file           : main.c
  * @brief          : STM32 UART experiment
  */
/* USER CODE END Header */
#include "main.h"

/* USER CODE BEGIN Header */
/**
  * @file           : main.c
  * @brief          : Task 9B - UART String-Based LED Command Interface
  */
/* USER CODE END Header */
#include "main.h"
#include <string.h>
#include <ctype.h>

UART_HandleTypeDef huart2;
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

#define LED_PORT GPIOC
#define LED_PIN GPIO_PIN_13
#define COMMAND_BUFFER_SIZE 32

/* Blue Pill onboard LED on PC13 is active-low. */
static void LED_On(void)
{
  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

static void LED_Off(void)
{
  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}

static void LED_Toggle(void)
{
  HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}

static void UART_SendMessage(const char *message)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)message,
                    (uint16_t)strlen(message), HAL_MAX_DELAY);
}

static void ProcessCommand(char *command)
{
  uint8_t i;
  for (i = 0; command[i] != '\0'; i++)
  {
    command[i] = (char)toupper((unsigned char)command[i]);
  }

  if (strcmp(command, "LED ON") == 0)
  {
    LED_On();
    UART_SendMessage("LED turned ON\r\n");
  }
  else if (strcmp(command, "LED OFF") == 0)
  {
    LED_Off();
    UART_SendMessage("LED turned OFF\r\n");
  }
  else if (strcmp(command, "LED TOGGLE") == 0)
  {
    LED_Toggle();
    UART_SendMessage("LED toggled\r\n");
  }
  else if (strcmp(command, "HELP") == 0)
  {
    UART_SendMessage("Commands: LED ON, LED OFF, LED TOGGLE\r\n");
  }
  else if (command[0] != '\0')
  {
    UART_SendMessage("Invalid command. Type HELP.\r\n");
  }
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  uint8_t receivedChar;
  char commandBuffer[COMMAND_BUFFER_SIZE];
  uint8_t bufferIndex = 0;

  LED_Off();
  UART_SendMessage("\r\nTask 9B: UART LED Command Interface\r\n");
  UART_SendMessage("Enter: LED ON | LED OFF | LED TOGGLE\r\n> ");

  while (1)
  {
    if (HAL_UART_Receive(&huart2, &receivedChar, 1, HAL_MAX_DELAY) == HAL_OK)
    {
      if (receivedChar == '\r' || receivedChar == '\n')
      {
        if (bufferIndex > 0)
        {
          commandBuffer[bufferIndex] = '\0';
          UART_SendMessage("\r\n");
          ProcessCommand(commandBuffer);
          bufferIndex = 0;
          UART_SendMessage("> ");
        }
      }
      else if (receivedChar == '\b' || receivedChar == 127)
      {
        if (bufferIndex > 0)
        {
          bufferIndex--;
          UART_SendMessage("\b \b");
        }
      }
      else if (bufferIndex < (COMMAND_BUFFER_SIZE - 1))
      {
        commandBuffer[bufferIndex++] = (char)receivedChar;
        HAL_UART_Transmit(&huart2, &receivedChar, 1, HAL_MAX_DELAY);
      }
      else
      {
        UART_SendMessage("\r\nCommand too long. Try again.\r\n> ");
        bufferIndex = 0;
      }
    }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}


/**
  * @brief GPIO Initialization Function
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* Blue Pill onboard LED: PC13, active-low */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

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
#ifdef USE_FULL_ASSERT
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
