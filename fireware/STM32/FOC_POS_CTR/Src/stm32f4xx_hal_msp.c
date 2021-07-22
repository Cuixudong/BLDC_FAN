/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32f4xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
/**
* Initializes the Global MSP.
*/
void HAL_MspInit(void)
{
    /* USER CODE BEGIN MspInit 0 */

    /* USER CODE END MspInit 0 */

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_3);

    /* System interrupt init*/

    /* USER CODE BEGIN MspInit 1 */

    /* USER CODE END MspInit 1 */
}

/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(hadc->Instance==ADC1)
    {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PA0-WKUP     ------> ADC1_IN0
        PA6     ------> ADC1_IN6
        PA3     ------> ADC1_IN3
        PB1     ------> ADC1_IN9
        PB0     ------> ADC1_IN8
        */
        GPIO_InitStruct.Pin = M1_TEMPERATURE_Pin|M1_CURR_AMPL_V_Pin|M1_CURR_AMPL_W_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = M1_BUS_VOLTAGE_Pin|M1_CURR_AMPL_U_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }
    else if(hadc->Instance==ADC2)
    {
        /* USER CODE BEGIN ADC2_MspInit 0 */

        /* USER CODE END ADC2_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ADC2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ADC2 GPIO Configuration
        PA6     ------> ADC2_IN6
        PA3     ------> ADC2_IN3
        PB0     ------> ADC2_IN8
        */
        GPIO_InitStruct.Pin = M1_CURR_AMPL_V_Pin|M1_CURR_AMPL_W_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = M1_CURR_AMPL_U_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(M1_CURR_AMPL_U_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN ADC2_MspInit 1 */

        /* USER CODE END ADC2_MspInit 1 */
    }

}

/**
* @brief ADC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance==ADC1)
    {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC1_CLK_DISABLE();

        /**ADC1 GPIO Configuration
        PA0-WKUP     ------> ADC1_IN0
        PA6     ------> ADC1_IN6
        PA3     ------> ADC1_IN3
        PB1     ------> ADC1_IN9
        PB0     ------> ADC1_IN8
        */
        HAL_GPIO_DeInit(GPIOA, M1_TEMPERATURE_Pin|M1_CURR_AMPL_V_Pin|M1_CURR_AMPL_W_Pin);

        HAL_GPIO_DeInit(GPIOB, M1_BUS_VOLTAGE_Pin|M1_CURR_AMPL_U_Pin);

        /* ADC1 interrupt DeInit */
        /* USER CODE BEGIN ADC1:ADC_IRQn disable */
        /**
        * Uncomment the line below to disable the "ADC_IRQn" interrupt
        * Be aware, disabling shared interrupt may affect other IPs
        */
        /* HAL_NVIC_DisableIRQ(ADC_IRQn); */
        /* USER CODE END ADC1:ADC_IRQn disable */

        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
    else if(hadc->Instance==ADC2)
    {
        /* USER CODE BEGIN ADC2_MspDeInit 0 */

        /* USER CODE END ADC2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC2_CLK_DISABLE();

        /**ADC2 GPIO Configuration
        PA6     ------> ADC2_IN6
        PA3     ------> ADC2_IN3
        PB0     ------> ADC2_IN8
        */
        HAL_GPIO_DeInit(GPIOA, M1_CURR_AMPL_V_Pin|M1_CURR_AMPL_W_Pin);

        HAL_GPIO_DeInit(M1_CURR_AMPL_U_GPIO_Port, M1_CURR_AMPL_U_Pin);

        /* ADC2 interrupt DeInit */
        /* USER CODE BEGIN ADC2:ADC_IRQn disable */
        /**
        * Uncomment the line below to disable the "ADC_IRQn" interrupt
        * Be aware, disabling shared interrupt may affect other IPs
        */
        /* HAL_NVIC_DisableIRQ(ADC_IRQn); */
        /* USER CODE END ADC2:ADC_IRQn disable */

        /* USER CODE BEGIN ADC2_MspDeInit 1 */

        /* USER CODE END ADC2_MspDeInit 1 */
    }

}

