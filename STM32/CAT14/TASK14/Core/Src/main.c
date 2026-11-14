/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : CAT 13 - BME280 over I2C1, ESP8266 MQTT publisher
  * @board          : NUCLEO-F410RB (STM32F410RB)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bme280.h"
#include "ESPLib.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* BME280 variables */
struct bme280_dev dev;
struct bme280_data comp_data;

int8_t rslt = BME280_OK;

float temperature = 0.0f;
float pressure = 0.0f;      /* hPa */
float humidity = 0.0f;      /* %RH */

static uint8_t dev_addr = BME280_I2C_ADDR_PRIM;

/* MQTT variables */
char mqttPayload[128];

uint32_t previousTime = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
int8_t user_i2c_read(uint8_t reg_addr,
                     uint8_t *reg_data,
                     uint32_t len,
                     void *intf_ptr);

int8_t user_i2c_write(uint8_t reg_addr,
                      const uint8_t *reg_data,
                      uint32_t len,
                      void *intf_ptr);

void user_delay_us(uint32_t period, void *intf_ptr);

void format_float_2(float value, char *output, size_t output_size);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
/* ==========================================
   PART A: INITIALIZE BME280
   ========================================== */

dev.intf_ptr = &dev_addr;
dev.intf = BME280_I2C_INTF;

dev.read = user_i2c_read;
dev.write = user_i2c_write;
dev.delay_us = user_delay_us;

/* Initialize BME280 */
rslt = bme280_init(&dev);

if (rslt != BME280_OK)
{
    Error_Handler();
}

/* Configure sensor settings */

struct bme280_settings settings = {0};

settings.osr_h = BME280_OVERSAMPLING_1X;
settings.osr_p = BME280_OVERSAMPLING_1X;
settings.osr_t = BME280_OVERSAMPLING_1X;

settings.filter = BME280_FILTER_COEFF_OFF;

rslt = bme280_set_sensor_settings(
            BME280_SEL_OSR_PRESS |
            BME280_SEL_OSR_TEMP |
            BME280_SEL_OSR_HUM |
            BME280_SEL_FILTER,
            &settings,
            &dev);

if (rslt != BME280_OK)
{
    Error_Handler();
}

/* Start normal measurement mode */

rslt = bme280_set_sensor_mode(
            BME280_POWERMODE_NORMAL,
            &dev);

if (rslt != BME280_OK)
{
    Error_Handler();
}


/* ==========================================
   PART B: INITIALIZE ESP8266
   ========================================== */

ESP8266_Init(StationMode, huart1);


/* ==========================================
   PART C: CONNECT TO WI-FI
   ========================================== */

/* Enter your own Wi-Fi credentials locally. Do not commit real passwords. */
ESP8266_wifiConnect("YOUR_WIFI_SSID", "YOUR_WIFI_PASSWORD");


/* ==========================================
   PART D: CONNECT TO MQTT BROKER
   ========================================== */

ESP8266_portConnect("TCP",
                    "broker.emqx.io",
                    "1883");


/* ==========================================
   PART E: CONFIGURE MQTT CLIENT
   ========================================== */

MQTT_InitTypeDef_t mqttSetting;

mqttSetting.keepAlive = 500;

mqttSetting.connectFlag = CleanSession;

mqttSetting.clientID = "STM32_Publisher";


/* ==========================================
   PART F: CONNECT MQTT CLIENT TO BROKER
   ========================================== */

MQTT_connectBroker(&mqttSetting);


/* Start periodic publishing timer */

previousTime = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		  /* Read BME280 sensor */

    rslt = bme280_get_sensor_data(
                BME280_ALL,
                &comp_data,
                &dev);

    if (rslt == BME280_OK)
    {
        /* Preserve the conversions from your working code */

        temperature = (float)comp_data.temperature;

        pressure = (float)(comp_data.pressure / 100.0);

        humidity = (float)comp_data.humidity;


        /* Publish once per approximately 1-second cycle */

        if (HAL_GetTick() - previousTime >= 1000U)
        {
            previousTime = HAL_GetTick();

            char tempStr[16];
            char humStr[16];
            char pressStr[16];

            /* Convert measurements to decimal strings */

            format_float_2(
                temperature,
                tempStr,
                sizeof(tempStr));

            format_float_2(
                humidity,
                humStr,
                sizeof(humStr));

            format_float_2(
                pressure,
                pressStr,
                sizeof(pressStr));


            /* Create JSON payload */

            snprintf(
                mqttPayload,
                sizeof(mqttPayload),
                "{\"temperature\":%s,"
                "\"humidity\":%s,"
                "\"pressure\":%s}",
                tempStr,
                humStr,
                pressStr);


            /* Publish actual BME280 measurements */

            MQTT_publishTopic(
                "agrisense/rice/bme280",
                mqttPayload,
                QoS0);
        }
    }

    /* Short delay prevents continuously polling the sensor */

    HAL_Delay(100);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
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

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void format_float_2(float value,
                    char *output,
                    size_t output_size)
{
    int32_t scaled = (int32_t)(value * 100.0f);

    uint32_t magnitude;

    if (scaled < 0)
    {
        magnitude = (uint32_t)(-scaled);
    }
    else
    {
        magnitude = (uint32_t)scaled;
    }

    snprintf(
        output,
        output_size,
        "%s%lu.%02lu",
        (scaled < 0) ? "-" : "",
        (unsigned long)(magnitude / 100U),
        (unsigned long)(magnitude % 100U));
}


/* BME280 I2C READ */

int8_t user_i2c_read(uint8_t reg_addr,
                     uint8_t *reg_data,
                     uint32_t len,
                     void *intf_ptr)
{
    uint8_t address = *(uint8_t *)intf_ptr;

    if (HAL_I2C_Mem_Read(
            &hi2c1,
            (uint16_t)(address << 1),
            reg_addr,
            I2C_MEMADD_SIZE_8BIT,
            reg_data,
            (uint16_t)len,
            HAL_MAX_DELAY) == HAL_OK)
    {
        return BME280_OK;
    }

    return BME280_E_COMM_FAIL;
}


/* BME280 I2C WRITE */

int8_t user_i2c_write(uint8_t reg_addr,
                      const uint8_t *reg_data,
                      uint32_t len,
                      void *intf_ptr)
{
    uint8_t address = *(uint8_t *)intf_ptr;

    if (HAL_I2C_Mem_Write(
            &hi2c1,
            (uint16_t)(address << 1),
            reg_addr,
            I2C_MEMADD_SIZE_8BIT,
            (uint8_t *)reg_data,
            (uint16_t)len,
            HAL_MAX_DELAY) == HAL_OK)
    {
        return BME280_OK;
    }

    return BME280_E_COMM_FAIL;
}


/* BME280 SENSOR DELAY */

void user_delay_us(uint32_t period, void *intf_ptr)
{
    (void)intf_ptr;

    HAL_Delay((period / 1000U) + 1U);
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
