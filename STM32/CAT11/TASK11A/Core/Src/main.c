
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* Wi-Fi credentials */
/* Set these locally before building. Do not publish real credentials. */
#define WIFI_NAME       "YOUR_WIFI_SSID"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"

/* Buffer sizes */
#define CMD_SIZE        150
#define REQUEST_SIZE    1200
#define RESPONSE_SIZE   500
#define RX_SIZE         500

UART_HandleTypeDef huart1;

/* Global variables */
char cmd[CMD_SIZE];
char request[REQUEST_SIZE];
char response[RESPONSE_SIZE];
char rxBuffer[RX_SIZE];

uint8_t rx;
int link_id = -1;

/* Function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

int sendAT(char *command, uint32_t timeout);
int waitForPrompt(void);
int receiveRequest(void);
int sendHttpResponse(void);
int waitForSendOK(void);
void ESP8266_Init(void);


/* =====================================================
   1. SEND AT COMMAND AND RECEIVE RESPONSE
   ===================================================== */

int sendAT(char *command, uint32_t timeout)
{
    memset(rxBuffer, 0, sizeof(rxBuffer));

    if (HAL_UART_Transmit(&huart1, (uint8_t *)command,
                          strlen(command), 1000) != HAL_OK)
        return 0;

    uint32_t start = HAL_GetTick();
    int i = 0;

    while ((HAL_GetTick() - start) < timeout)
    {
        if (HAL_UART_Receive(&huart1, &rx, 1, 100) == HAL_OK)
        {
            if (i < RX_SIZE - 1)
            {
                rxBuffer[i++] = (char)rx;
                rxBuffer[i] = '\0';
            }

            if (strstr(rxBuffer, "\r\nOK\r\n") ||
                strstr(rxBuffer, "ERROR") ||
                strstr(rxBuffer, "FAIL"))
                break;
        }
    }

    return (strstr(rxBuffer, "OK") != NULL &&
            strstr(rxBuffer, "ERROR") == NULL &&
            strstr(rxBuffer, "FAIL") == NULL);
}


/* =====================================================
   2. INITIALIZE ESP8266
   ===================================================== */

void ESP8266_Init(void)
{
    HAL_Delay(2000);

    if (!sendAT("AT\r\n", 3000))
        Error_Handler();

    if (!sendAT("AT+CWMODE=1\r\n", 3000))
        Error_Handler();

    snprintf(cmd, sizeof(cmd),
             "AT+CWJAP=\"%s\",\"%s\"\r\n",
             WIFI_NAME, WIFI_PASSWORD);

    if (!sendAT(cmd, 40000))
        Error_Handler();

    HAL_Delay(1000);

    if (!sendAT("AT+CIFSR\r\n", 3000))
        Error_Handler();

    if (!sendAT("AT+CIPMUX=1\r\n", 3000))
        Error_Handler();

    if (!sendAT("AT+CIPSERVER=1,80\r\n", 5000))
        Error_Handler();

    HAL_Delay(500);
}


/* =====================================================
   3. WAIT FOR ESP8266 '>' PROMPT
   ===================================================== */

int waitForPrompt(void)
{
    memset(rxBuffer, 0, sizeof(rxBuffer));

    uint32_t start = HAL_GetTick();
    int i = 0;

    while ((HAL_GetTick() - start) < 5000)
    {
        if (HAL_UART_Receive(&huart1, &rx, 1, 100) == HAL_OK)
        {
            if (rx == '>')
                return 1;

            if (i < RX_SIZE - 1)
            {
                rxBuffer[i++] = (char)rx;
                rxBuffer[i] = '\0';
            }

            if (strstr(rxBuffer, "ERROR") ||
                strstr(rxBuffer, "FAIL"))
                return 0;
        }
    }

    return 0;
}


/* =====================================================
   4. RECEIVE BROWSER HTTP REQUEST
   ===================================================== */

