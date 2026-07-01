/**
  ******************************************************************************
  * @file    stm32u5xx_hal_pka.h
  * @author  MCD Application Team
  * @brief   Header file of PKA HAL module.
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
#ifndef STM32U5xx_HAL_PKA_H
#define STM32U5xx_HAL_PKA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal_def.h"

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

#if defined(PKA) && defined(HAL_PKA_MODULE_ENABLED)

/** @addtogroup PKA
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup PKA_Exported_Types PKA Exported Types
  * @{
  */

/** @defgroup HAL_state_structure_definition HAL state structure definition
  * @brief  HAL State structures definition
  * @{
  */
typedef enum
{
  HAL_PKA_STATE_RESET   = 0x00U,
  HAL_PKA_STATE_READY   = 0x01U,
  HAL_PKA_STATE_BUSY    = 0x02U,
  HAL_PKA_STATE_ERROR   = 0x03U,
}
HAL_PKA_StateTypeDef;

/**
  * @}
  */

#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
/** @defgroup HAL_callback_id HAL callback ID enumeration
  * @{
  */
typedef enum
{
  HAL_PKA_OPERATION_COMPLETE_CB_ID      = 0x00U,
  HAL_PKA_ERROR_CB_ID                   = 0x01U,
  HAL_PKA_MSPINIT_CB_ID                 = 0x02U,
  HAL_PKA_MSPDEINIT_CB_ID               = 0x03U
} HAL_PKA_CallbackIDTypeDef;

/**
  * @}
  */

#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */

/** @defgroup PKA_Error_Code_definition PKA Error Code definition
  * @brief  PKA Error Code definition
  * @{
  */
