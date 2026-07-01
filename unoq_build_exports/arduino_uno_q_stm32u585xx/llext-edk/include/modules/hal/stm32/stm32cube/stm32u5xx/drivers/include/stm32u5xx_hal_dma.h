/**
  **********************************************************************************************************************
  * @file    stm32u5xx_hal_dma.h
  * @author  MCD Application Team
  * @brief   Header file of DMA HAL module.
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef STM32U5xx_HAL_DMA_H
#define STM32U5xx_HAL_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32u5xx_hal_def.h"

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

/** @addtogroup DMA
  * @{
  */


/* Exported types ----------------------------------------------------------------------------------------------------*/

/** @defgroup DMA_Exported_Types DMA Exported Types
  * @brief    DMA Exported Types
  * @{
  */

/**
  * @brief DMA Transfer Configuration Structure definition.
  */
typedef struct
{
  uint32_t Request;               /*!< Specifies the DMA channel request.
                                       This parameter can be a value of @ref DMA_Request_Selection                    */

  uint32_t BlkHWRequest;          /*!< Specifies the Block hardware request mode for DMA channel.
                                       Block Hardware request feature can be used only with dedicated peripherals.
                                       This parameter can be a value of @ref DMA_Block_Request                        */

  uint32_t Direction;             /*!< Specifies the transfer direction for DMA channel.
                                       This parameter can be a value of @ref DMA_Transfer_Direction                   */

  uint32_t SrcInc;                /*!< Specifies the source increment mode for the DMA channel.
                                       This parameter can be a value of @ref DMA_Source_Increment_Mode                */

  uint32_t DestInc;               /*!< Specifies the destination increment mode for the DMA channel.
                                       This parameter can be a value of @ref DMA_Destination_Increment_Mode           */

  uint32_t SrcDataWidth;          /*!< Specifies the source data width for the DMA channel.
                                       This parameter can be a value of @ref DMA_Source_Data_Width                    */

  uint32_t DestDataWidth;         /*!< Specifies the destination data width for the DMA channel.
                                       This parameter can be a value of @ref DMA_Destination_Data_Width               */

  uint32_t Priority;              /*!< Specifies the priority level for the DMA channel.
                                       This parameter can be a value of @ref DMA_Priority_Level                       */

  uint32_t SrcBurstLength;        /*!< Specifies the source burst length (number of beats within a burst) for the DMA
                                       channel.
                                       This parameter can be a value between 1 and 64                                 */

  uint32_t DestBurstLength;       /*!< Specifies the destination burst length (number of beats within a burst) for the
                                       DMA channel.
                                       This parameter can be a value between 1 and 64                                 */

  uint32_t TransferAllocatedPort; /*!< Specifies the transfer allocated ports.
                                       This parameter can be a combination of @ref DMA_Transfer_Allocated_Port        */

  uint32_t TransferEventMode;     /*!< Specifies the transfer event mode for the DMA channel.
                                       This parameter can be a value of @ref DMA_Transfer_Event_Mode                  */

  uint32_t Mode;                  /*!< Specifies the transfer mode for the DMA channel.
                                       This parameter can be a value of @ref DMA_Transfer_Mode                        */

} DMA_InitTypeDef;

/**
  * @brief DMA Linked-List Configuration Structure Definition.
  */
typedef struct
{
  uint32_t Priority;          /*!< Specifies the priority level for the DMA channel.
                                   This parameter can be a value of @ref DMA_Priority_Level        */

  uint32_t LinkStepMode;      /*!< Specifies the link step mode for the DMA channel.
                                   This parameter can be a value of @ref DMAEx_Link_Step_Mode      */

  uint32_t LinkAllocatedPort; /*!< Specifies the linked-list allocated port for the DMA channel.
                                   This parameter can be a value of @ref DMAEx_Link_Allocated_Port */

  uint32_t TransferEventMode; /*!< Specifies the transfer event mode for the DMA channel.
                                   This parameter can be a value of @ref DMA_Transfer_Event_Mode   */

  uint32_t LinkedListMode;    /*!< Specifies linked-list transfer mode for the DMA channel.
                                   This parameter can be a value of @ref DMAEx_LinkedList_Mode     */

} DMA_InitLinkedListTypeDef;

/**
  * @brief  HAL DMA State Enumeration Definition.
  */
typedef enum
{
  HAL_DMA_STATE_RESET   = 0x00U,
  HAL_DMA_STATE_READY   = 0x01U,
  HAL_DMA_STATE_BUSY    = 0x02U,
  HAL_DMA_STATE_ERROR   = 0x03U,
  HAL_DMA_STATE_ABORT   = 0x04U,
  HAL_DMA_STATE_SUSPEND = 0x05U,

} HAL_DMA_StateTypeDef;

/**
  * @brief  HAL DMA Level Complete Enumeration Definition.
  */
typedef enum
{
  HAL_DMA_FULL_TRANSFER = 0x00U,
  HAL_DMA_HALF_TRANSFER = 0x01U,

} HAL_DMA_LevelCompleteTypeDef;

/**
  * @brief  HAL DMA Callbacks IDs Enumeration Definition.
  */
typedef enum
{
  HAL_DMA_XFER_CPLT_CB_ID     = 0x00U,
  HAL_DMA_XFER_HALFCPLT_CB_ID = 0x01U,
  HAL_DMA_XFER_ERROR_CB_ID    = 0x02U,
  HAL_DMA_XFER_ABORT_CB_ID    = 0x03U,
  HAL_DMA_XFER_SUSPEND_CB_ID  = 0x04U,
  HAL_DMA_XFER_ALL_CB_ID      = 0x05U

} HAL_DMA_CallbackIDTypeDef;

