/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : CAT10 Wemos ESP8266 UART tasks (Task 9A / Task 9C)
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

UART_HandleTypeDef huart1; /* USART1: STM32 <-> Wemos D1 mini */
UART_HandleTypeDef huart2; /* USART2: STM32 <-> PC terminal */

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);

static void PC_Print(const char *text)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)text, strlen(text), HAL_MAX_DELAY);
}

/* Task 9A: send AT and display Wemos response on the PC terminal.
   Then provide a transparent two-way UART bridge for further AT testing. */
static void Task9A_UART_Test(void)
{
    uint8_t rxByte;

    PC_Print("\r\nTASK 9A - STM32 <-> Wemos UART test\r\n");
    PC_Print("Sending AT command to ESP8266...\r\n");

    const uint8_t atCommand[] = "AT\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t *)atCommand,
                      sizeof(atCommand) - 1, 1000);

    uint32_t start = HAL_GetTick();
    uint8_t receivedAny = 0;

    while ((HAL_GetTick() - start) < 4000)
    {
        if (HAL_UART_Receive(&huart1, &rxByte, 1, 20) == HAL_OK)
        {
            HAL_UART_Transmit(&huart2, &rxByte, 1, 100);
            receivedAny = 1;
        }
    }

    if (!receivedAny)
        PC_Print("\r\nNo response. Check TX/RX, common GND, baud rate and ESP power.\r\n");
    else
        PC_Print("\r\n\r\nUART response received.\r\n");

    PC_Print("UART bridge ready: type AT commands in the PC terminal.\r\n");

    while (1)
    {
        /* PC terminal -> Wemos */
        if (HAL_UART_Receive(&huart2, &rxByte, 1, 1) == HAL_OK)
            HAL_UART_Transmit(&huart1, &rxByte, 1, 100);

        /* Wemos -> PC terminal */
        if (HAL_UART_Receive(&huart1, &rxByte, 1, 1) == HAL_OK)
            HAL_UART_Transmit(&huart2, &rxByte, 1, 100);
    }
}

/* Send one AT command, collect the response, and print it on the PC.
   The command must include CR/LF. */
static HAL_StatusTypeDef ESP_SendCommand(const char *command,
                                         uint32_t timeout,
                                         char *response,
                                         uint16_t responseSize)
{
    uint8_t ch;
    uint16_t pos = 0;
    uint32_t start = HAL_GetTick();

    memset(response, 0, responseSize);

    PC_Print("\r\n>> ");
    PC_Print(command);

    if (HAL_UART_Transmit(&huart1, (uint8_t *)command,
                          strlen(command), 2000) != HAL_OK)
        return HAL_ERROR;

    while ((HAL_GetTick() - start) < timeout && pos < responseSize - 1)
    {
        if (HAL_UART_Receive(&huart1, &ch, 1, 20) == HAL_OK)
        {
            response[pos++] = (char)ch;
            response[pos] = '\0';

            if (strstr(response, "\r\nOK\r\n") != NULL ||
                strstr(response, "\nOK\n") != NULL)
                break;

            if (strstr(response, "ERROR") != NULL ||
                strstr(response, "FAIL") != NULL)
                break;
        }
    }

    if (pos > 0)
        HAL_UART_Transmit(&huart2, (uint8_t *)response, pos, 2000);
    else
        PC_Print("\r\n[No response]\r\n");

    if (strstr(response, "OK") != NULL &&
        strstr(response, "ERROR") == NULL &&
        strstr(response, "FAIL") == NULL)
        return HAL_OK;

    return HAL_ERROR;
}

/* Task 9C: configure ESP8266 as a station, join an access point,
   and query the assigned IP address. Set credentials before building. */
static void Task9C_ConnectWiFi(void)
{
#define WIFI_SSID       "YOUR_WIFI_SSID"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"

    char response[700];
    char joinCommand[180];

    PC_Print("\r\nTASK 9C - Connect Wemos ESP8266 to Wi-Fi\r\n");
    PC_Print("Keep the PC terminal open at 115200 baud, 8-N-1.\r\n");

    HAL_Delay(2000);

    if (ESP_SendCommand("AT\r\n", 3000, response, sizeof(response)) != HAL_OK)
        PC_Print("\r\nCheck ESP8266 AT firmware and UART wiring.\r\n");

    if (ESP_SendCommand("AT+CWMODE=1\r\n", 3000,
                        response, sizeof(response)) != HAL_OK)
        Error_Handler();

    snprintf(joinCommand, sizeof(joinCommand),
             "AT+CWJAP=\"%s\",\"%s\"\r\n",
             WIFI_SSID, WIFI_PASSWORD);

    PC_Print("\r\nJoining Wi-Fi. This may take several seconds...\r\n");
    if (ESP_SendCommand(joinCommand, 40000,
                        response, sizeof(response)) != HAL_OK)
        Error_Handler();

    if (ESP_SendCommand("AT+CIFSR\r\n", 5000,
                        response, sizeof(response)) != HAL_OK)
        Error_Handler();

    PC_Print("\r\nTask 9C completed. Check the IP address printed above.\r\n");
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();

    /* Select the task to run:
       Task9A_UART_Test();
       Task9A_UART_Test();
       Keep only ONE function call active at a time. */

    Task9A_UART_Test();

    while (1)
    {
        HAL_Delay(1000);
    }
}

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
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
