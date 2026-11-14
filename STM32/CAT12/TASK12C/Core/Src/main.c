/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : CAT 11 - STM32F410RB GPIO exercise
  * @board          : NUCLEO-F410RB
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Error_Handler(void);

/*
 * Clock assumption:
 * NUCLEO-F410RB external clock input from ST-LINK MCO = 8 MHz.
 * PLL settings below generate SYSCLK = 84 MHz.
 *
 * On-board LD2 LED: PA5, active HIGH.
 * On-board B1 user button: PC13, active HIGH.
 */


#define DEBOUNCE_MS       30U
#define LONG_PRESS_MS   1000U
#define BLINK_INTERVAL   250U

int main(void)
{
    uint32_t pressStart = 0;
    uint32_t lastBlink = 0;
    uint8_t ledState = 0;
    uint8_t pressActive = 0;
    uint8_t longPress = 0;
    uint8_t ledBeforeLongPress = 0;

    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    while (1)
    {
        GPIO_PinState button = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
        uint32_t now = HAL_GetTick();

        /* Start timing after a debounced button press. */
        if ((button == GPIO_PIN_SET) && (pressActive == 0U))
        {
            HAL_Delay(DEBOUNCE_MS);

            if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET)
            {
                pressActive = 1U;
                longPress = 0U;
                pressStart = HAL_GetTick();
                ledBeforeLongPress = ledState;
            }
        }

        if (pressActive != 0U)
        {
            now = HAL_GetTick();

            /* Once the press reaches 1 second, treat it as a long press. */
            if (((now - pressStart) >= LONG_PRESS_MS) && (longPress == 0U))
            {
                longPress = 1U;
                lastBlink = now;
            }

            /* Long press action: blink the LED while the button is held. */
            if (longPress != 0U)
            {
                if ((now - lastBlink) >= BLINK_INTERVAL)
                {
                    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
                    lastBlink = now;
                }
            }

            /* Button released: decide whether it was short or long. */
            if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
            {
                HAL_Delay(DEBOUNCE_MS);

                if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
                {
                    if (longPress == 0U)
                    {
                        /* Short press action: toggle and remember LED state. */
                        ledState = !ledState;
                    }
                    else
                    {
                        /* End long-press blinking and restore prior LED state. */
                        ledState = ledBeforeLongPress;
                    }

                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,
                                      ledState ? GPIO_PIN_SET : GPIO_PIN_RESET);

                    pressActive = 0U;
                }
            }
        }

        HAL_Delay(5);
    }
}


/**
  * @brief System Clock Configuration
  * @note  NUCLEO-F410RB: 8 MHz HSE bypass input -> 100 MHz SYSCLK.
  *        Confirm the board's HSE/MCO configuration in STM32CubeMX.
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
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |
                                  RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 |
                                  RCC_CLOCKTYPE_PCLK2;
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
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    /* B1 user button: PC13. Verify active level for the exact board revision. */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* LD2 user LED: PA5, active HIGH. */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
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