/**
  * @brief  DMA handle Structure definition
  */
typedef struct __DMA_HandleTypeDef
{
  DMA_Channel_TypeDef       *Instance;

  DMA_InitTypeDef           Init;

  DMA_InitLinkedListTypeDef InitLinkedList;

  HAL_LockTypeDef           Lock;

  uint32_t                  Mode;

  __IO HAL_DMA_StateTypeDef State;

  __IO uint32_t             ErrorCode;

  void                      *Parent;

  void (* XferCpltCallback)(struct __DMA_HandleTypeDef *hdma);

  void (* XferHalfCpltCallback)(struct __DMA_HandleTypeDef *hdma);

  void (* XferErrorCallback)(struct __DMA_HandleTypeDef *hdma);

  void (* XferAbortCallback)(struct __DMA_HandleTypeDef *hdma);

  void (* XferSuspendCallback)(struct __DMA_HandleTypeDef *hdma);

  struct __DMA_QListTypeDef  *LinkedListQueue;

} DMA_HandleTypeDef;
/**
  * @}
  */


/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup DMA_Exported_Constants DMA Exported Constants
  * @brief    DMA Exported constants
  * @{
  */

/** @defgroup DMA_Error_Codes DMA Error Codes
  * @brief    DMA Error Codes
  * @{
  */
#define HAL_DMA_ERROR_NONE             (0x0000U)
#define HAL_DMA_ERROR_DTE              (0x0001U)
#define HAL_DMA_ERROR_ULE              (0x0002U)
#define HAL_DMA_ERROR_USE              (0x0004U)
#define HAL_DMA_ERROR_TO               (0x0008U)
#define HAL_DMA_ERROR_TIMEOUT          (0x0010U)
#define HAL_DMA_ERROR_NO_XFER          (0x0020U)
#define HAL_DMA_ERROR_BUSY             (0x0040U)
#define HAL_DMA_ERROR_INVALID_CALLBACK (0x0080U)
#define HAL_DMA_ERROR_NOT_SUPPORTED    (0x0100U)
/**
  * @}
  */

/** @defgroup DMA_Interrupt_Enable_Definition DMA Interrupt Enable Definition
  * @brief    DMA Interrupt Enable Definition
  * @{
  */
#define DMA_IT_TC   DMA_CCR_TCIE
#define DMA_IT_HT   DMA_CCR_HTIE
#define DMA_IT_DTE  DMA_CCR_DTEIE
#define DMA_IT_ULE  DMA_CCR_ULEIE
#define DMA_IT_USE  DMA_CCR_USEIE
#define DMA_IT_SUSP DMA_CCR_SUSPIE
#define DMA_IT_TO   DMA_CCR_TOIE
/**
  * @}
  */

/** @defgroup DMA_Flag_Definition DMA Flag Definition
  * @brief    DMA Flag Definition
  * @{
  */
#define DMA_FLAG_IDLE DMA_CSR_IDLEF
#define DMA_FLAG_TC   DMA_CSR_TCF
#define DMA_FLAG_HT   DMA_CSR_HTF
#define DMA_FLAG_DTE  DMA_CSR_DTEF
#define DMA_FLAG_ULE  DMA_CSR_ULEF
#define DMA_FLAG_USE  DMA_CSR_USEF
#define DMA_FLAG_SUSP DMA_CSR_SUSPF
#define DMA_FLAG_TO   DMA_CSR_TOF
/**
  * @}
  */

/** @defgroup DMA_Request_Selection DMA Request Selection
  * @brief    DMA Request Selection
  * @{
  */
/* GPDMA1 requests */
#define GPDMA1_REQUEST_ADC1          0U
#define GPDMA1_REQUEST_ADC4          1U
#define GPDMA1_REQUEST_DAC1_CH1      2U
#define GPDMA1_REQUEST_DAC1_CH2      3U
#define GPDMA1_REQUEST_TIM6_UP       4U
#define GPDMA1_REQUEST_TIM7_UP       5U
#define GPDMA1_REQUEST_SPI1_RX       6U
#define GPDMA1_REQUEST_SPI1_TX       7U
#define GPDMA1_REQUEST_SPI2_RX       8U
#define GPDMA1_REQUEST_SPI2_TX       9U
#define GPDMA1_REQUEST_SPI3_RX       10U
#define GPDMA1_REQUEST_SPI3_TX       11U
#define GPDMA1_REQUEST_I2C1_RX       12U
#define GPDMA1_REQUEST_I2C1_TX       13U
#define GPDMA1_REQUEST_I2C1_EVC      14U
#define GPDMA1_REQUEST_I2C2_RX       15U
#define GPDMA1_REQUEST_I2C2_TX       16U
#define GPDMA1_REQUEST_I2C2_EVC      17U
#define GPDMA1_REQUEST_I2C3_RX       18U
#define GPDMA1_REQUEST_I2C3_TX       19U
#define GPDMA1_REQUEST_I2C3_EVC      20U
#define GPDMA1_REQUEST_I2C4_RX       21U
#define GPDMA1_REQUEST_I2C4_TX       22U
#define GPDMA1_REQUEST_I2C4_EVC      23U
#define GPDMA1_REQUEST_USART1_RX     24U
#define GPDMA1_REQUEST_USART1_TX     25U
#if defined(USART2)
#define GPDMA1_REQUEST_USART2_RX     26U
#define GPDMA1_REQUEST_USART2_TX     27U
#endif /* USART2 */
#define GPDMA1_REQUEST_USART3_RX     28U
#define GPDMA1_REQUEST_USART3_TX     29U
#define GPDMA1_REQUEST_UART4_RX      30U
#define GPDMA1_REQUEST_UART4_TX      31U
#define GPDMA1_REQUEST_UART5_RX      32U
#define GPDMA1_REQUEST_UART5_TX      33U
#define GPDMA1_REQUEST_LPUART1_RX    34U
#define GPDMA1_REQUEST_LPUART1_TX    35U
#define GPDMA1_REQUEST_SAI1_A        36U
#define GPDMA1_REQUEST_SAI1_B        37U
#if defined(SAI2)
#define GPDMA1_REQUEST_SAI2_A        38U
#define GPDMA1_REQUEST_SAI2_B        39U
#endif /* SAI2 */
#define GPDMA1_REQUEST_OCTOSPI1      40U
#if defined(OCTOSPI2)
#define GPDMA1_REQUEST_OCTOSPI2      41U
#endif /* OCTOSPI2 */
#define GPDMA1_REQUEST_TIM1_CH1      42U
#define GPDMA1_REQUEST_TIM1_CH2      43U
#define GPDMA1_REQUEST_TIM1_CH3      44U
#define GPDMA1_REQUEST_TIM1_CH4      45U
#define GPDMA1_REQUEST_TIM1_UP       46U
#define GPDMA1_REQUEST_TIM1_TRIG     47U
#define GPDMA1_REQUEST_TIM1_COM      48U
#define GPDMA1_REQUEST_TIM8_CH1      49U
#define GPDMA1_REQUEST_TIM8_CH2      50U
#define GPDMA1_REQUEST_TIM8_CH3      51U
#define GPDMA1_REQUEST_TIM8_CH4      52U
#define GPDMA1_REQUEST_TIM8_UP       53U
#define GPDMA1_REQUEST_TIM8_TRIG     54U
#define GPDMA1_REQUEST_TIM8_COM      55U
#define GPDMA1_REQUEST_TIM2_CH1      56U
#define GPDMA1_REQUEST_TIM2_CH2      57U
#define GPDMA1_REQUEST_TIM2_CH3      58U
#define GPDMA1_REQUEST_TIM2_CH4      59U
#define GPDMA1_REQUEST_TIM2_UP       60U
#define GPDMA1_REQUEST_TIM3_CH1      61U
#define GPDMA1_REQUEST_TIM3_CH2      62U
#define GPDMA1_REQUEST_TIM3_CH3      63U
#define GPDMA1_REQUEST_TIM3_CH4      64U
#define GPDMA1_REQUEST_TIM3_UP       65U
#define GPDMA1_REQUEST_TIM3_TRIG     66U
#define GPDMA1_REQUEST_TIM4_CH1      67U
#define GPDMA1_REQUEST_TIM4_CH2      68U
#define GPDMA1_REQUEST_TIM4_CH3      69U
#define GPDMA1_REQUEST_TIM4_CH4      70U
#define GPDMA1_REQUEST_TIM4_UP       71U
#define GPDMA1_REQUEST_TIM5_CH1      72U
#define GPDMA1_REQUEST_TIM5_CH2      73U
#define GPDMA1_REQUEST_TIM5_CH3      74U
#define GPDMA1_REQUEST_TIM5_CH4      75U
#define GPDMA1_REQUEST_TIM5_UP       76U
#define GPDMA1_REQUEST_TIM5_TRIG     77U
#define GPDMA1_REQUEST_TIM15_CH1     78U
#define GPDMA1_REQUEST_TIM15_UP      79U
#define GPDMA1_REQUEST_TIM15_TRIG    80U
#define GPDMA1_REQUEST_TIM15_COM     81U
#define GPDMA1_REQUEST_TIM16_CH1     82U
#define GPDMA1_REQUEST_TIM16_UP      83U
#define GPDMA1_REQUEST_TIM17_CH1     84U
#define GPDMA1_REQUEST_TIM17_UP      85U
#define GPDMA1_REQUEST_DCMI_PSSI     86U
#define GPDMA1_REQUEST_AES_IN        87U
#define GPDMA1_REQUEST_AES_OUT       88U
#define GPDMA1_REQUEST_HASH_IN       89U
#if defined(UCPD1)
#define GPDMA1_REQUEST_UCPD1_TX      90U
#define GPDMA1_REQUEST_UCPD1_RX      91U
#endif /* UCPD1 */
#define GPDMA1_REQUEST_MDF1_FLT0     92U
#define GPDMA1_REQUEST_MDF1_FLT1     93U
#define GPDMA1_REQUEST_MDF1_FLT2     94U
#define GPDMA1_REQUEST_MDF1_FLT3     95U
#define GPDMA1_REQUEST_MDF1_FLT4     96U
#define GPDMA1_REQUEST_MDF1_FLT5     97U
#define GPDMA1_REQUEST_ADF1_FLT0     98U
#define GPDMA1_REQUEST_FMAC_READ     99U
#define GPDMA1_REQUEST_FMAC_WRITE    100U
#define GPDMA1_REQUEST_CORDIC_READ   101U
#define GPDMA1_REQUEST_CORDIC_WRITE  102U
#define GPDMA1_REQUEST_SAES_IN       103U
#define GPDMA1_REQUEST_SAES_OUT      104U
#define GPDMA1_REQUEST_LPTIM1_IC1    105U
#define GPDMA1_REQUEST_LPTIM1_IC2    106U
#define GPDMA1_REQUEST_LPTIM1_UE     107U
#define GPDMA1_REQUEST_LPTIM2_IC1    108U
#define GPDMA1_REQUEST_LPTIM2_IC2    109U
#define GPDMA1_REQUEST_LPTIM2_UE     110U
#define GPDMA1_REQUEST_LPTIM3_IC1    111U
#define GPDMA1_REQUEST_LPTIM3_IC2    112U
#define GPDMA1_REQUEST_LPTIM3_UE     113U
#if defined (HSPI1_BASE)
#define GPDMA1_REQUEST_HSPI1         114U
#endif /* HSPI1_BASE */
#if defined (I2C5)
#define GPDMA1_REQUEST_I2C5_RX       115U
#define GPDMA1_REQUEST_I2C5_TX       116U
#define GPDMA1_REQUEST_I2C5_EVC      117U
#endif /* I2C5 */
#if defined (I2C6)
#define GPDMA1_REQUEST_I2C6_RX       118U
#define GPDMA1_REQUEST_I2C6_TX       119U
#define GPDMA1_REQUEST_I2C6_EVC      120U
#endif /* I2C6 */
#if defined (USART6)
#define GPDMA1_REQUEST_USART6_RX     121U
#define GPDMA1_REQUEST_USART6_TX     122U
#endif /* USART6 */
#if defined (ADC2)
#define GPDMA1_REQUEST_ADC2          123U
#endif /* ADC2 */
#if defined (JPEG)
#define GPDMA1_REQUEST_JPEG_RX       124U
#define GPDMA1_REQUEST_JPEG_TX       125U
#endif /* JPEG */

/* LPDMA1 requests */
#define LPDMA1_REQUEST_LPUART1_RX    0U
#define LPDMA1_REQUEST_LPUART1_TX    1U
#define LPDMA1_REQUEST_SPI3_RX       2U
#define LPDMA1_REQUEST_SPI3_TX       3U
#define LPDMA1_REQUEST_I2C3_RX       4U
#define LPDMA1_REQUEST_I2C3_TX       5U
#define LPDMA1_REQUEST_I2C3_EVC      6U
#define LPDMA1_REQUEST_ADC4          7U
#define LPDMA1_REQUEST_DAC1_CH1      8U
#define LPDMA1_REQUEST_DAC1_CH2      9U
#define LPDMA1_REQUEST_ADF1_FLT0     10U
#define LPDMA1_REQUEST_LPTIM1_IC1    11U
#define LPDMA1_REQUEST_LPTIM1_IC2    12U
#define LPDMA1_REQUEST_LPTIM1_UE     13U
#define LPDMA1_REQUEST_LPTIM3_IC1    14U
#define LPDMA1_REQUEST_LPTIM3_IC2    15U
#define LPDMA1_REQUEST_LPTIM3_UE     16U

/* Software request */
#define DMA_REQUEST_SW              DMA_CTR2_SWREQ
/**
  * @}
  */

/** @defgroup DMA_Block_Request DMA Block Request
  * @brief    DMA Block Request
  * @{
  */
#define DMA_BREQ_SINGLE_BURST          0x00000000U
#define DMA_BREQ_BLOCK                 DMA_CTR2_BREQ
/**
  * @}
  */

/** @defgroup DMA_Transfer_Direction DMA Transfer Direction
  * @brief    DMA transfer direction
  * @{
  */
#define DMA_PERIPH_TO_MEMORY 0x00000000U
#define DMA_MEMORY_TO_PERIPH DMA_CTR2_DREQ
#define DMA_MEMORY_TO_MEMORY DMA_CTR2_SWREQ
/**
  * @}
  */

/** @defgroup DMA_Source_Increment_Mode DMA Source Increment Mode
  * @brief    DMA Source Increment Mode
  * @{
  */
#define DMA_SINC_FIXED       0x00000000U
#define DMA_SINC_INCREMENTED DMA_CTR1_SINC
/**
  * @}
  */

/** @defgroup DMA_Destination_Increment_Mode DMA Destination Increment Mode
  * @brief    DMA Destination Increment Mode
  * @{
  */
#define DMA_DINC_FIXED       0x00000000U
#define DMA_DINC_INCREMENTED DMA_CTR1_DINC
/**
  * @}
  */

/** @defgroup DMA_Source_Data_Width DMA Source Data Width
  * @brief    DMA Source Data Width
  * @{
  */
#define DMA_SRC_DATAWIDTH_BYTE       0x00000000U
#define DMA_SRC_DATAWIDTH_HALFWORD   DMA_CTR1_SDW_LOG2_0
#define DMA_SRC_DATAWIDTH_WORD       DMA_CTR1_SDW_LOG2_1
/**
  * @}
  */

/** @defgroup DMA_Destination_Data_Width DMA destination Data Width
  * @brief    DMA destination Data Width
  * @{
  */
#define DMA_DEST_DATAWIDTH_BYTE       0x00000000U
#define DMA_DEST_DATAWIDTH_HALFWORD   DMA_CTR1_DDW_LOG2_0
#define DMA_DEST_DATAWIDTH_WORD       DMA_CTR1_DDW_LOG2_1

/**
  * @}
  */

/** @defgroup DMA_Priority_Level DMA Priority Level
  * @brief    DMA Priority Level
  * @{
  */
#define DMA_LOW_PRIORITY_LOW_WEIGHT  0x00000000U
#define DMA_LOW_PRIORITY_MID_WEIGHT  DMA_CCR_PRIO_0
#define DMA_LOW_PRIORITY_HIGH_WEIGHT DMA_CCR_PRIO_1
#define DMA_HIGH_PRIORITY            DMA_CCR_PRIO
/**
  * @}
  */

/** @defgroup DMA_Transfer_Allocated_Port DMA Transfer Allocated Port
  * @brief    DMA Transfer Allocated Port
  * @{
  */
#define DMA_SRC_ALLOCATED_PORT0  0x00000000U
#define DMA_SRC_ALLOCATED_PORT1  DMA_CTR1_SAP
#define DMA_DEST_ALLOCATED_PORT0 0x00000000U
#define DMA_DEST_ALLOCATED_PORT1 DMA_CTR1_DAP
/**
  * @}
  */

/** @defgroup DMA_Transfer_Event_Mode DMA Transfer Event Mode
  * @brief    DMA Transfer Event Mode
  * @{
  */
#define DMA_TCEM_BLOCK_TRANSFER          0x00000000U     /*!< The TC event is generated at the end of each block and the
                                                              HT event is generated at the half of each block         */
#define DMA_TCEM_REPEATED_BLOCK_TRANSFER DMA_CTR2_TCEM_0 /*!< The TC event is generated at the end of the repeated block
                                                              and the HT event is generated at the half of the repeated
                                                              block                                                   */
#define DMA_TCEM_EACH_LL_ITEM_TRANSFER   DMA_CTR2_TCEM_1 /*!< The TC event is generated at the end of each linked-list
                                                              item and the HT event is generated at the half of each
                                                              linked-list item                                        */
#define DMA_TCEM_LAST_LL_ITEM_TRANSFER   DMA_CTR2_TCEM   /*!< The TC event is generated at the end of the last
                                                              linked-list item and the HT event is generated at the half
                                                              of the last linked-list item                            */
/**
  * @}
  */

/** @defgroup DMA_Transfer_Mode DMA Transfer Mode
  * @brief    DMA Transfer Mode
  * @{
  */
#define DMA_NORMAL                           (0x00U)
/**
  * @}
  */

/** @defgroup DMA_Channel_Attributes DMA Channel Attributes
  * @brief    DMA Channel Security and Privilege Attributes
  * @note     Secure and non-secure attributes are only available from the secure world when TZEN = 1
  * @{
  */
#define DMA_CHANNEL_PRIV      (DMA_CHANNEL_ATTR_PRIV_MASK | 0x01U)
#define DMA_CHANNEL_NPRIV     (DMA_CHANNEL_ATTR_PRIV_MASK)

#define DMA_CHANNEL_SEC       (DMA_CHANNEL_ATTR_SEC_MASK | 0x02U)
#define DMA_CHANNEL_NSEC      (DMA_CHANNEL_ATTR_SEC_MASK)
#define DMA_CHANNEL_SRC_SEC   (DMA_CHANNEL_ATTR_SEC_SRC_MASK | 0x04U)
#define DMA_CHANNEL_SRC_NSEC  (DMA_CHANNEL_ATTR_SEC_SRC_MASK)
#define DMA_CHANNEL_DEST_SEC  (DMA_CHANNEL_ATTR_SEC_DEST_MASK | 0x08U)
#define DMA_CHANNEL_DEST_NSEC (DMA_CHANNEL_ATTR_SEC_DEST_MASK)

#define DMA_CHANNEL_ATTRIBUTE_UNLOCKED (0x00U)
#define DMA_CHANNEL_ATTRIBUTE_LOCKED   (0x01U)
/**
  * @}
  */


/**
  * @}
  */


/* Exported macro ----------------------------------------------------------------------------------------------------*/
/** @defgroup DMA_Exported_Macros DMA Exported Macros
  * @brief    DMA Exported Macros
  * @{
  */

/** @brief  Reset DMA handle state.
  * @param  __HANDLE__ : DMA handle.
  * @retval None.
  */
#define __HAL_DMA_RESET_HANDLE_STATE(__HANDLE__) \
  ((__HANDLE__)->State = HAL_DMA_STATE_RESET)

/**
  * @brief  Enable the specified DMA Channel.
  * @param  __HANDLE__ : DMA handle.
  * @retval None
  */
#define __HAL_DMA_ENABLE(__HANDLE__) \
  ((__HANDLE__)->Instance->CCR |=  DMA_CCR_EN)

/**
  * @brief  Disable the specified DMA Channel.
  * @param  __HANDLE__ : DMA handle.
  * @retval None
  */
#define __HAL_DMA_DISABLE(__HANDLE__) \
  ((__HANDLE__)->Instance->CCR |= (DMA_CCR_SUSP | DMA_CCR_RESET))

/**
  * @brief  Get the DMA channel pending flags.
  * @param  __HANDLE__ : DMA handle.
  * @param  __FLAG__   : Get the specified flag.
  *          This parameter can be any combination of the following values:
  *            @arg DMA_FLAG_TC    : Transfer Complete flag.
  *            @arg DMA_FLAG_HT    : Half Transfer Complete flag.
  *            @arg DMA_FLAG_DTE   : Data Transfer Error flag.
  *            @arg DMA_FLAG_ULE   : Update linked-list Error flag.
  *            @arg DMA_FLAG_USE   : User Setting Error flag.
  *            @arg DMA_FLAG_TO    : Trigger Overrun flag.
  *            @arg DMA_FLAG_SUSP  : Completed Suspension flag.
  *            @arg DMA_FLAG_IDLEF : Idle flag.
  * @retval The state of FLAG (SET or RESET).
  */
#define __HAL_DMA_GET_FLAG(__HANDLE__, __FLAG__) \
  ((__HANDLE__)->Instance->CSR & (__FLAG__))

/**
  * @brief  Clear the DMA Channel pending flags.
  * @param  __HANDLE__ : DMA handle.
  * @param  __FLAG__   : Specifies the flag to clear.
  *          This parameter can be any combination of the following values:
  *            @arg DMA_FLAG_TC   : Transfer Complete flag.
  *            @arg DMA_FLAG_HT   : Half Transfer Complete flag.
  *            @arg DMA_FLAG_DTE  : Data Transfer Error flag.
  *            @arg DMA_FLAG_ULE  : Update Linked-List Error flag.
  *            @arg DMA_FLAG_USE  : User Setting Error flag.
  *            @arg DMA_FLAG_TO   : Trigger Overrun flag.
  *            @arg DMA_FLAG_SUSP : Completed Suspension flag.
  * @retval None
  */
#define __HAL_DMA_CLEAR_FLAG(__HANDLE__, __FLAG__) \
  ((__HANDLE__)->Instance->CFCR = (__FLAG__))

/**
  * @brief  Enable the specified DMA Channel interrupts.
  * @param  __HANDLE__    : DMA handle.
  * @param  __INTERRUPT__ : Specifies the DMA interrupt sources to be enabled.
  *          This parameter can be any combination of the following values:
  *            @arg DMA_IT_TC   : Transfer Complete interrupt.
  *            @arg DMA_IT_HT   : Half Transfer Complete interrupt.
  *            @arg DMA_IT_DTE  : Data Transfer Error interrupt.
  *            @arg DMA_IT_ULE  : Update Linked-List Error interrupt.
  *            @arg DMA_IT_USE  : User Setting Error interrupt.
  *            @arg DMA_IT_TO   : Trigger Overrun interrupt.
  *            @arg DMA_IT_SUSP : Completed Suspension interrupt.
  * @retval None
  */
#define __HAL_DMA_ENABLE_IT(__HANDLE__, __INTERRUPT__) \
  ((__HANDLE__)->Instance->CCR |= (__INTERRUPT__))

/**
  * @brief  Disable the specified DMA Channel interrupts.
  * @param  __HANDLE__   : DMA handle.
  * @param __INTERRUPT__ : specifies the DMA interrupt sources to be disabled.
  *          This parameter can be any combination of the following values:
  *            @arg DMA_IT_TC   : Transfer Complete interrupt.
  *            @arg DMA_IT_HT   : Half Transfer Complete interrupt.
  *            @arg DMA_IT_DTE  : Data Transfer Error interrupt.
  *            @arg DMA_IT_ULE  : Update Linked-List Error interrupt.
  *            @arg DMA_IT_USE  : User Setting Error interrupt.
  *            @arg DMA_IT_TO   : Trigger Overrun interrupt.
  *            @arg DMA_IT_SUSP : Completed Suspension interrupt.
  * @retval None
  */
#define __HAL_DMA_DISABLE_IT(__HANDLE__, __INTERRUPT__) \
  ((__HANDLE__)->Instance->CCR &= ~(__INTERRUPT__))

/**
  * @brief  Checks whether the specified DMA Channel interrupt is enabled or not.
  * @param  __HANDLE__    : DMA handle.
  * @param  __INTERRUPT__ : specifies the DMA interrupt source to check.
  *            @arg DMA_IT_TC   : Transfer Complete interrupt.
  *            @arg DMA_IT_HT   : Half Transfer Complete interrupt.
  *            @arg DMA_IT_DTE  : Data Transfer Error interrupt.
  *            @arg DMA_IT_ULE  : Update Linked-List Error interrupt.
  *            @arg DMA_IT_USE  : User Setting Error interrupt.
  *            @arg DMA_IT_TO   : Trigger Overrun interrupt.
  *            @arg DMA_IT_SUSP : Completed Suspension interrupt.
  * @retval The state of DMA_IT (SET or RESET).
  */
#define __HAL_DMA_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) \
  (((__HANDLE__)->Instance->CCR & (__INTERRUPT__)))

/**
  * @brief  Writes the block number of bytes to be transferred from the source on the DMA Channel.
  * @param  __HANDLE__  : DMA handle.
  * @param  __COUNTER__ : Number of data bytes to be transferred from the source (from 0 to 65535).
  */
#define __HAL_DMA_SET_COUNTER(__HANDLE__, __COUNTER__) \
  MODIFY_REG((__HANDLE__)->Instance->CBR1, DMA_CBR1_BNDT, (__COUNTER__))

/**
  * @brief  Returns the number of remaining data bytes in the current DMA Channel transfer.
  * @param  __HANDLE__ : DMA handle.
  * @retval The number of remaining data units in the current DMA Stream transfer.
  */
#define __HAL_DMA_GET_COUNTER(__HANDLE__) \
  (((__HANDLE__)->Instance->CBR1) & DMA_CBR1_BNDT)
/**
  * @}
  */


/* Include DMA HAL Extension module */
#include "stm32u5xx_hal_dma_ex.h"


/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @defgroup DMA_Exported_Functions DMA Exported Functions
  * @brief    DMA Exported Functions
  * @{
  */

/** @defgroup DMA_Exported_Functions_Group1 Initialization and De-Initialization Functions
  * @brief    Initialization and De-Initialization Functions
  * @{
  */
HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *const hdma);
HAL_StatusTypeDef HAL_DMA_DeInit(DMA_HandleTypeDef *const hdma);
/**
  * @}
  */

/** @defgroup DMA_Exported_Functions_Group2 I/O Operation Functions
  * @brief    I/O Operation Functions
  * @{
  */
HAL_StatusTypeDef HAL_DMA_Start(DMA_HandleTypeDef *const hdma,
                                uint32_t SrcAddress,
                                uint32_t DstAddress,
                                uint32_t SrcDataSize);
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *const hdma,
                                   uint32_t SrcAddress,
                                   uint32_t DstAddress,
                                   uint32_t SrcDataSize);
HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *const hdma);
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *const hdma);
HAL_StatusTypeDef HAL_DMA_PollForTransfer(DMA_HandleTypeDef *const hdma,
                                          HAL_DMA_LevelCompleteTypeDef CompleteLevel,
                                          uint32_t Timeout);
void HAL_DMA_IRQHandler(DMA_HandleTypeDef *const hdma);
HAL_StatusTypeDef HAL_DMA_RegisterCallback(DMA_HandleTypeDef *const hdma,
                                           HAL_DMA_CallbackIDTypeDef CallbackID,
                                           void (*const  pCallback)(DMA_HandleTypeDef *const _hdma));
HAL_StatusTypeDef HAL_DMA_UnRegisterCallback(DMA_HandleTypeDef *const hdma,
                                             HAL_DMA_CallbackIDTypeDef CallbackID);
/**
  * @}
  */

/** @defgroup DMA_Exported_Functions_Group3 State and Error Functions
  * @brief    State and Error Functions
  * @{
  */
HAL_DMA_StateTypeDef HAL_DMA_GetState(DMA_HandleTypeDef const *const hdma);
uint32_t             HAL_DMA_GetError(DMA_HandleTypeDef const *const hdma);
/**
  * @}
  */

/** @defgroup DMA_Exported_Functions_Group4 DMA Attributes Functions
  * @brief    DMA Attributes Functions
  * @{
  */