/**
* @brief DAC MSP Initialization
* This function configures the hardware resources used in this example
* @param hdac: DAC handle pointer
* @retval None
*/
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(hdac->Instance==DAC)
    {
        /* USER CODE BEGIN DAC_MspInit 0 */

        /* USER CODE END DAC_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_DAC_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**DAC GPIO Configuration
        PA4     ------> DAC_OUT1
        PA5     ------> DAC_OUT2
        */
        GPIO_InitStruct.Pin = DBG_DAC_CH1_Pin|DBG_DAC_CH2_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN DAC_MspInit 1 */

        /* USER CODE END DAC_MspInit 1 */
    }

}

/**
* @brief DAC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hdac: DAC handle pointer
* @retval None
*/
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{
    if(hdac->Instance==DAC)
    {
        /* USER CODE BEGIN DAC_MspDeInit 0 */

        /* USER CODE END DAC_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_DAC_CLK_DISABLE();

        /**DAC GPIO Configuration
        PA4     ------> DAC_OUT1
        PA5     ------> DAC_OUT2
        */
        HAL_GPIO_DeInit(GPIOA, DBG_DAC_CH1_Pin|DBG_DAC_CH2_Pin);

        /* USER CODE BEGIN DAC_MspDeInit 1 */

        /* USER CODE END DAC_MspDeInit 1 */
    }

}

/**
* @brief SPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(hspi->Instance==SPI2)
    {
        /* USER CODE BEGIN SPI2_MspInit 0 */

        /* USER CODE END SPI2_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_SPI2_CLK_ENABLE();

        __HAL_RCC_GPIOI_CLK_ENABLE();
        /**SPI2 GPIO Configuration
        PI3     ------> SPI2_MOSI
        PI2     ------> SPI2_MISO
        PI1     ------> SPI2_SCK
        */
        GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

        /* USER CODE BEGIN SPI2_MspInit 1 */

        /* USER CODE END SPI2_MspInit 1 */
    }

}

/**
* @brief SPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
    if(hspi->Instance==SPI2)
    {
        /* USER CODE BEGIN SPI2_MspDeInit 0 */

        /* USER CODE END SPI2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI2_CLK_DISABLE();

        /**SPI2 GPIO Configuration
        PI3     ------> SPI2_MOSI
        PI2     ------> SPI2_MISO
        PI1     ------> SPI2_SCK
        */
        HAL_GPIO_DeInit(GPIOI, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1);

        /* USER CODE BEGIN SPI2_MspDeInit 1 */

        /* USER CODE END SPI2_MspDeInit 1 */
    }

}

/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(htim_base->Instance==TIM1)
    {
        /* USER CODE BEGIN TIM1_MspInit 0 */

        /* USER CODE END TIM1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM1_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**TIM1 GPIO Configuration
        PB12     ------> TIM1_BKIN
        */
        GPIO_InitStruct.Pin = M1_OCP_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        HAL_GPIO_Init(M1_OCP_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM1_MspInit 1 */

        /* USER CODE END TIM1_MspInit 1 */
    }

}

/**
* @brief TIM_Encoder MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_encoder: TIM_Encoder handle pointer
* @retval None
*/
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_encoder)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(htim_encoder->Instance==TIM3)
    {
        /* USER CODE BEGIN TIM3_MspInit 0 */

        /* USER CODE END TIM3_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM3_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**TIM3 GPIO Configuration
        PC7     ------> TIM3_CH2
        PC6     ------> TIM3_CH1
        */
        GPIO_InitStruct.Pin = M1_ENCODER_B_Pin|M1_ENCODER_A_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM3_MspInit 1 */

        /* USER CODE END TIM3_MspInit 1 */
    }

}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(htim->Instance==TIM1)
    {
        /* USER CODE BEGIN TIM1_MspPostInit 0 */

        /* USER CODE END TIM1_MspPostInit 0 */

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**TIM1 GPIO Configuration
        PA10     ------> TIM1_CH3
        PA9     ------> TIM1_CH2
        PA8     ------> TIM1_CH1
        PB13     ------> TIM1_CH1N
        PB14     ------> TIM1_CH2N
        PB15     ------> TIM1_CH3N
        */
        GPIO_InitStruct.Pin = M1_PWM_WH_Pin|M1_PWM_VH_Pin|M1_PWM_UH_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = M1_PWM_UL_Pin|M1_PWM_VL_Pin|M1_PWM_WL_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM1_MspPostInit 1 */

        /* USER CODE END TIM1_MspPostInit 1 */
    }

}
/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance==TIM1)
    {
        /* USER CODE BEGIN TIM1_MspDeInit 0 */

        /* USER CODE END TIM1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM1_CLK_DISABLE();

        /**TIM1 GPIO Configuration
        PA10     ------> TIM1_CH3
        PA9     ------> TIM1_CH2
        PA8     ------> TIM1_CH1
        PB12     ------> TIM1_BKIN
        PB13     ------> TIM1_CH1N
        PB14     ------> TIM1_CH2N
        PB15     ------> TIM1_CH3N
        */
        HAL_GPIO_DeInit(GPIOA, M1_PWM_WH_Pin|M1_PWM_VH_Pin|M1_PWM_UH_Pin);

        HAL_GPIO_DeInit(GPIOB, M1_OCP_Pin|M1_PWM_UL_Pin|M1_PWM_VL_Pin|M1_PWM_WL_Pin);

        /* TIM1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM1_BRK_TIM9_IRQn);
        HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
        /* USER CODE BEGIN TIM1_MspDeInit 1 */

        /* USER CODE END TIM1_MspDeInit 1 */
    }

}