#define HAL_PKA_ERROR_NONE      (0x00000000U)
#define HAL_PKA_ERROR_ADDRERR   (0x00000001U)
#define HAL_PKA_ERROR_RAMERR    (0x00000002U)
#define HAL_PKA_ERROR_TIMEOUT   (0x00000004U)
#define HAL_PKA_ERROR_OPERATION (0x00000008U)
#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
#define HAL_PKA_ERROR_INVALID_CALLBACK  (0x00000010U)
#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup PKA_handle_Structure_definition PKA handle Structure definition
  * @brief  PKA handle Structure definition
  * @{
  */
#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
typedef struct __PKA_HandleTypeDef
#else
typedef struct
#endif  /* USE_HAL_PKA_REGISTER_CALLBACKS */
{
  PKA_TypeDef                   *Instance;
  __IO HAL_PKA_StateTypeDef     State;
  __IO uint32_t                 ErrorCode;
  __IO uint32_t                 primeordersize;
  __IO uint32_t                 opsize;
  __IO uint32_t                 modulussize;
#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
  void (* OperationCpltCallback)(struct __PKA_HandleTypeDef *hpka);
  void (* ErrorCallback)(struct __PKA_HandleTypeDef *hpka);
  void (* MspInitCallback)(struct __PKA_HandleTypeDef *hpka);
  void (* MspDeInitCallback)(struct __PKA_HandleTypeDef *hpka);
#endif  /* USE_HAL_PKA_REGISTER_CALLBACKS */
} PKA_HandleTypeDef;
/**
  * @}
  */

#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
/** @defgroup PKA_Callback_definition PKA Callback pointer definition
  * @brief  PKA Callback pointer definition
  * @{
  */
typedef  void (*pPKA_CallbackTypeDef)(PKA_HandleTypeDef *hpka);
/**
  * @}
  */
#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */
/** @defgroup PKA_Operation PKA operation structure definition
  * @brief  Input and output data definition
  * @{
  */

typedef struct
{
  uint32_t scalarMulSize;
  uint32_t modulusSize;
  uint32_t coefSign;
  const uint8_t *coefA;
  const uint8_t *coefB;
  const uint8_t *modulus;
  const uint8_t *pointX;
  const uint8_t *pointY;
  const uint8_t *scalarMul;
  const uint8_t *primeOrder;
} PKA_ECCMulInTypeDef;

typedef struct
{
  uint32_t primeOrderSize;
  uint32_t scalarMulSize;
  uint32_t modulusSize;
  uint32_t coefSign;
  const uint8_t *coefA;
  const uint8_t *coefB;
  const uint8_t *modulus;
  const uint8_t *pointX;
  const uint8_t *pointY;
  const uint8_t *scalarMul;
  const uint8_t *primeOrder;
} PKA_ECCMulExInTypeDef;

typedef struct
{
  uint32_t modulusSize;
  uint32_t coefSign;
  const uint8_t *coefA;
  const uint8_t *coefB;
  const uint8_t *modulus;
  const uint8_t *pointX;
  const uint8_t *pointY;
  const uint32_t *pMontgomeryParam;
} PKA_PointCheckInTypeDef;

typedef struct
{
  uint32_t size;
  const uint8_t *pOpDp;
  const uint8_t *pOpDq;
  const uint8_t *pOpQinv;
  const uint8_t *pPrimeP;
  const uint8_t *pPrimeQ;
  const uint8_t *popA;
} PKA_RSACRTExpInTypeDef;

typedef struct
{
  uint32_t primeOrderSize;
  uint32_t modulusSize;
  uint32_t coefSign;
  const uint8_t *coef;
  const uint8_t *modulus;
  const uint8_t *basePointX;
  const uint8_t *basePointY;
  const uint8_t *pPubKeyCurvePtX;
  const uint8_t *pPubKeyCurvePtY;
  const uint8_t *RSign;
  const uint8_t *SSign;
  const uint8_t *hash;
  const uint8_t *primeOrder;
} PKA_ECDSAVerifInTypeDef;

typedef struct
{
  uint32_t primeOrderSize;
  uint32_t modulusSize;
  uint32_t coefSign;
  const uint8_t *coef;
  const uint8_t *coefB;
  const uint8_t *modulus;
  const uint8_t *integer;
  const uint8_t *basePointX;
  const uint8_t *basePointY;
  const uint8_t *hash;
  const uint8_t *privateKey;
  const uint8_t *primeOrder;
} PKA_ECDSASignInTypeDef;

typedef struct
{
  uint8_t *RSign;
  uint8_t *SSign;
} PKA_ECDSASignOutTypeDef;

typedef struct
{
  uint8_t *ptX;
  uint8_t *ptY;
} PKA_ECDSASignOutExtParamTypeDef, PKA_ECCMulOutTypeDef, PKA_ECCProjective2AffineOutTypeDef,
PKA_ECCDoubleBaseLadderOutTypeDef;

typedef struct
{
  uint8_t *ptX;
  uint8_t *ptY;
  uint8_t *ptZ;
} PKA_ECCCompleteAdditionOutTypeDef;

typedef struct
{
  uint32_t expSize;
  uint32_t OpSize;
  const uint8_t *pExp;
  const uint8_t *pOp1;
  const uint8_t *pMod;
} PKA_ModExpInTypeDef;

typedef struct
{
  uint32_t expSize;
  uint32_t OpSize;
  const uint8_t *pOp1;
  const uint8_t *pExp;
  const uint8_t *pMod;
  const uint8_t *pPhi;
} PKA_ModExpProtectModeInTypeDef;

typedef struct
{
  uint32_t expSize;
  uint32_t OpSize;
  const uint8_t *pExp;
  const uint8_t *pOp1;
  const uint8_t *pMod;
  const uint32_t *pMontgomeryParam;
} PKA_ModExpFastModeInTypeDef;

typedef struct
{
  uint32_t size;
  const uint8_t *pOp1;
} PKA_MontgomeryParamInTypeDef;

typedef struct
{
  uint32_t size;
  const uint32_t *pOp1;
  const uint32_t *pOp2;
} PKA_AddInTypeDef, PKA_SubInTypeDef, PKA_MulInTypeDef, PKA_CmpInTypeDef;

typedef struct
{
  uint32_t size;
  const uint32_t *pOp1;
  const uint8_t *pMod;
} PKA_ModInvInTypeDef;

typedef struct
{
  uint32_t OpSize;
  uint32_t modSize;
  const uint32_t *pOp1;
  const uint8_t *pMod;
} PKA_ModRedInTypeDef;

typedef struct
{
  uint32_t size;
  const uint32_t *pOp1;
  const uint32_t *pOp2;
  const uint8_t  *pOp3;
} PKA_ModAddInTypeDef, PKA_ModSubInTypeDef, PKA_MontgomeryMulInTypeDef;

typedef struct
{
  uint32_t primeOrderSize;
  uint32_t modulusSize;
  uint32_t coefSign;
  const uint8_t *coefA;
  const uint8_t *modulus;
  const uint8_t *integerK;
  const uint8_t *integerM;
  const uint8_t *basePointX1;
  const uint8_t *basePointY1;
  const uint8_t *basePointZ1;
  const uint8_t *basePointX2;
  const uint8_t *basePointY2;
  const uint8_t *basePointZ2;
} PKA_ECCDoubleBaseLadderInTypeDef;

typedef struct
{
  uint32_t modulusSize;
  const uint8_t *modulus;
  const uint8_t *basePointX;
  const uint8_t *basePointY;
  const uint8_t *basePointZ;
  const uint32_t *pMontgomeryParam;
} PKA_ECCProjective2AffineInTypeDef;

typedef struct
{
  uint32_t modulusSize;
  uint32_t coefSign;
  const uint8_t *modulus;
  const uint8_t *coefA;
  const uint8_t *basePointX1;
  const uint8_t *basePointY1;
  const uint8_t *basePointZ1;
  const uint8_t *basePointX2;
  const uint8_t *basePointY2;
  const uint8_t *basePointZ2;
} PKA_ECCCompleteAdditionInTypeDef;
/**
  * @}
  */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup PKA_Exported_Constants PKA Exported Constants
  * @{
  */

/** @defgroup PKA_Mode PKA mode
  * @{
  */
#define PKA_MODE_MONTGOMERY_PARAM                 (0x00000001U)
#define PKA_MODE_MODULAR_EXP                      (0x00000000U)
#define PKA_MODE_MODULAR_EXP_FAST_MODE            (0x00000002U)
#define PKA_MODE_ECC_MUL                          (0x00000020U)
#define PKA_MODE_ECDSA_SIGNATURE                  (0x00000024U)
#define PKA_MODE_ECDSA_VERIFICATION               (0x00000026U)
#define PKA_MODE_POINT_CHECK                      (0x00000028U)
#define PKA_MODE_RSA_CRT_EXP                      (0x00000007U)
#define PKA_MODE_MODULAR_INV                      (0x00000008U)
#define PKA_MODE_ARITHMETIC_ADD                   (0x00000009U)
#define PKA_MODE_ARITHMETIC_SUB                   (0x0000000AU)
#define PKA_MODE_ARITHMETIC_MUL                   (0x0000000BU)
#define PKA_MODE_COMPARISON                       (0x0000000CU)
#define PKA_MODE_MODULAR_RED                      (0x0000000DU)
#define PKA_MODE_MODULAR_ADD                      (0x0000000EU)
#define PKA_MODE_MODULAR_SUB                      (0x0000000FU)
#define PKA_MODE_MONTGOMERY_MUL                   (0x00000010U)
#define PKA_MODE_ECC_PROJECTIVE_AFF               (0x0000002FU)
#define PKA_MODE_DOUBLE_BASE_LADDER               (0x00000027U)
#define PKA_MODE_ECC_COMPLETE_ADD                 (0x00000023U)
#define PKA_MODE_MODULAR_EXP_PROTECT              (0x00000003U)
/**
  * @}
  */

/** @defgroup PKA_Interrupt_configuration_definition PKA Interrupt configuration definition
  * @brief PKA Interrupt definition
  * @{
  */
#define PKA_IT_PROCEND                            PKA_CR_PROCENDIE
#define PKA_IT_ADDRERR                            PKA_CR_ADDRERRIE
#define PKA_IT_RAMERR                             PKA_CR_RAMERRIE
#define PKA_IT_OPERR                              PKA_CR_OPERRIE

/**
  * @}
  */

/** @defgroup PKA_Flag_definition PKA Flag definition
  * @{
  */
#define PKA_FLAG_PROCEND                          PKA_SR_PROCENDF
#define PKA_FLAG_ADDRERR                          PKA_SR_ADDRERRF
#define PKA_FLAG_RAMERR                           PKA_SR_RAMERRF
#define PKA_FLAG_OPERR                            PKA_SR_OPERRF

/**
  * @}
  */

/** @defgroup PKA_Operation_Status PKA Operation Status
  * @{
  */
#define PKA_NO_ERROR                           0xD60DUL
#define PKA_FAILED_COMPUTATION                 0xCBC9UL
#define PKA_RPART_SIGNATURE_NULL               0xA3B7UL
#define PKA_SPART_SIGNATURE_NULL               0xF946UL

/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/

/** @defgroup PKA_Exported_Macros PKA Exported Macros
  * @{
  */

/** @brief  Reset PKA handle state.
  * @param  __HANDLE__ specifies the PKA Handle
  * @retval None
  */
#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
#define __HAL_PKA_RESET_HANDLE_STATE(__HANDLE__)                do{                                                   \
                                                                    (__HANDLE__)->State = HAL_PKA_STATE_RESET;       \
                                                                    (__HANDLE__)->MspInitCallback = NULL;            \
                                                                    (__HANDLE__)->MspDeInitCallback = NULL;          \
                                                                  } while(0)
#else
#define __HAL_PKA_RESET_HANDLE_STATE(__HANDLE__)                ((__HANDLE__)->State = HAL_PKA_STATE_RESET)
#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */

/** @brief  Enable the specified PKA interrupt.
  * @param  __HANDLE__ specifies the PKA Handle
  * @param  __INTERRUPT__ specifies the interrupt source to enable.
  *        This parameter can be one of the following values:
  *            @arg @ref PKA_IT_PROCEND End Of Operation interrupt enable
  *            @arg @ref PKA_IT_ADDRERR Address error interrupt enable
  *            @arg @ref PKA_IT_RAMERR RAM error interrupt enable
  *            @arg @ref PKA_IT_OPERR Operation error interrupt enable
  * @retval None
  */
#define __HAL_PKA_ENABLE_IT(__HANDLE__, __INTERRUPT__)          ((__HANDLE__)->Instance->CR |= (__INTERRUPT__))

/** @brief  Disable the specified PKA interrupt.
  * @param  __HANDLE__ specifies the PKA Handle
  * @param  __INTERRUPT__ specifies the interrupt source to disable.
  *        This parameter can be one of the following values:
  *            @arg @ref PKA_IT_PROCEND End Of Operation interrupt enable
  *            @arg @ref PKA_IT_ADDRERR Address error interrupt enable
  *            @arg @ref PKA_IT_RAMERR RAM error interrupt enable
  *            @arg @ref PKA_IT_OPERR Operation error interrupt enable
  * @retval None
  */
#define __HAL_PKA_DISABLE_IT(__HANDLE__, __INTERRUPT__)         ((__HANDLE__)->Instance->CR &= (~(__INTERRUPT__)))

/** @brief  Check whether the specified PKA interrupt source is enabled or not.
  * @param  __HANDLE__ specifies the PKA Handle
  * @param  __INTERRUPT__ specifies the PKA interrupt source to check.
  *        This parameter can be one of the following values:
  *            @arg @ref PKA_IT_PROCEND End Of Operation interrupt enable
  *            @arg @ref PKA_IT_ADDRERR Address error interrupt enable
  *            @arg @ref PKA_IT_RAMERR RAM error interrupt enable
  *            @arg @ref PKA_IT_OPERR Operation error interrupt enable
  * @retval The new state of __INTERRUPT__ (SET or RESET)
  */
#define __HAL_PKA_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)      ((((__HANDLE__)->Instance->CR\
                                                                   & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)