HAL_StatusTypeDef HAL_DMA_ConfigChannelAttributes(DMA_HandleTypeDef *const hdma,
                                                  uint32_t ChannelAttributes);
HAL_StatusTypeDef HAL_DMA_GetConfigChannelAttributes(DMA_HandleTypeDef const *const hdma,
                                                     uint32_t *const pChannelAttributes);

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
HAL_StatusTypeDef HAL_DMA_LockChannelAttributes(DMA_HandleTypeDef const *const hdma);
#endif /* (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */
HAL_StatusTypeDef HAL_DMA_GetLockChannelAttributes(DMA_HandleTypeDef const *const hdma,
                                                   uint32_t *const pLockState);


/**
  * @}
  */

/**
  * @}
  */


/* Private constants -------------------------------------------------------------------------------------------------*/
/** @defgroup DMA_Private_Constants DMA Private Constants
  * @brief    DMA Private Constants
  * @{
  */
#define HAL_TIMEOUT_DMA_ABORT          (0x00000005U)
#define HAL_DMA_CHANNEL_START          (0x00000050U)
#define HAL_DMA_CHANNEL_SIZE           (0x00000080U)
#define HAL_DMA_OFFSET_MASK            (0x00000FFFU)
#define DMA_CHANNEL_ATTR_PRIV_MASK     (0x00000010U)
#define DMA_CHANNEL_ATTR_SEC_MASK      (0x00000020U)
#define DMA_CHANNEL_ATTR_SEC_SRC_MASK  (0x00000040U)
#define DMA_CHANNEL_ATTR_SEC_DEST_MASK (0x00000080U)
#define DMA_CHANNEL_ATTR_VALUE_MASK    (0x0000000FU)
#define DMA_CHANNEL_ATTR_ITEM_MASK     (0x000000F0U)
#define DMA_CHANNEL_BURST_MIN          (0x00000001U)
#define DMA_CHANNEL_BURST_MAX          (0x00000040U)
/**
  * @}
  */