/**
* @brief TIM_Encoder MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_encoder: TIM_Encoder handle pointer
* @retval None
*/
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_encoder)
{
    if(htim_encoder->Instance==TIM3)
    {
        /* USER CODE BEGIN TIM3_MspDeInit 0 */

        /* USER CODE END TIM3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM3_CLK_DISABLE();

        /**TIM3 GPIO Configuration
        PC7     ------> TIM3_CH2
        PC6     ------> TIM3_CH1
        */
        HAL_GPIO_DeInit(GPIOC, M1_ENCODER_B_Pin|M1_ENCODER_A_Pin);

        /* TIM3 interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM3_IRQn);
        /* USER CODE BEGIN TIM3_MspDeInit 1 */

        /* USER CODE END TIM3_MspDeInit 1 */
    }

}

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(huart->Instance==UART5)
    {
        /* USER CODE BEGIN UART5_MspInit 0 */

        /* USER CODE END UART5_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_UART5_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**UART5 GPIO Configuration
        PC12     ------> UART5_TX
        PD2     ------> UART5_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* UART5 interrupt Init */
        HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(UART5_IRQn);
        /* USER CODE BEGIN UART5_MspInit 1 */

        /* USER CODE END UART5_MspInit 1 */
    }
    else if(huart->Instance==USART1)
    {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**USART1 GPIO Configuration
        PB7     ------> USART1_RX
        PB6     ------> USART1_TX
        */
        GPIO_InitStruct.Pin = UART_RX_Pin|UART_TX_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    }
    else if(huart->Instance==USART3)
    {
        /* USER CODE BEGIN USART3_MspInit 0 */

        /* USER CODE END USART3_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**USART3 GPIO Configuration
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USART3 interrupt Init */
        HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
        /* USER CODE BEGIN USART3_MspInit 1 */

        /* USER CODE END USART3_MspInit 1 */
    }

}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if(huart->Instance==UART5)
    {
        /* USER CODE BEGIN UART5_MspDeInit 0 */

        /* USER CODE END UART5_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_UART5_CLK_DISABLE();

        /**UART5 GPIO Configuration
        PC12     ------> UART5_TX
        PD2     ------> UART5_RX
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);

        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

        /* UART5 interrupt DeInit */
        HAL_NVIC_DisableIRQ(UART5_IRQn);
        /* USER CODE BEGIN UART5_MspDeInit 1 */

        /* USER CODE END UART5_MspDeInit 1 */
    }
    else if(huart->Instance==USART1)
    {
        /* USER CODE BEGIN USART1_MspDeInit 0 */

        /* USER CODE END USART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
        PB7     ------> USART1_RX
        PB6     ------> USART1_TX
        */
        HAL_GPIO_DeInit(GPIOB, UART_RX_Pin|UART_TX_Pin);

        /* USART1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
        /* USER CODE BEGIN USART1_MspDeInit 1 */

        /* USER CODE END USART1_MspDeInit 1 */
    }
    else if(huart->Instance==USART3)
    {
        /* USER CODE BEGIN USART3_MspDeInit 0 */

        /* USER CODE END USART3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART3_CLK_DISABLE();

        /**USART3 GPIO Configuration
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

        /* USART3 interrupt DeInit */
        HAL_NVIC_DisableIRQ(USART3_IRQn);
        /* USER CODE BEGIN USART3_MspDeInit 1 */

        /* USER CODE END USART3_MspDeInit 1 */
    }

}

