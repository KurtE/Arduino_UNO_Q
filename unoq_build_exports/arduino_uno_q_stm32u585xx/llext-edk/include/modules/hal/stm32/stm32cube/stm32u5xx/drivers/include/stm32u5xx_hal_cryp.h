/**
  ******************************************************************************
  * @file    stm32u5xx_hal_cryp.h
  * @author  MCD Application Team
  * @brief   Header file of CRYP HAL module.
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
#ifndef STM32U5xx_HAL_CRYP_H
#define STM32U5xx_HAL_CRYP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal_def.h"

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

#if defined(AES)

/** @defgroup CRYP CRYP
  * @brief CRYP HAL module driver.
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup CRYP_Exported_Types CRYP Exported Types
  * @{
  */

/**
  * @brief CRYP Init Structure definition
  */

typedef struct
{
  uint32_t DataType;                   /*!< 32-bit data, 16-bit data, 8-bit data or 1-bit string.
                                        This parameter can be a value of @ref CRYP_Data_Type */
  uint32_t KeySize;                    /*!< Used only in AES mode : 128, 192 or 256 bit key length in CRYP1.
                                        128 or 256 bit key length in TinyAES This parameter can be a value
                                        of @ref CRYP_Key_Size */
  uint32_t *pKey;
  uint32_t *pInitVect;                 /*!< The initialization vector used also as initialization
                                         counter in CTR mode */
  uint32_t Algorithm;                  /*!<  DES/ TDES Algorithm ECB/CBC
                                        AES Algorithm ECB/CBC/CTR/GCM or CCM
                                        This parameter can be a value of @ref CRYP_Algorithm_Mode */
  uint32_t *Header;                    /*!< used only in AES GCM and CCM Algorithm for authentication,
                                        GCM : also known as Additional Authentication Data
                                        CCM : named B1 composed of the associated data length and Associated Data. */
  uint32_t HeaderSize;
  uint32_t *B0;
  uint32_t DataWidthUnit;
  uint32_t HeaderWidthUnit;
  uint32_t KeyIVConfigSkip;            /*!< CRYP peripheral Key and IV configuration skip, to config Key and Initialization
                                            Vector only once and to skip configuration for consecutive processings.
                                            This parameter can be a value of @ref CRYP_Configuration_Skip */
  uint32_t KeyMode;
  uint32_t KeySelect;
  uint32_t KeyProtection;

} CRYP_ConfigTypeDef;

/**
  * @brief  CRYP State Structure definition
  */

typedef enum
{
  HAL_CRYP_STATE_RESET             = 0x00U,
  HAL_CRYP_STATE_READY             = 0x01U,
  HAL_CRYP_STATE_BUSY              = 0x02U,
#if (USE_HAL_CRYP_SUSPEND_RESUME == 1U)
  HAL_CRYP_STATE_SUSPENDED         = 0x03U,
#endif /* USE_HAL_CRYP_SUSPEND_RESUME */
} HAL_CRYP_STATETypeDef;

/**
  * @brief CRYP Context Structure definition
  */

typedef struct
{
  uint32_t DataType;
  uint32_t KeySize;
  uint32_t *pKey;
  uint32_t *pInitVect;
  uint32_t Algorithm;
  uint32_t DataWidthUnit;
  uint32_t KeyIVConfigSkip;
  uint32_t KeyMode;
  uint32_t Phase;
  uint32_t KeyIVConfig;
  uint32_t CR_Reg;
  uint32_t IER_Reg;
  uint32_t IVR0_Reg;
  uint32_t IVR1_Reg;
  uint32_t IVR2_Reg;
  uint32_t IVR3_Reg;

} CRYP_ContextTypeDef;

#if (USE_HAL_CRYP_SUSPEND_RESUME == 1U)
/**
  * @brief HAL CRYP mode suspend definitions
  */
typedef enum
{
  HAL_CRYP_SUSPEND_NONE            = 0x00U,
  HAL_CRYP_SUSPEND                 = 0x01U
} HAL_SuspendTypeDef;
#endif /* USE_HAL_CRYP_SUSPEND_RESUME */