/* Private macros ----------------------------------------------------------------------------------------------------*/
/** @defgroup DMA_Private_Macros DMA Private Macros
  * @brief    DMA Private Macros
  * @{
  */
#define GET_DMA_INSTANCE(__HANDLE__) \
  ((DMA_TypeDef *)((uint32_t)((__HANDLE__)->Instance) & (~HAL_DMA_OFFSET_MASK)))

#define GET_DMA_CHANNEL(__HANDLE__) \
  ((((uint32_t)((__HANDLE__)->Instance) & HAL_DMA_OFFSET_MASK) - HAL_DMA_CHANNEL_START) / HAL_DMA_CHANNEL_SIZE)

#define IS_DMA_MODE(MODE) \
  ((MODE) == DMA_NORMAL)

#define IS_DMA_DIRECTION(DIRECTION)         \
  (((DIRECTION) == DMA_PERIPH_TO_MEMORY) || \
   ((DIRECTION) == DMA_MEMORY_TO_PERIPH) || \
   ((DIRECTION) == DMA_MEMORY_TO_MEMORY))

#define IS_DMA_LEVEL_COMPLETE(LEVEL)     \
  (((LEVEL) == HAL_DMA_FULL_TRANSFER) || \
   ((LEVEL) == HAL_DMA_HALF_TRANSFER))

