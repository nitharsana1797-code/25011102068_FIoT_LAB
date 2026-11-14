/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : CAT 12 - BME280 I2C1 sensing with UART2 JSON output
  * @board          : NUCLEO-F410RB (STM32F410RB)
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include "bme280.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Peripheral handles */
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;

/* BME280 state and measurements */
static struct bme280_dev dev;
static struct bme280_data comp_data;
static struct bme280_settings settings;
static int8_t rslt;
static uint8_t dev_addr = BME280_I2C_ADDR_PRIM; /* SDO low: 0x76 */

static float temperature_c;
static float pressure_hpa;
static float humidity_rh;

static char json_payload[160];
static char http_response[320];

/* Function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
void Error_Handler(void);

static void UART2_SendString(const char *s);
static void Format2(char *out, size_t out_size, float value);
static void BuildJsonAndHttpResponse(void);
static void BME280_Init(void);
static void BME280_ReadAndReport(void);

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data,
                     uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data,
                      uint32_t len, void *intf_ptr);
void user_delay_us(uint32_t period, void *intf_ptr);

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART2_UART_Init();

    UART2_SendString("\r\nCAT12 - NUCLEO-F410RB / BME280\r\n");

    BME280_Init();

    while (1)
    {
        BME280_ReadAndReport();
        HAL_Delay(1000);
    }
}

/* Configure Bosch Sensor API interface and operating parameters. */
static void BME280_Init(void)
{
    dev.intf_ptr = &dev_addr;
    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_us = user_delay_us;

    rslt = bme280_init(&dev);
    if (rslt != BME280_OK)
    {
        UART2_SendString("ERROR: BME280 init failed. Check I2C wiring/address.\r\n");
        Error_Handler();
    }

    settings.osr_h = BME280_OVERSAMPLING_1X;
    settings.osr_p = BME280_OVERSAMPLING_1X;
    settings.osr_t = BME280_OVERSAMPLING_1X;
    settings.filter = BME280_FILTER_COEFF_OFF;
    settings.standby_time = BME280_STANDBY_TIME_0_5_MS;

    rslt = bme280_set_sensor_settings(
        BME280_SEL_OSR_PRESS |
        BME280_SEL_OSR_TEMP  |
        BME280_SEL_OSR_HUM   |
        BME280_SEL_FILTER    |
        BME280_SEL_STANDBY,
        &settings,
        &dev);

    if (rslt != BME280_OK)
    {
        UART2_SendString("ERROR: BME280 settings configuration failed.\r\n");
        Error_Handler();
    }

    rslt = bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &dev);
    if (rslt != BME280_OK)
    {
        UART2_SendString("ERROR: BME280 normal mode configuration failed.\r\n");
        Error_Handler();
    }

    UART2_SendString("BME280 initialized successfully.\r\n");
}

/* Read compensated values, construct JSON and print through UART2. */
static void BME280_ReadAndReport(void)
{
    char t[20], h[20], p[20];

    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);
    if (rslt != BME280_OK)
    {
        UART2_SendString("ERROR: BME280 data read failed.\r\n");
        return;
    }

    temperature_c = (float)comp_data.temperature;
    pressure_hpa = (float)(comp_data.pressure / 100.0);
    humidity_rh = (float)comp_data.humidity;

    Format2(t, sizeof(t), temperature_c);
    Format2(h, sizeof(h), humidity_rh);
    Format2(p, sizeof(p), pressure_hpa);

    (void)snprintf(json_payload, sizeof(json_payload),
                   "{\"status\":\"success\",\"temperature\":%s,"
                   "\"humidity\":%s,\"pressure\":%s}",
                   t, h, p);

    BuildJsonAndHttpResponse();

    UART2_SendString("BME280 JSON: ");
    UART2_SendString(json_payload);
    UART2_SendString("\r\n");

    /* Full HTTP response is printed for inspection.
       Send this buffer using ESP8266 AT+CIPSEND in the Wi-Fi integration stage. */
    UART2_SendString("HTTP response preview:\r\n");
    UART2_SendString(http_response);
    UART2_SendString("\r\n");
}

/* Format a float to two decimal places without requiring printf %f support. */
static void Format2(char *out, size_t out_size, float value)
{
    int32_t scaled;
    int32_t whole;
    int32_t fraction;

    if (value >= 0.0f)
        scaled = (int32_t)(value * 100.0f + 0.5f);
    else
        scaled = (int32_t)(value * 100.0f - 0.5f);

    whole = scaled / 100;
    fraction = scaled % 100;
    if (fraction < 0)
        fraction = -fraction;

    (void)snprintf(out, out_size, "%ld.%02ld",
                   (long)whole, (long)fraction);
}

static void BuildJsonAndHttpResponse(void)
{
    int content_length = (int)strlen(json_payload);

    (void)snprintf(http_response, sizeof(http_response),
                   "HTTP/1.1 200 OK\r\n"
                   "Content-Type: application/json\r\n"
                   "Content-Length: %d\r\n"
                   "Connection: close\r\n"
                   "\r\n"
                   "%s",
                   content_length, json_payload);
}

static void UART2_SendString(const char *s)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)s, (uint16_t)strlen(s),
                      HAL_MAX_DELAY);
}

/* Bosch API I2C read callback. */
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data,
                     uint32_t len, void *intf_ptr)
{
    uint8_t address = *(uint8_t *)intf_ptr;

    if (HAL_I2C_Mem_Read(&hi2c1, (uint16_t)(address << 1),
                         reg_addr, I2C_MEMADD_SIZE_8BIT,
                         reg_data, (uint16_t)len, HAL_MAX_DELAY) == HAL_OK)
        return BME280_OK;

    return BME280_E_COMM_FAIL;
}

/* Bosch API I2C write callback. */
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data,
                      uint32_t len, void *intf_ptr)
{
    uint8_t address = *(uint8_t *)intf_ptr;

    if (HAL_I2C_Mem_Write(&hi2c1, (uint16_t)(address << 1),
                          reg_addr, I2C_MEMADD_SIZE_8BIT,
                          (uint8_t *)reg_data, (uint16_t)len,
                          HAL_MAX_DELAY) == HAL_OK)
        return BME280_OK;

    return BME280_E_COMM_FAIL;
}

/* Bosch callback expects microseconds; HAL_Delay has millisecond resolution. */
void user_delay_us(uint32_t period, void *intf_ptr)
{
    (void)intf_ptr;
    HAL_Delay((period / 1000U) + 1U);
}

/**
  * NUCLEO-F410RB clock setup used in the earlier F410RB lab configuration:
  * 8 MHz HSE external clock input in bypass mode -> 100 MHz SYSCLK.
  * Verify HSE source/frequency against the exact board and CubeMX project.
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

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
        Error_Handler();

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |
                                  RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 |
                                  RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
        Error_Handler();
}

static void MX_I2C1_Init(void)
{
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
        Error_Handler();
}

static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK)
        Error_Handler();
}

static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
}
#endif