/**
  * @brief  CRYP handle Structure definition
  */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)
typedef struct __CRYP_HandleTypeDef
#else
typedef struct
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
{
  AES_TypeDef                       *Instance;

  CRYP_ConfigTypeDef                Init;
  uint32_t                          *pCrypInBuffPtr;

  uint32_t                          *pCrypOutBuffPtr;

  __IO uint16_t                     CrypHeaderCount;

  __IO uint16_t                     CrypInCount;

  __IO uint16_t                     CrypOutCount;

  uint16_t                          Size;

  uint32_t                          Phase;

  DMA_HandleTypeDef                 *hdmain;

  DMA_HandleTypeDef                 *hdmaout;

  HAL_LockTypeDef                   Lock;

  __IO  HAL_CRYP_STATETypeDef       State;

  __IO uint32_t                     ErrorCode;

  uint32_t                          KeyIVConfig;      /*!< CRYP peripheral Key and IV configuration flag, used when
                                                           configuration can be skipped */

  uint32_t                          SizesSum;         /*!< Sum of successive payloads lengths (in bytes), stored
                                                           for a single signature computation after several
                                                           messages processing */

#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1U)
  void (*InCpltCallback)(struct __CRYP_HandleTypeDef *hcryp);
  void (*OutCpltCallback)(struct __CRYP_HandleTypeDef *hcryp);
  void (*ErrorCallback)(struct __CRYP_HandleTypeDef *hcryp);

  void (* MspInitCallback)(struct __CRYP_HandleTypeDef *hcryp);
  void (* MspDeInitCallback)(struct __CRYP_HandleTypeDef *hcryp);

#endif /* (USE_HAL_CRYP_REGISTER_CALLBACKS) */

#if (USE_HAL_CRYP_SUSPEND_RESUME == 1U)

  __IO HAL_SuspendTypeDef     SuspendRequest;

  CRYP_ConfigTypeDef          Init_saved;

  uint32_t                    *pCrypInBuffPtr_saved;

  uint32_t                    *pCrypOutBuffPtr_saved;

  uint32_t                    CrypInCount_saved;

  uint32_t                    CrypOutCount_saved;

  uint32_t                    Phase_saved;

  __IO HAL_CRYP_STATETypeDef  State_saved;

  uint32_t                    IV_saved[4];

  uint32_t                    SUSPxR_saved[8];

  uint32_t                    CR_saved;

  uint32_t                    Key_saved[8];

  uint16_t                    Size_saved;

  uint16_t                    CrypHeaderCount_saved;

  uint32_t                    SizesSum_saved;

  uint32_t                    ResumingFlag;
#endif /* USE_HAL_CRYP_SUSPEND_RESUME */

} CRYP_HandleTypeDef;

#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1U)

/**
  * @brief  HAL CRYP Callback ID enumeration definition
  */
typedef enum
{
  HAL_CRYP_MSPINIT_CB_ID           = 0x00U,
  HAL_CRYP_MSPDEINIT_CB_ID         = 0x01U,
  HAL_CRYP_INPUT_COMPLETE_CB_ID    = 0x02U,
  HAL_CRYP_OUTPUT_COMPLETE_CB_ID   = 0x03U,
  HAL_CRYP_ERROR_CB_ID             = 0x04U,
} HAL_CRYP_CallbackIDTypeDef;

/**
  * @brief  HAL CRYP Callback pointer definition
  */
typedef  void (*pCRYP_CallbackTypeDef)(CRYP_HandleTypeDef *hcryp);

#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup CRYP_Exported_Constants CRYP Exported Constants
  * @{
  */

/** @defgroup CRYP_Error_Definition   CRYP Error Definition
  * @{
  */
#define HAL_CRYP_ERROR_NONE              0x00000000U
#define HAL_CRYP_ERROR_WRITE             0x00000001U
#define HAL_CRYP_ERROR_READ              0x00000002U
#define HAL_CRYP_ERROR_DMA               0x00000004U
#define HAL_CRYP_ERROR_BUSY              0x00000008U
#define HAL_CRYP_ERROR_TIMEOUT           0x00000010U
#define HAL_CRYP_ERROR_NOT_SUPPORTED     0x00000020U
#define HAL_CRYP_ERROR_AUTH_TAG_SEQUENCE 0x00000040U
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1U)
#define HAL_CRYP_ERROR_INVALID_CALLBACK  ((uint32_t)0x00000080U)
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
#define HAL_CRYP_ERROR_KEY               0x00000100U
#define HAL_CRYP_ERROR_RNG               0x00000200U
/**
  * @}
  */