#define IS_DMA_SOURCE_INC(INC)  \
  (((INC) == DMA_SINC_FIXED) || \
   ((INC) == DMA_SINC_INCREMENTED))

#define IS_DMA_DESTINATION_INC(INC) \
  (((INC) == DMA_DINC_FIXED)     || \
   ((INC) == DMA_DINC_INCREMENTED))

#define IS_DMA_SOURCE_DATA_WIDTH(WIDTH)       \
  (((WIDTH) == DMA_SRC_DATAWIDTH_BYTE)     || \
   ((WIDTH) == DMA_SRC_DATAWIDTH_HALFWORD) || \
   ((WIDTH) == DMA_SRC_DATAWIDTH_WORD))

#define IS_DMA_DESTINATION_DATA_WIDTH(WIDTH)   \
  (((WIDTH) == DMA_DEST_DATAWIDTH_BYTE)     || \
   ((WIDTH) == DMA_DEST_DATAWIDTH_HALFWORD) || \
   ((WIDTH) == DMA_DEST_DATAWIDTH_WORD))

#define IS_DMA_BURST_LENGTH(LENGTH)       \
  (((LENGTH) >= DMA_CHANNEL_BURST_MIN) && \
   ((LENGTH) <= DMA_CHANNEL_BURST_MAX))