/** @brief  Check whether the specified PKA flag is set or not.
  * @param  __HANDLE__ specifies the PKA Handle
  * @param  __FLAG__ specifies the flag to check.
  *        This parameter can be one of the following values:
  *            @arg @ref PKA_FLAG_PROCEND End Of Operation
  *            @arg @ref PKA_FLAG_ADDRERR Address error
  *            @arg @ref PKA_FLAG_RAMERR RAM error
  *            @arg @ref PKA_FLAG_OPERR Operation error
  * @retval The new state of __FLAG__ (SET or RESET)
  */
#define __HAL_PKA_GET_FLAG(__HANDLE__, __FLAG__)                (((((__HANDLE__)->Instance->SR)\
                                                                   & (__FLAG__)) == (__FLAG__)) ? SET : RESET)

/** @brief  Clear the PKA pending flags which are cleared by writing 1 in a specific bit.
  * @param  __HANDLE__ specifies the PKA Handle
  * @param  __FLAG__ specifies the flag to clear.
  *          This parameter can be any combination of the following values:
  *            @arg @ref PKA_FLAG_PROCEND End Of Operation
  *            @arg @ref PKA_FLAG_ADDRERR Address error
  *            @arg @ref PKA_FLAG_RAMERR RAM error
  *            @arg @ref PKA_FLAG_OPERR Operation error
  * @retval None
  */