static uint32_t FSMC_Initialized = 0;

static void HAL_FSMC_MspInit(void) {
    /* USER CODE BEGIN FSMC_MspInit 0 */

    /* USER CODE END FSMC_MspInit 0 */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (FSMC_Initialized) {
        return;
    }
    FSMC_Initialized = 1;

    /* Peripheral clock enable */
    __HAL_RCC_FSMC_CLK_ENABLE();

    /** FSMC GPIO Configuration
    PG12   ------> FSMC_NE4
    PD0   ------> FSMC_D2
    PD5   ------> FSMC_NWE
    PD1   ------> FSMC_D3
    PD4   ------> FSMC_NOE
    PD15   ------> FSMC_D1
    PD14   ------> FSMC_D0
    PG0   ------> FSMC_A10
    PE13   ------> FSMC_D10
    PD10   ------> FSMC_D15
    PE8   ------> FSMC_D5
    PE9   ------> FSMC_D6
    PE11   ------> FSMC_D8
    PE14   ------> FSMC_D11
    PD9   ------> FSMC_D14
    PD8   ------> FSMC_D13
    PE7   ------> FSMC_D4
    PE10   ------> FSMC_D7
    PE12   ------> FSMC_D9
    PE15   ------> FSMC_D12
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_1|GPIO_PIN_4
                          |GPIO_PIN_15|GPIO_PIN_14|GPIO_PIN_10|GPIO_PIN_9
                          |GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11
                          |GPIO_PIN_14|GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_12
                          |GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* USER CODE BEGIN FSMC_MspInit 1 */

    /* USER CODE END FSMC_MspInit 1 */
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* hsram) {
    /* USER CODE BEGIN SRAM_MspInit 0 */

    /* USER CODE END SRAM_MspInit 0 */
    HAL_FSMC_MspInit();
    /* USER CODE BEGIN SRAM_MspInit 1 */

    /* USER CODE END SRAM_MspInit 1 */
}

static uint32_t FSMC_DeInitialized = 0;

static void HAL_FSMC_MspDeInit(void) {
    /* USER CODE BEGIN FSMC_MspDeInit 0 */

    /* USER CODE END FSMC_MspDeInit 0 */
    if (FSMC_DeInitialized) {
        return;
    }
    FSMC_DeInitialized = 1;
    /* Peripheral clock enable */
    __HAL_RCC_FSMC_CLK_DISABLE();

    /** FSMC GPIO Configuration
    PG12   ------> FSMC_NE4
    PD0   ------> FSMC_D2
    PD5   ------> FSMC_NWE
    PD1   ------> FSMC_D3
    PD4   ------> FSMC_NOE
    PD15   ------> FSMC_D1
    PD14   ------> FSMC_D0
    PG0   ------> FSMC_A10
    PE13   ------> FSMC_D10
    PD10   ------> FSMC_D15
    PE8   ------> FSMC_D5
    PE9   ------> FSMC_D6
    PE11   ------> FSMC_D8
    PE14   ------> FSMC_D11
    PD9   ------> FSMC_D14
    PD8   ------> FSMC_D13
    PE7   ------> FSMC_D4
    PE10   ------> FSMC_D7
    PE12   ------> FSMC_D9
    PE15   ------> FSMC_D12
    */
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_12|GPIO_PIN_0);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_1|GPIO_PIN_4
                    |GPIO_PIN_15|GPIO_PIN_14|GPIO_PIN_10|GPIO_PIN_9
                    |GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_13|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11
                    |GPIO_PIN_14|GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_12
                    |GPIO_PIN_15);

    /* USER CODE BEGIN FSMC_MspDeInit 1 */

    /* USER CODE END FSMC_MspDeInit 1 */
}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* hsram) {
    /* USER CODE BEGIN SRAM_MspDeInit 0 */

    /* USER CODE END SRAM_MspDeInit 0 */
    HAL_FSMC_MspDeInit();
    /* USER CODE BEGIN SRAM_MspDeInit 1 */

    /* USER CODE END SRAM_MspDeInit 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