/** @defgroup CRYP_Data_Width_Unit CRYP Data Width Unit
  * @{
  */

#define CRYP_DATAWIDTHUNIT_WORD   0x00000000U
#define CRYP_DATAWIDTHUNIT_BYTE   0x00000001U

/**
  * @}
  */

/** @defgroup CRYP_Header_Width_Unit CRYP Header Width Unit
  * @{
  */

#define CRYP_HEADERWIDTHUNIT_WORD   0x00000000U
#define CRYP_HEADERWIDTHUNIT_BYTE   0x00000001U

/**
  * @}
  */

/** @defgroup CRYP_Algorithm_Mode CRYP Algorithm Mode
  * @{
  */

#define CRYP_AES_ECB            0x00000000U
#define CRYP_AES_CBC            AES_CR_CHMOD_0
#define CRYP_AES_CTR            AES_CR_CHMOD_1
#define CRYP_AES_GCM_GMAC       (AES_CR_CHMOD_0 | AES_CR_CHMOD_1)
#define CRYP_AES_CCM            AES_CR_CHMOD_2

/**
  * @}
  */

/** @defgroup CRYP_Key_Size CRYP Key Size
  * @{
  */

#define CRYP_KEYSIZE_128B         0x00000000U
#define CRYP_KEYSIZE_256B         AES_CR_KEYSIZE

/**
  * @}
  */

/** @defgroup CRYP_Key_Mode CRYP Key Mode
  * @{
  */

#define CRYP_KEYMODE_NORMAL         0x00000000U
#define CRYP_KEYMODE_SHARED         AES_CR_KMOD_1
#define CRYP_KEYMODE_WRAPPED        AES_CR_KMOD_0

/**
  * @}
  */

/** @defgroup CRYP_Key_Select CRYP Key Select
  * @{
  */

#define CRYP_KEYSEL_NORMAL       0x00000000U
#define CRYP_KEYSEL_HW           AES_CR_KEYSEL_0
#define CRYP_KEYSEL_SW           AES_CR_KEYSEL_1
#define CRYP_KEYSEL_HSW          AES_CR_KEYSEL_2

/**
  * @}
  */

/** @defgroup CRYP_Key_ShareID CRYP Key Share ID
  * @{
  */

#define CRYP_KSHAREID_AES         0x00000000U

/**
  * @}
  */

/** @defgroup CRYP_Key_Protection CRYP Key Protection
  * @{
  */

#define CRYP_KEYPROT_ENABLE         AES_CR_KEYPROT
#define CRYP_KEYPROT_DISABLE        0x00000000U
/**
  * @}
  */


/** @defgroup CRYP_Data_Type CRYP Data Type
  * @{
  */

#define CRYP_DATATYPE_32B      0x00000000U
#define CRYP_DATATYPE_16B      AES_CR_DATATYPE_0
#define CRYP_DATATYPE_8B       AES_CR_DATATYPE_1
#define CRYP_DATATYPE_1B       AES_CR_DATATYPE

#define CRYP_NO_SWAP           CRYP_DATATYPE_32B
#define CRYP_HALFWORD_SWAP     CRYP_DATATYPE_16B
#define CRYP_BYTE_SWAP         CRYP_DATATYPE_8B
#define CRYP_BIT_SWAP          CRYP_DATATYPE_1B

/**
  * @}
  */

/** @defgroup CRYP_Interrupt  CRYP Interrupt
  * @{
  */
