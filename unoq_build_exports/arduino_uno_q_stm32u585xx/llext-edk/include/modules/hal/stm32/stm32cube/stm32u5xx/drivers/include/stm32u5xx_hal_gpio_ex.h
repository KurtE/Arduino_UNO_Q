/**
  ******************************************************************************
  * @file    stm32u5xx_hal_gpio_ex.h
  * @author  MCD Application Team
  * @brief   Header file of GPIO HAL Extended module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32U5xx_HAL_GPIO_EX_H
#define STM32U5xx_HAL_GPIO_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal_def.h"

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

/** @defgroup GPIOEx GPIOEx
  * @brief GPIO Extended HAL module driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup LPGPIO_MapTypeDef GPIO/Ex Exported Types
  * @{
  */
typedef struct
{
  GPIO_TypeDef *GPIO_PORT;
  uint32_t Pin_Pos;
} LPGPIO_MapTypeDef;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup GPIOEx_Exported_Constants GPIOEx Exported Constants
  * @{
  */

/** @defgroup GPIOEx_Alternate_function_selection GPIOEx Alternate function selection
  * @{
  */

/**
  * @brief   AF 0 selection
  */
#define GPIO_AF0_RTC_50HZ      ((uint8_t)0x00)
#define GPIO_AF0_MCO           ((uint8_t)0x00)
#define GPIO_AF0_SWJ           ((uint8_t)0x00)
#define GPIO_AF0_TRACE         ((uint8_t)0x00)
#define GPIO_AF0_LPTIM1        ((uint8_t)0x00)
#define GPIO_AF0_CSLEEP        ((uint8_t)0x00)
#define GPIO_AF0_CSTOP         ((uint8_t)0x00)
#define GPIO_AF0_CRS           ((uint8_t)0x00)
#define GPIO_AF0_SRDSTOP       ((uint8_t)0x00)

/**
  * @brief   AF 1 selection
  */
#define GPIO_AF1_TIM1          ((uint8_t)0x01)
#define GPIO_AF1_TIM2          ((uint8_t)0x01)
#define GPIO_AF1_TIM5          ((uint8_t)0x01)
#define GPIO_AF1_TIM8          ((uint8_t)0x01)
#define GPIO_AF1_LPTIM1        ((uint8_t)0x01)
#define GPIO_AF1_IR            ((uint8_t)0x01)

/**
  * @brief   AF 2 selection
  */
#define GPIO_AF2_TIM1          ((uint8_t)0x02)
#define GPIO_AF2_TIM2          ((uint8_t)0x02)
#define GPIO_AF2_TIM3          ((uint8_t)0x02)
#define GPIO_AF2_TIM4          ((uint8_t)0x02)
#define GPIO_AF2_TIM5          ((uint8_t)0x02)
#define GPIO_AF2_LPTIM1        ((uint8_t)0x02)
#define GPIO_AF2_LPTIM2        ((uint8_t)0x02)
#define GPIO_AF2_LPTIM3        ((uint8_t)0x02)
#if defined(I2C5)
#define GPIO_AF2_I2C5          ((uint8_t)0x02)
#endif /* I2C5 */
#if defined(I2C6)
#define GPIO_AF2_I2C6          ((uint8_t)0x02)
#endif /* I2C6 */
#if defined(GFXTIM)
#define GPIO_AF2_GFXTIM        ((uint8_t)0x02)
#endif /* GFXTIM */

/**
  * @brief   AF 3 selection
  */
#define GPIO_AF3_I2C4          ((uint8_t)0x03)
#define GPIO_AF3_OCTOSPI1      ((uint8_t)0x03)
#define GPIO_AF3_SAI1          ((uint8_t)0x03)
#define GPIO_AF3_SPI2          ((uint8_t)0x03)
#define GPIO_AF3_TIM1          ((uint8_t)0x03)
#define GPIO_AF3_TIM8          ((uint8_t)0x03)
#define GPIO_AF3_TIM8_COMP1    ((uint8_t)0x03)
#define GPIO_AF3_TIM8_COMP2    ((uint8_t)0x03)
#define GPIO_AF3_TIM1_COMP1    ((uint8_t)0x03)
#define GPIO_AF3_TIM1_COMP2    ((uint8_t)0x03)
#if defined(USART2)
#define GPIO_AF3_USART2        ((uint8_t)0x03)
#endif /* USART2 */
#define GPIO_AF3_ADF1          ((uint8_t)0x03)
#if defined(USB_OTG_HS)
#define GPIO_AF3_USB_HS        ((uint8_t)0x03)
#endif /* USB_OTG_HS */

/**
  * @brief   AF 4 selection
  */
#define GPIO_AF4_I2C1          ((uint8_t)0x04)
#define GPIO_AF4_I2C2          ((uint8_t)0x04)
#define GPIO_AF4_I2C3          ((uint8_t)0x04)
#define GPIO_AF4_I2C4          ((uint8_t)0x04)
#define GPIO_AF4_PSSI          ((uint8_t)0x04)
#define GPIO_AF4_DCMI          ((uint8_t)0x04)
#define GPIO_AF4_LPTIM3        ((uint8_t)0x04)
#if defined (I2C5)
#define GPIO_AF4_I2C5          ((uint8_t)0x04)
#endif /* I2C5 */

/**
  * @brief   AF 5 selection
  */
#define GPIO_AF5_I2C4          ((uint8_t)0x05)
#define GPIO_AF5_OCTOSPI1      ((uint8_t)0x05)
#if defined(OCTOSPI2)
#define GPIO_AF5_OCTOSPI2      ((uint8_t)0x05)
#endif /* OCTOSPI2 */
#define GPIO_AF5_SPI1          ((uint8_t)0x05)
#define GPIO_AF5_SPI2          ((uint8_t)0x05)
#define GPIO_AF5_SPI3          ((uint8_t)0x05)
#define GPIO_AF5_DCMI          ((uint8_t)0x05)
#define GPIO_AF5_MDF1          ((uint8_t)0x05)
#define GPIO_AF5_PSSI          ((uint8_t)0x05)
#if defined(GFXTIM)
#define GPIO_AF5_GFXTIM        ((uint8_t)0x05)
#endif /* GFXTIM */

/**
  * @brief   AF 6 selection
  */
#define GPIO_AF6_OCTOSPI1      ((uint8_t)0x06)
#if defined(OCTOSPI2)
#define GPIO_AF6_OCTOSPI2      ((uint8_t)0x06)
#endif /* OCTOPSI2 */
#define GPIO_AF6_MDF1          ((uint8_t)0x06)
#define GPIO_AF6_SPI3          ((uint8_t)0x06)
#define GPIO_AF6_I2C3          ((uint8_t)0x06)

/**
  * @brief   AF 7 selection
  */
#define GPIO_AF7_USART1        ((uint8_t)0x07)
#if defined(USART2)
#define GPIO_AF7_USART2        ((uint8_t)0x07)
#endif /* USART2 */
#define GPIO_AF7_USART3        ((uint8_t)0x07)
#if defined(USART6)
#define GPIO_AF7_USART6        ((uint8_t)0x07)
#endif /* USART6 */
#if defined(LTDC)
#define GPIO_AF7_LTDC          ((uint8_t)0x07)
#endif /* LTDC */

/**
  * @brief   AF 8 selection
  */
#define GPIO_AF8_LPUART1       ((uint8_t)0x08)
#define GPIO_AF8_UART4         ((uint8_t)0x08)
#define GPIO_AF8_UART5         ((uint8_t)0x08)
#define GPIO_AF8_SDMMC1        ((uint8_t)0x08)
#if defined(SDMMC2)
#define GPIO_AF8_SDMMC2        ((uint8_t)0x08)
#endif /* SDMMC2 */
#if defined(LTDC)
#define GPIO_AF8_LTDC          ((uint8_t)0x08)
#endif /* LTDC */
#if defined(HSPI1)
#define GPIO_AF8_HSPI1         ((uint8_t)0x08)
#endif /* HSPI1 */

/**
  * @brief   AF 9 selection
  */
#define GPIO_AF9_FDCAN1        ((uint8_t)0x09)
#define GPIO_AF9_TSC           ((uint8_t)0x09)

/**
  * @brief   AF 10 selection
  */
#define GPIO_AF10_DCMI         ((uint8_t)0x0A)
#define GPIO_AF10_PSSI         ((uint8_t)0x0A)
#define GPIO_AF10_USB          ((uint8_t)0x0A)
#define GPIO_AF10_OCTOSPI1     ((uint8_t)0x0A)
#if defined(OCTOSPI2)
#define GPIO_AF10_OCTOSPI2     ((uint8_t)0x0A)
#endif /* OCTOSPI2 */
#define GPIO_AF10_CRS          ((uint8_t)0x0A)
#if defined(USB_OTG_HS)
#define GPIO_AF10_USB_HS       ((uint8_t)0x0A)
#endif /* USB_OTG_HS */
#if defined(DSI)
#define GPIO_AF10_DSI          ((uint8_t)0x0A)
#endif /*  DSI */
#if defined(GFXTIM)
#define GPIO_AF10_GFXTIM       ((uint8_t)0x0A)
#endif /* GFXTIM */

/**
  * @brief   AF 11 selection
  */
#if defined(UCPD1)
#define GPIO_AF11_UCPD1        ((uint8_t)0x0B)
#endif /* UCPD1 */
#if defined(SDMMC2)
#define GPIO_AF11_SDMMC2       ((uint8_t)0x0B)
#endif /* SDMMC2 */
#define GPIO_AF11_LPGPIO1      ((uint8_t)0x0B)
#if defined(FMC_BASE)
#define GPIO_AF11_FMC          ((uint8_t)0x0B)
#endif /* FMC_BASE */
#if defined(DSI)
#define GPIO_AF11_DSI          ((uint8_t)0x0B)
#endif /*  DSI */
#if defined(GFXTIM)
#define GPIO_AF11_GFXTIM       ((uint8_t)0x0B)
#endif /* GFXTIM */

/**
  * @brief   AF 12 selection
  */
#define GPIO_AF12_COMP1        ((uint8_t)0x0C)
#define GPIO_AF12_COMP2        ((uint8_t)0x0C)
#if defined(FMC_BASE)
#define GPIO_AF12_FMC          ((uint8_t)0x0C)
#endif /* FMC_BASE */
#define GPIO_AF12_TIM1_COMP1   ((uint8_t)0x0C)
#define GPIO_AF12_TIM1_COMP2   ((uint8_t)0x0C)
#define GPIO_AF12_TIM8_COMP2   ((uint8_t)0x0C)
#define GPIO_AF12_SDMMC1       ((uint8_t)0x0C)
#if defined(SDMMC2)
#define GPIO_AF12_SDMMC2       ((uint8_t)0x0C)
#endif /* SDMMC2 */

/**
  * @brief   AF 13 selection
  */
#define GPIO_AF13_SAI1         ((uint8_t)0x0D)
#if defined(SAI2)
#define GPIO_AF13_SAI2         ((uint8_t)0x0D)
#endif /* SAI2 */
#define GPIO_AF13_LPTIM4       ((uint8_t)0x0D)
#define GPIO_AF13_LPTIM2       ((uint8_t)0x0D)
#if defined(GFXTIM)
#define GPIO_AF13_GFXTIM       ((uint8_t)0x0D)
#endif /* GFXTIM */

/**
  * @brief   AF 14 selection
  */
#define GPIO_AF14_LPTIM2       ((uint8_t)0x0E)
#define GPIO_AF14_LPTIM3       ((uint8_t)0x0E)
#define GPIO_AF14_TIM2         ((uint8_t)0x0E)
#define GPIO_AF14_TIM15        ((uint8_t)0x0E)
#define GPIO_AF14_TIM15_COMP1  ((uint8_t)0x0E)
#define GPIO_AF14_TIM16        ((uint8_t)0x0E)
#define GPIO_AF14_TIM16_COMP1  ((uint8_t)0x0E)
#define GPIO_AF14_TIM17        ((uint8_t)0x0E)
#define GPIO_AF14_TIM17_COMP1  ((uint8_t)0x0E)
#if defined(FMC_BASE)
#define GPIO_AF14_FMC          ((uint8_t)0x0E)
#endif /* FMC_BASE */

/**
  * @brief   AF 15 selection
  */
#define GPIO_AF15_EVENTOUT     ((uint8_t)0x0F)

#define IS_GPIO_AF(AF)   ((AF) <= (uint8_t)0x0F)

/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup GPIOEx_Exported_Macros GPIOEx Exported Macros
  * @{
  */

/** @defgroup GPIOEx_Get_Port_Index GPIOEx Get Port Index
  * @{
  */

/* GPIO_Peripheral_Memory_Mapping Peripheral Memory Mapping */
#define GPIO_GET_INDEX(__GPIOx__)           (((uint32_t )(__GPIOx__) & (~GPIOA_BASE)) >> 10)

/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32U5xx_HAL_GPIO_EX_H */