#define IS_DMA_PRIORITY(PRIORITY)                  \
  (((PRIORITY) == DMA_LOW_PRIORITY_LOW_WEIGHT)  || \
   ((PRIORITY) == DMA_LOW_PRIORITY_MID_WEIGHT)  || \
   ((PRIORITY) == DMA_LOW_PRIORITY_HIGH_WEIGHT) || \
   ((PRIORITY) == DMA_HIGH_PRIORITY))

#define  IS_DMA_TRANSFER_ALLOCATED_PORT(ALLOCATED_PORT) \
  (((ALLOCATED_PORT) & (~(DMA_CTR1_SAP | DMA_CTR1_DAP))) == 0U)

#if defined (GPDMA1_REQUEST_JPEG_TX)
#define IS_DMA_REQUEST(REQUEST)        (((REQUEST) == DMA_REQUEST_SW) || ((REQUEST) <= GPDMA1_REQUEST_JPEG_TX))
#elif defined (GPDMA1_REQUEST_ADC2)
#define IS_DMA_REQUEST(REQUEST)        (((REQUEST) == DMA_REQUEST_SW) || ((REQUEST) <= GPDMA1_REQUEST_ADC2))
#else
#define IS_DMA_REQUEST(REQUEST)        (((REQUEST) == DMA_REQUEST_SW) || ((REQUEST) <= GPDMA1_REQUEST_LPTIM3_UE))
#endif /* GPDMA1_REQUEST_JPEG_TX */