#define CRYP_IT_CCFIE     AES_IER_CCFIE
#define CRYP_IT_RWEIE     AES_IER_RWEIE
#define CRYP_IT_KEIE      AES_IER_KEIE
#define CRYP_IT_RNGEIE    AES_IER_RNGEIE

/**
  * @}
  */

/** @defgroup CRYP_Flags CRYP Flags
  * @{
  */

#define CRYP_FLAG_BUSY       AES_SR_BUSY                   /*!< GCM process suspension forbidden also set when
                                                             transferring a shared key from SAES peripheral  */
#define CRYP_FLAG_WRERR      (AES_SR_WRERR | 0x80000000U)
#define CRYP_FLAG_RDERR      (AES_SR_RDERR | 0x80000000U)
#define CRYP_FLAG_CCF        AES_ISR_CCF
#define CRYP_FLAG_KEYVALID   AES_SR_KEYVALID
#define CRYP_FLAG_KEIF       AES_ISR_KEIF
#define CRYP_FLAG_RWEIF      AES_ISR_RWEIF
#define CRYP_FLAG_RNGEIF     AES_ISR_RNGEIF


/**
  * @}
  */

/** @defgroup CRYP_CLEAR_Flags CRYP CLEAR Flags
  * @{
  */

#define CRYP_CLEAR_CCF      AES_ICR_CCF
#define CRYP_CLEAR_RWEIF    AES_ICR_RWEIF  /*!< Clear Error Flag : RWEIF in AES_ISR and
                                              both RDERR and WRERR flags in AES_SR */
#define CRYP_CLEAR_KEIF     AES_ICR_KEIF
#define CRYP_CLEAR_RNGEIF   AES_ICR_RNGEIF


/**
  * @}
  */

/** @defgroup CRYP_Configuration_Skip CRYP Key and IV Configuration Skip Mode
  * @{
  */

#define CRYP_KEYIVCONFIG_ALWAYS  0x00000000U
#define CRYP_KEYIVCONFIG_ONCE    0x00000001U
#define CRYP_KEYNOCONFIG         0x00000002U
#define CRYP_IVCONFIG_ONCE       0x00000004U

/**
  * @}
  */


/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup CRYP_Exported_Macros CRYP Exported Macros
  * @{
  */

/** @brief Reset CRYP handle state
  * @param  __HANDLE__ specifies the CRYP handle.
  * @retval None
  */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1U)
#define __HAL_CRYP_RESET_HANDLE_STATE(__HANDLE__) do{\
                                                      (__HANDLE__)->State = HAL_CRYP_STATE_RESET;\
                                                      (__HANDLE__)->MspInitCallback = NULL;\
                                                      (__HANDLE__)->MspDeInitCallback = NULL;\
                                                     }while(0U)
#else
#define __HAL_CRYP_RESET_HANDLE_STATE(__HANDLE__) ( (__HANDLE__)->State = HAL_CRYP_STATE_RESET)
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */

/**
  * @brief  Enable/Disable the CRYP peripheral.
  * @param  __HANDLE__ specifies the CRYP handle.
  * @retval None
  */

#define __HAL_CRYP_ENABLE(__HANDLE__)  ((__HANDLE__)->Instance->CR |=  AES_CR_EN)
#define __HAL_CRYP_DISABLE(__HANDLE__) ((__HANDLE__)->Instance->CR &=  ~AES_CR_EN)


/** @brief  Check whether the specified CRYP status flag is set or not.
  * @param  __HANDLE__ specifies the CRYP handle.
  * @param  __FLAG__ specifies the flag to check.
  *         This parameter can be one of the following values for TinyAES:
  *            @arg @ref CRYP_FLAG_KEYVALID Key valid flag
  *            @arg @ref CRYP_FLAG_BUSY GCM process suspension forbidden or
  *                                     transferring a shared key from SAES IP.
  *            @arg @ref CRYP_FLAG_WRERR Write Error flag
  *            @arg @ref CRYP_FLAG_RDERR Read Error flag
  *            @arg @ref CRYP_FLAG_CCF Computation Complete flag
  *            @arg @ref CRYP_FLAG_KEIF Key error flag
  *            @arg @ref CRYP_FLAG_RWEIF Read/write Error flag

  * @retval The state of __FLAG__ (FlagStatus).
  */