#define __HAL_PKA_CLEAR_FLAG(__HANDLE__, __FLAG__)              ((__HANDLE__)->Instance->CLRFR = (__FLAG__))

/** @brief  Enable the specified PKA peripheral.
  * @param  __HANDLE__ specifies the PKA Handle
  * @retval None
  */
#define __HAL_PKA_ENABLE(__HANDLE__)                            (SET_BIT((__HANDLE__)->Instance->CR,  PKA_CR_EN))

/** @brief  Disable the specified PKA peripheral.
  * @param  __HANDLE__ specifies the PKA Handle
  * @retval None
  */
#define __HAL_PKA_DISABLE(__HANDLE__)                           (CLEAR_BIT((__HANDLE__)->Instance->CR, PKA_CR_EN))

/** @brief  Start a PKA operation.
  * @param  __HANDLE__ specifies the PKA Handle
  * @retval None
  */
#define __HAL_PKA_START(__HANDLE__)                             (SET_BIT((__HANDLE__)->Instance->CR,  PKA_CR_START))
/**
  * @}
  */

/* Private macros --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @addtogroup PKA_Exported_Functions
  * @{
  */

/** @addtogroup PKA_Exported_Functions_Group1
  * @{
  */
/* Initialization and de-initialization functions *****************************/
HAL_StatusTypeDef HAL_PKA_Init(PKA_HandleTypeDef *hpka);
HAL_StatusTypeDef HAL_PKA_DeInit(PKA_HandleTypeDef *hpka);
void              HAL_PKA_MspInit(PKA_HandleTypeDef *hpka);
void              HAL_PKA_MspDeInit(PKA_HandleTypeDef *hpka);