#define IS_DMA_BLOCK_HW_REQUEST(MODE)   \
  (((MODE) == DMA_BREQ_SINGLE_BURST) || \
   ((MODE) == DMA_BREQ_BLOCK))

#define IS_DMA_TCEM_EVENT_MODE(MODE)               \
  (((MODE) == DMA_TCEM_BLOCK_TRANSFER)          || \
   ((MODE) == DMA_TCEM_REPEATED_BLOCK_TRANSFER) || \
   ((MODE) == DMA_TCEM_EACH_LL_ITEM_TRANSFER)   || \
   ((MODE) == DMA_TCEM_LAST_LL_ITEM_TRANSFER))

#define IS_DMA_BLOCK_SIZE(SIZE) \
  (((SIZE) > 0U) && ((SIZE) <= DMA_CBR1_BNDT))

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define IS_DMA_ATTRIBUTES(ATTRIBUTE)                                                                               \
  (((ATTRIBUTE) != 0U) && (((ATTRIBUTE) & (~(DMA_CHANNEL_ATTR_VALUE_MASK | DMA_CHANNEL_ATTR_ITEM_MASK))) == 0U) && \
   (((((ATTRIBUTE) & DMA_CHANNEL_ATTR_ITEM_MASK) >> 4U) | ((ATTRIBUTE) & DMA_CHANNEL_ATTR_VALUE_MASK)) ==          \
    (((ATTRIBUTE) & DMA_CHANNEL_ATTR_ITEM_MASK) >> 4U)))
#else
#define IS_DMA_ATTRIBUTES(ATTRIBUTE)    \
  (((ATTRIBUTE) == DMA_CHANNEL_PRIV) || \
   ((ATTRIBUTE) == DMA_CHANNEL_NPRIV))
#endif /* (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define IS_DMA_GLOBAL_ACTIVE_FLAG_S(INSTANCE, GLOBAL_FLAG) \
  (((INSTANCE)->SMISR & (GLOBAL_FLAG)))
#endif /* (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */
#define IS_DMA_GLOBAL_ACTIVE_FLAG_NS(INSTANCE, GLOBAL_FLAG) \
  (((INSTANCE)->MISR & (GLOBAL_FLAG)))

/**
  * @}
  */


/* Private functions -------------------------------------------------------------------------------------------------*/
/** @defgroup DMA_Private_Functions DMA Private Functions
  * @brief    DMA Private Functions
  * @{
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32U5xx_HAL_DMA_H */