#define __HAL_CRYP_GET_FLAG(__HANDLE__, __FLAG__) (\
                                                   ((__FLAG__) == CRYP_FLAG_KEYVALID )?((((__HANDLE__)->Instance->SR \
                                                       & (CRYP_FLAG_KEYVALID)) == (CRYP_FLAG_KEYVALID))?SET:RESET) : \
                                                   ((__FLAG__) == CRYP_FLAG_BUSY )?((((__HANDLE__)->Instance->SR \
                                                       & (CRYP_FLAG_BUSY)) == (CRYP_FLAG_BUSY))?SET:RESET) : \
                                                   ((__FLAG__) == CRYP_FLAG_WRERR )?((((__HANDLE__)->Instance->SR \
                                                       & (CRYP_FLAG_WRERR & 0x7FFFFFFFU)) == \
                                                       (CRYP_FLAG_WRERR & 0x7FFFFFFFU))?SET:RESET) : \
                                                   ((__FLAG__) == CRYP_FLAG_RDERR )?((((__HANDLE__)->Instance->SR \
                                                       & (CRYP_FLAG_RDERR & 0x7FFFFFFFU)) == \
                                                       (CRYP_FLAG_RDERR & 0x7FFFFFFFU))?SET:RESET) : \
                                                   ((__FLAG__) == CRYP_FLAG_KEIF )?((((__HANDLE__)->Instance->ISR \
                                                       & (CRYP_FLAG_KEIF)) == (CRYP_FLAG_KEIF))?SET:RESET) : \
                                                   ((__FLAG__) == CRYP_FLAG_RWEIF )?((((__HANDLE__)->Instance->ISR \
                                                       & (CRYP_FLAG_RWEIF)) == (CRYP_FLAG_RWEIF))?SET:RESET) : \
                                                   ((((__HANDLE__)->Instance->ISR & (CRYP_FLAG_CCF)) == (CRYP_FLAG_CCF)))?SET:RESET)

/** @brief  Clear the CRYP pending status flag.
  * @param  __HANDLE__ specifies the CRYP handle.
  * @param  __FLAG__ specifies the flag to clear.
  *         This parameter can be one of the following values:
  *            @arg @ref CRYP_CLEAR_RWEIF Read (RDERR), Write (WRERR) or Read/write (RWEIF) Error  Flag Clear
  *            @arg @ref CRYP_CLEAR_CCF Computation Complete Flag (CCF) Clear
  *            @arg @ref CRYP_CLEAR_KEIF Key error interrupt flag clear
  * @retval None
  */

#define __HAL_CRYP_CLEAR_FLAG(__HANDLE__, __FLAG__) SET_BIT((__HANDLE__)->Instance->ICR, (__FLAG__))


/** @brief  Check whether the specified CRYP interrupt source is enabled or not.
  * @param  __HANDLE__ specifies the CRYP handle.
  * @param __INTERRUPT__ CRYP interrupt source to check
  *         This parameter can be one of the following values for TinyAES:
  *            @arg @ref CRYP_IT_RWEIE Error interrupt (used for RDERR and WRERR)
  *            @arg @ref CRYP_IT_CCFIE Computation Complete interrupt
  *            @arg @ref CRYP_IT_KEIE Key error interrupt
  * @retval State of interruption (TRUE or FALSE).
  */

#define __HAL_CRYP_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->Instance->IER\
                                                              & (__INTERRUPT__)) == (__INTERRUPT__))

/**
  * @brief  Enable the CRYP interrupt.
  * @param  __HANDLE__ specifies the CRYP handle.
  * @param  __INTERRUPT__ CRYP Interrupt.
  *         This parameter can be one of the following values for TinyAES:
  *            @arg @ref CRYP_IT_RWEIE Error interrupt (used for RDERR and WRERR)
  *            @arg @ref CRYP_IT_CCFIE Computation Complete interrupt
  *            @arg @ref CRYP_IT_KEIE Key error interrupt
  * @retval None
  */