int receiveRequest(void)
{
    const char marker[] = "+IPD,";
    int matched = 0;
    int i = 0;

    memset(request, 0, sizeof(request));

    uint32_t start = HAL_GetTick();

    /* Search for +IPD */
    while ((HAL_GetTick() - start) < 15000)
    {
        if (HAL_UART_Receive(&huart1, &rx, 1, 100) == HAL_OK)
        {
            if (rx == marker[matched])
            {
                matched++;
            }
            else
            {
                matched = (rx == marker[0]) ? 1 : 0;
            }

            if (matched == sizeof(marker) - 1)
            {
                memcpy(request, marker, sizeof(marker) - 1);
                i = sizeof(marker) - 1;
                break;
            }
        }
    }

    if (i == 0)
        return 0;

    /* Read the remaining HTTP request */
    start = HAL_GetTick();

    while ((HAL_GetTick() - start) < 5000 &&
           i < REQUEST_SIZE - 1)
    {
        if (HAL_UART_Receive(&huart1, &rx, 1, 100) == HAL_OK)
        {
            request[i++] = (char)rx;
            request[i] = '\0';

            /* End of HTTP headers */
            if (strstr(request, "\r\n\r\n") != NULL)
            {
                if (sscanf(request, "+IPD,%d,", &link_id) == 1)
                    return 1;

                return 0;
            }
        }
    }

    return 0;
}


/* =====================================================
   5. WAIT FOR SEND OK
   ===================================================== */

int waitForSendOK(void)
{
    memset(rxBuffer, 0, sizeof(rxBuffer));

    uint32_t start = HAL_GetTick();
    int i = 0;

    while ((HAL_GetTick() - start) < 5000)
    {
        if (HAL_UART_Receive(&huart1, &rx, 1, 100) == HAL_OK)
        {
            if (i < RX_SIZE - 1)
            {
                rxBuffer[i++] = (char)rx;
                rxBuffer[i] = '\0';
            }

            if (strstr(rxBuffer, "SEND OK"))
                return 1;

            if (strstr(rxBuffer, "SEND FAIL") ||
                strstr(rxBuffer, "ERROR"))
                return 0;
        }
    }

    return 0;
}


/* =====================================================
   6. SEND HTML RESPONSE TO BROWSER
   ===================================================== */

int sendHttpResponse(void)
{
    if (link_id < 0 || link_id > 4)
        return 0;

    char html[] =
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<title>STM32 ESP8266 Server</title>"
        "</head>"
        "<body>"
        "<h1>ESP8266 HTTP Server Works!</h1>"
        "<p>STM32F410RB successfully sent this webpage.</p>"
        "<h2>HTTP Communication Successful!</h2>"
        "</body>"
        "</html>";

    /* Prepare HTTP response */
    int htmlLength = strlen(html);

    int responseLength = snprintf(
        response,
        sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        htmlLength,
        html
    );

    if (responseLength <= 0 ||
        responseLength >= RESPONSE_SIZE)
        return 0;

    /* Send CIPSEND command */
    snprintf(cmd, sizeof(cmd),
             "AT+CIPSEND=%d,%d\r\n",
             link_id, responseLength);

    if (HAL_UART_Transmit(&huart1, (uint8_t *)cmd,
                          strlen(cmd), 1000) != HAL_OK)
        return 0;

    /* Wait for > prompt */
    if (!waitForPrompt())
        return 0;

    /* Transmit HTML response */
    if (HAL_UART_Transmit(&huart1,
                          (uint8_t *)response,
                          responseLength, 3000) != HAL_OK)
        return 0;

    /* Wait for SEND OK */
    if (!waitForSendOK())
        return 0;

    /* Close connection */
    snprintf(cmd, sizeof(cmd),
             "AT+CIPCLOSE=%d\r\n", link_id);

    HAL_UART_Transmit(&huart1, (uint8_t *)cmd,
                      strlen(cmd), 1000);

    HAL_Delay(200);

    return 1;
}


/* =====================================================
   7. MAIN FUNCTION
   ===================================================== */

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    MX_USART1_UART_Init();

    /* Initialize Wi-Fi and HTTP server */
    ESP8266_Init();

    /* Main loop */
    while (1)
    {
        if (receiveRequest())
        {
            sendHttpResponse();
        }
    }
}


/* =====================================================
   8. SYSTEM CLOCK CONFIGURATION
   ===================================================== */

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(
        PWR_REGULATOR_VOLTAGE_SCALE1);

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

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct,
                            FLASH_LATENCY_3) != HAL_OK)
        Error_Handler();
}


/* =====================================================
   9. USART1 INITIALIZATION
   ===================================================== */

static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart1) != HAL_OK)
        Error_Handler();
}


/* =====================================================
   10. GPIO INITIALIZATION
   ===================================================== */

static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
}


/* =====================================================
   11. ERROR HANDLER
   ===================================================== */

void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}