#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
/* Callbacks Register/UnRegister functions  ***********************************/
HAL_StatusTypeDef HAL_PKA_RegisterCallback(PKA_HandleTypeDef *hpka, HAL_PKA_CallbackIDTypeDef CallbackID,
                                           pPKA_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_PKA_UnRegisterCallback(PKA_HandleTypeDef *hpka, HAL_PKA_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @addtogroup PKA_Exported_Functions_Group2
  * @{
  */
/* IO operation functions *****************************************************/
/* High Level Functions *******************************************************/
HAL_StatusTypeDef HAL_PKA_ModExp(PKA_HandleTypeDef *hpka, PKA_ModExpInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ModExp_IT(PKA_HandleTypeDef *hpka, PKA_ModExpInTypeDef *in);
HAL_StatusTypeDef HAL_PKA_ModExpFastMode(PKA_HandleTypeDef *hpka, PKA_ModExpFastModeInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ModExpFastMode_IT(PKA_HandleTypeDef *hpka, PKA_ModExpFastModeInTypeDef *in);
HAL_StatusTypeDef HAL_PKA_ModExpProtectMode(PKA_HandleTypeDef *hpka, PKA_ModExpProtectModeInTypeDef *in,
                                            uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ModExpProtectMode_IT(PKA_HandleTypeDef *hpka, PKA_ModExpProtectModeInTypeDef *in);
void HAL_PKA_ModExp_GetResult(PKA_HandleTypeDef *hpka, uint8_t *pRes);

HAL_StatusTypeDef HAL_PKA_ECDSASign(PKA_HandleTypeDef *hpka, PKA_ECDSASignInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ECDSASign_IT(PKA_HandleTypeDef *hpka, PKA_ECDSASignInTypeDef *in);
void HAL_PKA_ECDSASign_GetResult(PKA_HandleTypeDef *hpka, PKA_ECDSASignOutTypeDef *out,
                                 PKA_ECDSASignOutExtParamTypeDef *outExt);

HAL_StatusTypeDef HAL_PKA_ECDSAVerif(PKA_HandleTypeDef *hpka, PKA_ECDSAVerifInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ECDSAVerif_IT(PKA_HandleTypeDef *hpka, PKA_ECDSAVerifInTypeDef *in);
uint32_t HAL_PKA_ECDSAVerif_IsValidSignature(PKA_HandleTypeDef const *const hpka);

HAL_StatusTypeDef HAL_PKA_RSACRTExp(PKA_HandleTypeDef *hpka, PKA_RSACRTExpInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_RSACRTExp_IT(PKA_HandleTypeDef *hpka, PKA_RSACRTExpInTypeDef *in);
void HAL_PKA_RSACRTExp_GetResult(PKA_HandleTypeDef *hpka, uint8_t *pRes);

HAL_StatusTypeDef HAL_PKA_PointCheck(PKA_HandleTypeDef *hpka, PKA_PointCheckInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_PointCheck_IT(PKA_HandleTypeDef *hpka, PKA_PointCheckInTypeDef *in);
uint32_t HAL_PKA_PointCheck_IsOnCurve(PKA_HandleTypeDef const *const hpka);

HAL_StatusTypeDef HAL_PKA_ECCMul(PKA_HandleTypeDef *hpka, PKA_ECCMulInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ECCMul_IT(PKA_HandleTypeDef *hpka, PKA_ECCMulInTypeDef *in);
HAL_StatusTypeDef HAL_PKA_ECCMulEx(PKA_HandleTypeDef *hpka, PKA_ECCMulExInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ECCMulEx_IT(PKA_HandleTypeDef *hpka, PKA_ECCMulExInTypeDef *in);
void HAL_PKA_ECCMul_GetResult(PKA_HandleTypeDef *hpka, PKA_ECCMulOutTypeDef *out);

HAL_StatusTypeDef HAL_PKA_Add(PKA_HandleTypeDef *hpka, PKA_AddInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_Add_IT(PKA_HandleTypeDef *hpka, PKA_AddInTypeDef *in);
HAL_StatusTypeDef HAL_PKA_Sub(PKA_HandleTypeDef *hpka, PKA_SubInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_Sub_IT(PKA_HandleTypeDef *hpka, PKA_SubInTypeDef *in);
HAL_StatusTypeDef HAL_PKA_Cmp(PKA_HandleTypeDef *hpka, PKA_CmpInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_Cmp_IT(PKA_HandleTypeDef *hpka, PKA_CmpInTypeDef *in);
HAL_StatusTypeDef HAL_PKA_Mul(PKA_HandleTypeDef *hpka, PKA_MulInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_Mul_IT(PKA_HandleTypeDef *hpka, PKA_MulInTypeDef *in);
HAL_StatusTypeDef HAL_PKA_ModAdd(PKA_HandleTypeDef *hpka, PKA_ModAddInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ModAdd_IT(PKA_HandleTypeDef *hpka, PKA_ModAddInTypeDef *in);
HAL_StatusTypeDef HAL_PKA_ModSub(PKA_HandleTypeDef *hpka, PKA_ModSubInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ModSub_IT(PKA_HandleTypeDef *hpka, PKA_ModSubInTypeDef *in);
HAL_StatusTypeDef HAL_PKA_ModInv(PKA_HandleTypeDef *hpka, PKA_ModInvInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ModInv_IT(PKA_HandleTypeDef *hpka, PKA_ModInvInTypeDef *in);
HAL_StatusTypeDef HAL_PKA_ModRed(PKA_HandleTypeDef *hpka, PKA_ModRedInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ModRed_IT(PKA_HandleTypeDef *hpka, PKA_ModRedInTypeDef *in);
HAL_StatusTypeDef HAL_PKA_MontgomeryMul(PKA_HandleTypeDef *hpka, PKA_MontgomeryMulInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_MontgomeryMul_IT(PKA_HandleTypeDef *hpka, PKA_MontgomeryMulInTypeDef *in);
void HAL_PKA_Arithmetic_GetResult(PKA_HandleTypeDef *hpka, uint32_t *pRes);

HAL_StatusTypeDef HAL_PKA_MontgomeryParam(PKA_HandleTypeDef *hpka, PKA_MontgomeryParamInTypeDef *in, uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_MontgomeryParam_IT(PKA_HandleTypeDef *hpka, PKA_MontgomeryParamInTypeDef *in);
void HAL_PKA_MontgomeryParam_GetResult(PKA_HandleTypeDef *hpka, uint32_t *pRes);

HAL_StatusTypeDef HAL_PKA_ECCDoubleBaseLadder(PKA_HandleTypeDef *hpka, PKA_ECCDoubleBaseLadderInTypeDef *in,
                                              uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ECCDoubleBaseLadder_IT(PKA_HandleTypeDef *hpka, PKA_ECCDoubleBaseLadderInTypeDef *in);
void HAL_PKA_ECCDoubleBaseLadder_GetResult(PKA_HandleTypeDef *hpka, PKA_ECCDoubleBaseLadderOutTypeDef *out);

HAL_StatusTypeDef HAL_PKA_ECCProjective2Affine(PKA_HandleTypeDef *hpka, PKA_ECCProjective2AffineInTypeDef *in,
                                               uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ECCProjective2Affine_IT(PKA_HandleTypeDef *hpka, PKA_ECCProjective2AffineInTypeDef *in);
void HAL_PKA_ECCProjective2Affine_GetResult(PKA_HandleTypeDef *hpka, PKA_ECCProjective2AffineOutTypeDef *out);

HAL_StatusTypeDef HAL_PKA_ECCCompleteAddition(PKA_HandleTypeDef *hpka, PKA_ECCCompleteAdditionInTypeDef *in,
                                              uint32_t Timeout);
HAL_StatusTypeDef HAL_PKA_ECCCompleteAddition_IT(PKA_HandleTypeDef *hpka, PKA_ECCCompleteAdditionInTypeDef *in);
void HAL_PKA_ECCCompleteAddition_GetResult(PKA_HandleTypeDef *hpka, PKA_ECCCompleteAdditionOutTypeDef *out);

HAL_StatusTypeDef HAL_PKA_Abort(PKA_HandleTypeDef *hpka);
void HAL_PKA_RAMReset(PKA_HandleTypeDef *hpka);
void HAL_PKA_OperationCpltCallback(PKA_HandleTypeDef *hpka);
void HAL_PKA_ErrorCallback(PKA_HandleTypeDef *hpka);
void HAL_PKA_IRQHandler(PKA_HandleTypeDef *hpka);
/**
  * @}
  */

/** @addtogroup PKA_Exported_Functions_Group3
  * @{
  */
/* Peripheral State and Error functions ***************************************/
HAL_PKA_StateTypeDef HAL_PKA_GetState(const PKA_HandleTypeDef *hpka);
uint32_t             HAL_PKA_GetError(const PKA_HandleTypeDef *hpka);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* defined(PKA) && defined(HAL_PKA_MODULE_ENABLED) */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32U5xx_HAL_PKA_H */