#define __HAL_CRYP_ENABLE_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->Instance->IER) |= (__INTERRUPT__))

/**
  * @brief  Disable the CRYP interrupt.
  * @param  __HANDLE__ specifies the CRYP handle.
  * @param  __INTERRUPT__ CRYP Interrupt.
  *         This parameter can be one of the following values for TinyAES:
  *            @arg @ref CRYP_IT_RWEIE Error interrupt (used for RDERR and WRERR)
  *            @arg @ref CRYP_IT_CCFIE Computation Complete interrupt
  *            @arg @ref CRYP_IT_KEIE Key error interrupt
  * @retval None
  */

#define __HAL_CRYP_DISABLE_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->Instance->IER) &= ~(__INTERRUPT__))

/**
  * @}
  */

/* Include CRYP HAL Extended module */
#include "stm32u5xx_hal_cryp_ex.h"

/* Exported functions --------------------------------------------------------*/
/** @defgroup CRYP_Exported_Functions CRYP Exported Functions
  * @{
  */

/** @addtogroup CRYP_Exported_Functions_Group1
  * @{
  */
HAL_StatusTypeDef HAL_CRYP_Init(CRYP_HandleTypeDef *hcryp);
HAL_StatusTypeDef HAL_CRYP_DeInit(CRYP_HandleTypeDef *hcryp);
void HAL_CRYP_MspInit(CRYP_HandleTypeDef *hcryp);
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp);
HAL_StatusTypeDef HAL_CRYP_SetConfig(CRYP_HandleTypeDef *hcryp, CRYP_ConfigTypeDef *pConf);
HAL_StatusTypeDef HAL_CRYP_GetConfig(CRYP_HandleTypeDef *hcryp, CRYP_ConfigTypeDef *pConf);
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1U)
HAL_StatusTypeDef HAL_CRYP_RegisterCallback(CRYP_HandleTypeDef *hcryp, HAL_CRYP_CallbackIDTypeDef CallbackID,
                                            pCRYP_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_CRYP_UnRegisterCallback(CRYP_HandleTypeDef *hcryp, HAL_CRYP_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
#if (USE_HAL_CRYP_SUSPEND_RESUME == 1U)
void HAL_CRYP_ProcessSuspend(CRYP_HandleTypeDef *hcryp);
HAL_StatusTypeDef HAL_CRYP_Suspend(CRYP_HandleTypeDef *hcryp);
HAL_StatusTypeDef HAL_CRYP_Resume(CRYP_HandleTypeDef *hcryp);
#endif /* defined (USE_HAL_CRYP_SUSPEND_RESUME) */
HAL_StatusTypeDef  HAL_CRYP_SaveContext(CRYP_HandleTypeDef *hcryp, CRYP_ContextTypeDef *pcont);
HAL_StatusTypeDef HAL_CRYP_RestoreContext(CRYP_HandleTypeDef *hcryp, CRYP_ContextTypeDef *pcont);

/**
  * @}
  */

/** @addtogroup CRYP_Exported_Functions_Group2
  * @{
  */

/* encryption/decryption ***********************************/
HAL_StatusTypeDef HAL_CRYP_Encrypt(CRYP_HandleTypeDef *hcryp, uint32_t *pInput, uint16_t Size, uint32_t *pOutput,
                                   uint32_t Timeout);
HAL_StatusTypeDef HAL_CRYP_Decrypt(CRYP_HandleTypeDef *hcryp, uint32_t *pInput, uint16_t Size, uint32_t *pOutput,
                                   uint32_t Timeout);
HAL_StatusTypeDef HAL_CRYP_Encrypt_IT(CRYP_HandleTypeDef *hcryp, uint32_t *pInput, uint16_t Size, uint32_t *pOutput);
HAL_StatusTypeDef HAL_CRYP_Decrypt_IT(CRYP_HandleTypeDef *hcryp, uint32_t *pInput, uint16_t Size, uint32_t *pOutput);
HAL_StatusTypeDef HAL_CRYP_Encrypt_DMA(CRYP_HandleTypeDef *hcryp, uint32_t *pInput, uint16_t Size, uint32_t *pOutput);
HAL_StatusTypeDef HAL_CRYP_Decrypt_DMA(CRYP_HandleTypeDef *hcryp, uint32_t *pInput, uint16_t Size, uint32_t *pOutput);

/**
  * @}
  */


/** @addtogroup CRYP_Exported_Functions_Group3
  * @{
  */
/* Interrupt Handler functions  **********************************************/
void HAL_CRYP_IRQHandler(CRYP_HandleTypeDef *hcryp);
HAL_CRYP_STATETypeDef HAL_CRYP_GetState(const CRYP_HandleTypeDef *hcryp);
void HAL_CRYP_InCpltCallback(CRYP_HandleTypeDef *hcryp);
void HAL_CRYP_OutCpltCallback(CRYP_HandleTypeDef *hcryp);
void HAL_CRYP_ErrorCallback(CRYP_HandleTypeDef *hcryp);
uint32_t HAL_CRYP_GetError(const CRYP_HandleTypeDef *hcryp);

/**
  * @}
  */

/**
  * @}
  */

/* Private macros --------------------------------------------------------*/
/** @defgroup CRYP_Private_Macros CRYP Private Macros
  * @{
  */

#define IS_CRYP_INSTANCE(INSTANCE)(((INSTANCE) == AES)   || \
                                   ((INSTANCE) == SAES))

#define IS_CRYP_ALGORITHM(ALGORITHM) (((ALGORITHM) == CRYP_AES_ECB)      || \
                                      ((ALGORITHM)  == CRYP_AES_CBC)     || \
                                      ((ALGORITHM)  == CRYP_AES_CTR)     || \
                                      ((ALGORITHM)  == CRYP_AES_GCM_GMAC)|| \
                                      ((ALGORITHM)  == CRYP_AES_CCM))


#define IS_CRYP_KEYSIZE(KEYSIZE)(((KEYSIZE) == CRYP_KEYSIZE_128B)   || \
                                 ((KEYSIZE) == CRYP_KEYSIZE_256B))

#define IS_CRYP_DATATYPE(DATATYPE)(((DATATYPE) == CRYP_NO_SWAP) || \
                                   ((DATATYPE) == CRYP_HALFWORD_SWAP) || \
                                   ((DATATYPE) == CRYP_BYTE_SWAP)  || \
                                   ((DATATYPE) == CRYP_BIT_SWAP))

#define IS_CRYP_INIT(CONFIG)(((CONFIG) == CRYP_KEYIVCONFIG_ALWAYS) || \
                             ((CONFIG) == CRYP_KEYNOCONFIG)  || \
                             ((CONFIG) == CRYP_IVCONFIG_ONCE)  || \
                             ((CONFIG) == CRYP_KEYIVCONFIG_ONCE))

#define IS_CRYP_BUFFERSIZE(ALGO, DATAWIDTH, SIZE)                                             \
  (((((ALGO) == CRYP_AES_CTR)) &&                                             \
    ((((DATAWIDTH) == CRYP_DATAWIDTHUNIT_WORD) && (((SIZE) % 4U) == 0U))           || \
     (((DATAWIDTH) == CRYP_DATAWIDTHUNIT_BYTE) && (((SIZE) % 16U) == 0U))))        || \
   (((ALGO) == CRYP_AES_ECB) || ((ALGO) == CRYP_AES_CBC)                  || \
    ((ALGO)== CRYP_AES_GCM_GMAC) || ((ALGO) == CRYP_AES_CCM)))


/**
  * @}
  */


/* Private constants ---------------------------------------------------------*/
/** @defgroup CRYP_Private_Constants CRYP Private Constants
  * @{
  */

/**
  * @}
  */
/* Private defines -----------------------------------------------------------*/
/** @defgroup CRYP_Private_Defines CRYP Private Defines
  * @{
  */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup CRYP_Private_Variables CRYP Private Variables
  * @{
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup CRYP_Private_Functions CRYP Private Functions
  * @{
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* AES */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32U5xx_HAL_CRYP_H */
