/**
  ******************************************************************************
  * @file    stm32u5xx_hal_gfxtim.h
  * @author  MCD Application Team
  * @brief   Header file of GFXTIM HAL module.
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
#ifndef STM32U5xx_HAL_GFXTIM_H
#define STM32U5xx_HAL_GFXTIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal_def.h"

#if defined (GFXTIM)

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

/** @addtogroup GFXTIM
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup GFXTIM_Exported_Types  GFXTIM Exported Types
  * @{
  */

/**
  * @brief  HAL GFXTIM states definition
  */
typedef enum
{
  HAL_GFXTIM_STATE_RESET       = 0x00U,
  HAL_GFXTIM_STATE_READY       = 0x01U,
  HAL_GFXTIM_STATE_ERROR       = 0xFFU
} HAL_GFXTIM_StateTypeDef;

/**
  * @brief  GFXTIM initialization structure definition
  */
typedef struct
{
  uint32_t SynchroSrc;               /*!< Synchronization signals (HSYNC and VSYNC) sources.
                                     This parameter can be a value of @ref GFXTIM_SynchroSrc */
  uint32_t TearingEffectSrc;         /*!< Tearing effect source
                                     This parameter can be a value of @ref GFXTIM_TearingEffectSrc */
  uint32_t TearingEffectPolarity;    /*!< Tearing effect source
                                     This parameter can be a value of @ref GFXTIM_TearingEffectPolarity */
  uint32_t TearingEffectInterrupt;   /*!< Tearing effect interrupt Enable or Disable
                                     This parameter can be a value of @ref GFXTIM_Interrupt */
} GFXTIM_InitTypeDef;

/**
  * @brief  GFXTIM handle structure definition
  */
#if (USE_HAL_GFXTIM_REGISTER_CALLBACKS == 1)
typedef struct __GFXTIM_HandleTypeDef
#else
typedef struct
#endif /* USE_HAL_GFXTIM_REGISTER_CALLBACKS */
{
  GFXTIM_TypeDef                    *Instance;
  __IO HAL_GFXTIM_StateTypeDef      State;
  __IO uint32_t                     ErrorCode;
  GFXTIM_InitTypeDef                Init;
#if (USE_HAL_GFXTIM_REGISTER_CALLBACKS == 1)
  void (*HAL_GFXTIM_AbsoluteTimer_AFCC1Callback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*HAL_GFXTIM_AbsoluteTimer_AFCOFCallback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*HAL_GFXTIM_AbsoluteTimer_ALCC1Callback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*HAL_GFXTIM_AbsoluteTimer_ALCC2Callback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*HAL_GFXTIM_AbsoluteTimer_ALCOFCallback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*HAL_GFXTIM_RelativeTimer_RFC1RCallback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*HAL_GFXTIM_RelativeTimer_RFC2RCallback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*HAL_GFXTIM_TECallback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*HAL_GFXTIM_EventGenerator_EV1Callback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*HAL_GFXTIM_EventGenerator_EV2Callback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*HAL_GFXTIM_EventGenerator_EV3Callback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*HAL_GFXTIM_EventGenerator_EV4Callback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*HAL_GFXTIM_WatchdogTimer_AlarmCallback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*HAL_GFXTIM_WatchdogTimer_PreAlarmCallback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*ErrorCallback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*MspInitCallback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
  void (*MspDeInitCallback)(struct __GFXTIM_HandleTypeDef *hgfxtim);
#endif /* USE_HAL_GFXTIM_REGISTER_CALLBACKS */
} GFXTIM_HandleTypeDef;


#if (USE_HAL_GFXTIM_REGISTER_CALLBACKS == 1)
/**
  * @brief  GFXTIM callback ID enumeration definition
  */
typedef enum
{
  HAL_GFXTIM_AFC_COMPARE1_CB_ID   =  1U,
  HAL_GFXTIM_AFC_OVERFLOW_CB_ID   =  2U,
  HAL_GFXTIM_ALC_COMPARE1_CB_ID   =  3U,
  HAL_GFXTIM_ALC_COMPARE2_CB_ID   =  4U,
  HAL_GFXTIM_ALC_OVERFLOW_CB_ID   =  5U,
  HAL_GFXTIM_RFC1_RELOAD_CB_ID    =  6U,
  HAL_GFXTIM_RFC2_RELOAD_CB_ID    =  7U,
  HAL_GFXTIM_TE_CB_ID             =  8U,
  HAL_GFXTIM_EVENT1_CB_ID         =  9U,
  HAL_GFXTIM_EVENT2_CB_ID         = 10U,
  HAL_GFXTIM_EVENT3_CB_ID         = 11U,
  HAL_GFXTIM_EVENT4_CB_ID         = 12U,
  HAL_GFXTIM_WDG_ALARM_CB_ID      = 13U,
  HAL_GFXTIM_WDG_PREALARM_CB_ID   = 14U,
  HAL_GFXTIM_ERROR_CB_ID          = 15U,
  HAL_GFXTIM_MSP_INIT_CB_ID       = 16U,
  HAL_GFXTIM_MSP_DEINIT_CB_ID     = 17U,
} HAL_GFXTIM_CallbackIDTypeDef;

/**
  * @brief  GFXTIM callback pointers definition
  */
typedef void (*pGFXTIM_CallbackTypeDef)(GFXTIM_HandleTypeDef *hgfxtim);
#endif /* USE_HAL_GFXTIM_REGISTER_CALLBACKS */




/**
  * @brief  GFXTIM clock generator structure definition
  */
typedef struct
{
  uint32_t LCCHwReloadSrc;      /*!< Line Clock Counter hardware reload source
                                This parameter can be a value of @ref GFXTIM_LCCHwReloadSrc */

  uint32_t LCCReloadValue;      /*!< Line Clock Counter reload value (22 bits)
                                This parameter must be a number between Min_Data = 1 and Max_Data = 4194303 */

  uint32_t LCCClockSrc;         /*!< Line Clock Counter Clock Source
                                This parameter can be a value of @ref GFXTIM_LCCClockSrc */

  uint32_t LineClockSrc;        /*!< Line Clock Source
                                This parameter can be a value of @ref GFXTIM_LineClockSrc */

  uint32_t FCCHwReloadSrc;      /*!< Frame Clock Counter hardware reload source
                                This parameter can be a value of @ref GFXTIM_FCCHwReloadSrc */

  uint32_t FCCReloadValue;      /*!< Frame Clock Counter reload value (12 bits)
                                This parameter must be a number between Min_Data = 1 and Max_Data = 4095 */

  uint32_t FCCClockSrc;         /*!< Frame Clock Counter Clock Source
                                This parameter can be a value of @ref GFXTIM_FCCClockSrc */

  uint32_t FrameClockSrc;       /*!< Frame Clock Source
                                This parameter can be a value of @ref GFXTIM_FrameClockSrc */

  uint32_t LineClockCalib;      /*!< Debug purpose
                                This parameter can be a value of @ref GFXTIM_LineClockCalib */

  uint32_t FrameClockCalib;     /*!< Debug purpose
                                This parameter can be a value of @ref GFXTIM_FrameClockCalib */
} GFXTIM_ClockGeneratorConfigTypeDef;

/**
  * @brief  GFXTIM absolute timer configuration structure
  */
typedef struct
{
  uint32_t FrameCompare1Value;      /*!< Absolute Frame Compare 1 value (20 bits)
                                    This parameter must be a number between 1 and 1048575 */

  uint32_t FrameCounterValue;       /*!< Absolute Frame Counter initial value (20 bits)
                                    This parameter must be a number between 1 and 1048575 */

  uint32_t FrameOverflowInterrupt;  /*!< Absolute Frame Counter Overflow Interrupt Enable or Disable
                                    This parameter can be a value of @ref GFXTIM_Interrupt */

  uint32_t FrameCompare1Interrupt;  /*!< Absolute Frame Compare 1 Interrupt Enable or Disable
                                    This parameter can be a value of @ref GFXTIM_Interrupt */

  uint32_t LineCompare1Value;       /*!< Absolute Line Compare 1 value (12 bits)
                                    This parameter must be a number between 1 and 4095 */

  uint32_t LineCompare2Value;       /*!< Absolute Line Compare 2 value (12 bits)
                                    This parameter must be a number between 1 and 4095 */

  uint32_t LineCounterValue;        /*!< Absolute Line Counter value (12 bits)
                                    This parameter must be a number between 1 and 4095 */

  uint32_t LineOverflowInterrupt;   /*!< Absolute Line Counter Overflow Interrupt Enable or Disable
                                    This parameter can be a value of @ref GFXTIM_Interrupt */

  uint32_t LineCompare1Interrupt;   /*!< Absolute Line Compare 1 Interrupt Enable or Disable
                                    This parameter can be a value of @ref GFXTIM_Interrupt */

  uint32_t LineCompare2Interrupt;   /*!< Absolute Line Compare 2 Interrupt Enable or Disable
                                    This parameter can be a value of @ref GFXTIM_Interrupt */
} GFXTIM_AbsoluteTimerConfigTypeDef;


/**
  * @brief  GFXTIM relative timer configuration structure
  */
typedef struct
{
  uint32_t AutoReloadValue;      /*!< Auto reload value (12 bits)
                                 This parameter must be a number between 1 and 4095 */

  uint32_t CounterMode;          /*!< Counter Mode
                                 This parameter can be a value of GFXTIM_RelativeCounterMode */
  uint32_t ReloadInterrupt;      /*!< Relative Frame Counter Reload Interrupt Enable or Disable
                                 This parameter can be a value of @ref GFXTIM_Interrupt */
} GFXTIM_RelativeTimerConfigTypeDef;


/**
  * @brief  GFXTIM event generator configuration structure
  */
typedef struct
{
  uint32_t LineEvent;          /*!< Line event selection
                               This parameter can be a value of GFXTIM_EventLine */

  uint32_t FrameEvent;         /*!< Frmae event selection
                               This parameter can be a value of GFXTIM_EventFrame */

  uint32_t EventInterrupt;     /*!< Event interrupt Enable or Disable
                               This parameter can be a value of @ref GFXTIM_Interrupt */
} GFXTIM_EventGeneratorConfigTypeDef;

/**
  * @brief  GFXTIM watchdog configuration structure
  */
typedef struct
{
  uint32_t ClockSrc;          /*!< Clock source
                              This parameter can be a value of GFXTIM_WatchdogClockSrc */

  uint32_t AutoReloadValue;   /*!< Reload value (16 bits)
                              This parameter must be a number between 1 and 65535 */

  uint32_t HwReloadConfig;    /*!< Hardware reload configuration
                              This parameter can be a value of GFXTIM_WatchdogHwReloadConfig */

  uint32_t PreAlarmValue;     /*!< Pre-alarm value (16 bits)
                              This parameter must be a number between 1 and 65535 */

  uint32_t AlarmInterrupt;    /*!< Interrupt Enable or Disable when watchdog counter reaches 0
                              This parameter can be a value of @ref GFXTIM_Interrupt */

  uint32_t PreAlarmInterrupt; /*!< Interrupt Enable or Disable when watchdog counter reaches pre-alarm value
                               This parameter can be a value of @ref GFXTIM_Interrupt */
} GFXTIM_WatchdogConfigTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup GFXTIM_Exported_Constants GFXTIM Exported Constants
  * @{
  */

/** @defgroup GFXTIM_ErrorCode GFXTIM Error Code
  * @{
  */
#define GFXTIM_ERROR_NONE                      0U
#define GFXTIM_ERROR_STATE                     1U
#if (USE_HAL_GFXTIM_REGISTER_CALLBACKS == 1)
#define GFXTIM_ERROR_INVALID_CALLBACK          2U
#endif /* USE_HAL_GFXTIM_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup GFXTIM_Interrupt GFXTIM Interrupt
  * @{
  */
#define GFXTIM_IT_DISABLE                   0U
#define GFXTIM_IT_ENABLE                    1U

/**
  * @}
  */

/** @defgroup GFXTIM_SynchroSrc GFXTIM Synchronization Source
  * @{
  */
#define GFXTIM_SYNC_SRC_HSYNC_VSYNC_0    0U
#define GFXTIM_SYNC_SRC_HSYNC_VSYNC_1    GFXTIM_CR_SYNCS_0
#define GFXTIM_SYNC_SRC_HSYNC_VSYNC_2    GFXTIM_CR_SYNCS_1
#define GFXTIM_SYNC_SRC_HSYNC_VSYNC_3    (GFXTIM_CR_SYNCS_0 | GFXTIM_CR_SYNCS_1)
/**
  * @}
  */

/** @defgroup GFXTIM_TearingEffectSrc GFXTIM Tearing Effect Source
  * @{
  */
#define GFXTIM_TE_SRC_GPIO         0U
#define GFXTIM_TE_SRC_ITE          GFXTIM_CR_TES_0
#define GFXTIM_TE_SRC_HSYNC        GFXTIM_CR_TES_1
#define GFXTIM_TE_SRC_VSYNC        (GFXTIM_CR_TES_0 | GFXTIM_CR_TES_1)
/**
  * @}
  */

/** @defgroup GFXTIM_TearingEffectPolarity GFXTIM Tearing Effect Polarity
  * @{
  */
#define GFXTIM_TE_RISING_EDGE           0U
#define GFXTIM_TE_FALLING_EDGE          GFXTIM_CR_TEPOL
/**
  * @}
  */

/** @defgroup GFXTIM_LCCHwReloadSrc GFXTIM Line Clock Counter Hardware Reload Source
  * @{
  */
#define GFXTIM_LCC_HW_RELOAD_SRC_NONE           0U
#define GFXTIM_LCC_HW_RELOAD_SRC_FCC_UNDERFLOW  GFXTIM_CGCR_LCCHRS_0
#define GFXTIM_LCC_HW_RELOAD_SRC_HSYNC_RISING   GFXTIM_CGCR_LCCHRS_1
#define GFXTIM_LCC_HW_RELOAD_SRC_HSYNC_FALLING  (GFXTIM_CGCR_LCCHRS_0 | GFXTIM_CGCR_LCCHRS_1)
#define GFXTIM_LCC_HW_RELOAD_SRC_VSYNC_RISING   GFXTIM_CGCR_LCCHRS_2
#define GFXTIM_LCC_HW_RELOAD_SRC_VSYNC_FALLING  (GFXTIM_CGCR_LCCHRS_2 | GFXTIM_CGCR_LCCHRS_0)
#define GFXTIM_LCC_HW_RELOAD_SRC_TE_RISING      (GFXTIM_CGCR_LCCHRS_2 | GFXTIM_CGCR_LCCHRS_1)
#define GFXTIM_LCC_HW_RELOAD_SRC_TE_FALLING     (GFXTIM_CGCR_LCCHRS_2 | GFXTIM_CGCR_LCCHRS_1 | GFXTIM_CGCR_LCCHRS_0)
/**
  * @}
  */

/** @defgroup GFXTIM_LCCClockSrc GFXTIM Line Clock Counter Clock Source
  * @{
  */
#define GFXTIM_LCC_CLK_SRC_DISABLE    0U
#define GFXTIM_LCC_CLK_SRC_SYSCLOCK   GFXTIM_CGCR_LCCCS
/**
  * @}
  */

/** @defgroup GFXTIM_LineClockSrc GFXTIM Line Clock Source
  * @{
  */
#define GFXTIM_LINE_CLK_SRC_LCC_UNDERFLOW   0U
#define GFXTIM_LINE_CLK_SRC_FCC_UNDERFLOW   GFXTIM_CGCR_LCS_0
#define GFXTIM_LINE_CLK_SRC_HSYNC_RISING    GFXTIM_CGCR_LCS_1
#define GFXTIM_LINE_CLK_SRC_HSYNC_FALLING   (GFXTIM_CGCR_LCS_0 | GFXTIM_CGCR_LCS_1)
#define GFXTIM_LINE_CLK_SRC_VSYNC_RISING    GFXTIM_CGCR_LCS_2
#define GFXTIM_LINE_CLK_SRC_VSYNC_FALLING   (GFXTIM_CGCR_LCS_2 | GFXTIM_CGCR_LCS_0)
#define GFXTIM_LINE_CLK_SRC_TE_RISING       (GFXTIM_CGCR_LCS_2 | GFXTIM_CGCR_LCS_1)
#define GFXTIM_LINE_CLK_SRC_TE_FALLING      (GFXTIM_CGCR_LCS_2 | GFXTIM_CGCR_LCS_1 | GFXTIM_CGCR_LCS_0)
/**
  * @}
  */

/** @defgroup GFXTIM_FCCHwReloadSrc GFXTIM Frame Clock Counter Hardware Reload source
  * @{
  */
#define GFXTIM_FCC_HW_RELOAD_SRC_NONE           0U
#define GFXTIM_FCC_HW_RELOAD_SRC_LCC_UNDERFLOW  GFXTIM_CGCR_FCCHRS_0
#define GFXTIM_FCC_HW_RELOAD_SRC_HSYNC_RISING   GFXTIM_CGCR_FCCHRS_1
#define GFXTIM_FCC_HW_RELOAD_SRC_HSYNC_FALLING  (GFXTIM_CGCR_FCCHRS_0 | GFXTIM_CGCR_FCCHRS_1)
#define GFXTIM_FCC_HW_RELOAD_SRC_VSYNC_RISING   GFXTIM_CGCR_FCCHRS_2
#define GFXTIM_FCC_HW_RELOAD_SRC_VSYNC_FALLING  (GFXTIM_CGCR_FCCHRS_2 | GFXTIM_CGCR_FCCHRS_0)
#define GFXTIM_FCC_HW_RELOAD_SRC_TE_RISING      (GFXTIM_CGCR_FCCHRS_2 | GFXTIM_CGCR_FCCHRS_1)
#define GFXTIM_FCC_HW_RELOAD_SRC_TE_FALLING     (GFXTIM_CGCR_FCCHRS_2 | GFXTIM_CGCR_FCCHRS_1 | GFXTIM_CGCR_FCCHRS_0)
/**
  * @}
  */

/** @defgroup GFXTIM_FCCClockSrc GFXTIM Frame CLock Counter Clock Source
  * @{
  */
#define GFXTIM_FCC_CLK_SRC_DISABLE         0U
#define GFXTIM_FCC_CLK_SRC_LCC_UNDERFLOW   GFXTIM_CGCR_FCCCS_0
#define GFXTIM_FCC_CLK_SRC_HSYNC_RISING    GFXTIM_CGCR_FCCCS_1
#define GFXTIM_FCC_CLK_SRC_HSYNC_FALLING   (GFXTIM_CGCR_FCCCS_0 | GFXTIM_CGCR_FCCCS_1)
#define GFXTIM_FCC_CLK_SRC_VSYNC_RISING    GFXTIM_CGCR_FCCCS_2
#define GFXTIM_FCC_CLK_SRC_VSYNC_FALLING   (GFXTIM_CGCR_FCCCS_2 | GFXTIM_CGCR_FCCCS_0)
#define GFXTIM_FCC_CLK_SRC_TE_RISING       (GFXTIM_CGCR_FCCCS_2 | GFXTIM_CGCR_FCCCS_1)
#define GFXTIM_FCC_CLK_SRC_TE_FALLING      (GFXTIM_CGCR_FCCCS_2 | GFXTIM_CGCR_FCCCS_1 | GFXTIM_CGCR_FCCCS_0)
/**
  * @}
  */

/** @defgroup GFXTIM_FrameClockSrc GFXTIM GFXTIM  Frame Clock Source
  * @{
  */
#define GFXTIM_FRAME_CLK_SRC_LCC_UNDERFLOW   0U
#define GFXTIM_FRAME_CLK_SRC_FCC_UNDERFLOW   GFXTIM_CGCR_FCS_0
#define GFXTIM_FRAME_CLK_SRC_HSYNC_RISING    GFXTIM_CGCR_FCS_1
#define GFXTIM_FRAME_CLK_SRC_HSYNC_FALLING   (GFXTIM_CGCR_FCS_0 | GFXTIM_CGCR_FCS_1)
#define GFXTIM_FRAME_CLK_SRC_VSYNC_RISING    GFXTIM_CGCR_FCS_2
#define GFXTIM_FRAME_CLK_SRC_VSYNC_FALLING   (GFXTIM_CGCR_FCS_2 | GFXTIM_CGCR_FCS_0)
#define GFXTIM_FRAME_CLK_SRC_TE_RISING       (GFXTIM_CGCR_FCS_2 | GFXTIM_CGCR_FCS_1)
#define GFXTIM_FRAME_CLK_SRC_TE_FALLING      (GFXTIM_CGCR_FCS_2 | GFXTIM_CGCR_FCS_1 | GFXTIM_CGCR_FCS_0)
/**
  * @}
  */

/** @defgroup GFXTIM_LineClockCalib GFXTIM Line Clock Calibration Output
  * @{
  */
#define GFXTIM_LINE_CLK_CALIB_DISABLE   0U
#define GFXTIM_LINE_CLK_CALIB_ENABLE    GFXTIM_CR_LCCOE
/**
  * @}
  */

/** @defgroup GFXTIM_FrameClockCalib GFXTIM Frame Clock Calibration Output (for debug purpose)
  * @{
  */
#define GFXTIM_FRAME_CLK_CALIB_DISABLE   0U
#define GFXTIM_FRAME_CLK_CALIB_ENABLE    GFXTIM_CR_FCCOE
/**
  * @}
  */

/** @defgroup GFXTIM_ClockGeneratorCounter GFXTIM  Clock Generator Counter
  * @{
  */
#define GFXTIM_LINE_CLK_COUNTER     GFXTIM_CGCR_LCCFR
#define GFXTIM_FRAME_CLK_COUNTER    GFXTIM_CGCR_FCCFR
/**
  * @}
  */

/** @defgroup GFXTIM_AbsoluteTime GFXTIM Absolute Time
  * @{
  */
#define GFXTIM_ABSOLUTE_GLOBAL_TIME   0x00000014U
#define GFXTIM_ABSOLUTE_FRAME_TIME    0x00000015U
#define GFXTIM_ABSOLUTE_LINE_TIME     0x00000016U
/**
  * @}
  */

/** @defgroup GFXTIM_AbsoluteLineComparator GFXTIM Absolute Line Comparator
  * @{
  */
#define GFXTIM_ABSOLUTE_LINE_COMPARE1   0x1CU
#define GFXTIM_ABSOLUTE_LINE_COMPARE2   0x1DU
/**
  * @}
  */


/** @defgroup GFXTIM_RelativeCounterMode GFXTIM Relative Frame Counter Mode
  * @{
  */
#define GFXTIM_MODE_ONE_SHOT          0U
#define GFXTIM_MODE_CONTINUOUS        1U
/**
  * @}
  */

/** @defgroup GFXTIM_RelativeTimer GFXTIM Relative Timer
  * @{
  */
#define GFXTIM_RELATIVE_TIMER1          0U
#define GFXTIM_RELATIVE_TIMER2          1U
/**
  * @}
  */

/** @defgroup GFXTIM_EventLineSrc GFXTIM Event generator Line source selection
  * @{
  */
#define GFXTIM_LINE_EVENT_NONE            (0U << GFXTIM_EVSR_LES1_Pos)
#define GFXTIM_LINE_EVENT_ALC_OVERFLOW    (1U << GFXTIM_EVSR_LES1_Pos)
#define GFXTIM_LINE_EVENT_TE              (2U << GFXTIM_EVSR_LES1_Pos)
#define GFXTIM_LINE_EVENT_ALC1_COMPARE    (4U << GFXTIM_EVSR_LES1_Pos)
#define GFXTIM_LINE_EVENT_ALC2_COMPARE    (5U << GFXTIM_EVSR_LES1_Pos)
/**
  * @}
  */

/** @defgroup GFXTIM_EventFrameSrc GFXTIM Event generator Frame Source selection
  * @{
  */
#define GFXTIM_FRAME_EVENT_NONE            (0U << GFXTIM_EVSR_FES1_Pos )
#define GFXTIM_FRAME_EVENT_AFC_OVERFLOW    (1U << GFXTIM_EVSR_FES1_Pos )
#define GFXTIM_FRAME_EVENT_AFC_COMPARE     (2U << GFXTIM_EVSR_FES1_Pos )
#define GFXTIM_FRAME_EVENT_RFC1_RELOAD     (4U << GFXTIM_EVSR_FES1_Pos )
#define GFXTIM_FRAME_EVENT_RFC2_RELOAD     (5U << GFXTIM_EVSR_FES1_Pos )
/**
  * @}
  */

/** @defgroup GFXTIM_EventGenerator GFXTIM Event Generator ID
  * @{
  */
#define GFXTIM_EVENT_GENERATOR_1      0U
#define GFXTIM_EVENT_GENERATOR_2      1U
#define GFXTIM_EVENT_GENERATOR_3      2U
#define GFXTIM_EVENT_GENERATOR_4      3U
/**
  * @}
  */


/** @defgroup GFXTIM_WatchdogHwReloadConfig GFXTIM Watchdog hardware reload configuration
  * @{
  */
#define GFXTIM_WATCHDOG_HW_RELOAD_DISABLE           (0U  <<  GFXTIM_WDGTCR_WDGHRC_Pos)
#define GFXTIM_WATCHDOG_HW_RELOAD_RISING_EDGE       (1U  <<  GFXTIM_WDGTCR_WDGHRC_Pos)
#define GFXTIM_WATCHDOG_HW_RELOAD_FALLING_EDGE      (2U  <<  GFXTIM_WDGTCR_WDGHRC_Pos)
/**
  * @}
  */

/** @defgroup GFXTIM_WatchdogClockSrc GFXTIM Watchdog clock source
  * @{
  */
#define GFXTIM_WATCHDOG_CLK_SRC_LINE_CLK       (0U  <<  GFXTIM_WDGTCR_WDGCS_Pos)
#define GFXTIM_WATCHDOG_CLK_SRC_FRAME_CLK      (1U  <<  GFXTIM_WDGTCR_WDGCS_Pos)
#define GFXTIM_WATCHDOG_CLK_SRC_HSYNC_RISING   (2U  <<  GFXTIM_WDGTCR_WDGCS_Pos)
#define GFXTIM_WATCHDOG_CLK_SRC_HSYNC_FALLING  (3U  <<  GFXTIM_WDGTCR_WDGCS_Pos)
#define GFXTIM_WATCHDOG_CLK_SRC_VSYNC_RISING   (4U  <<  GFXTIM_WDGTCR_WDGCS_Pos)
#define GFXTIM_WATCHDOG_CLK_SRC_VSYNC_FALLING  (5U  <<  GFXTIM_WDGTCR_WDGCS_Pos)
#define GFXTIM_WATCHDOG_CLK_SRC_TE_RISING      (6U  <<  GFXTIM_WDGTCR_WDGCS_Pos)
#define GFXTIM_WATCHDOG_CLK_SRC_TE_FALLING     (7U  <<  GFXTIM_WDGTCR_WDGCS_Pos)
#define GFXTIM_WATCHDOG_CLK_SRC_EVENT_1        (8U  <<  GFXTIM_WDGTCR_WDGCS_Pos)
#define GFXTIM_WATCHDOG_CLK_SRC_EVENT_2        (9U  <<  GFXTIM_WDGTCR_WDGCS_Pos)
#define GFXTIM_WATCHDOG_CLK_SRC_EVENT_3        (10U <<  GFXTIM_WDGTCR_WDGCS_Pos)
#define GFXTIM_WATCHDOG_CLK_SRC_EVENT_4        (11U <<  GFXTIM_WDGTCR_WDGCS_Pos)
/**
  * @}
  */

/** @defgroup GFXTIM_Flag GFXTIM flags
  * @{
  */
#define GFXTIM_FLAG_AFCO  GFXTIM_ISR_AFCOF
#define GFXTIM_FLAG_ALCO  GFXTIM_ISR_ALCOF
#define GFXTIM_FLAG_TE    GFXTIM_ISR_TEF
#define GFXTIM_FLAG_AFCC1 GFXTIM_ISR_AFCC1F
#define GFXTIM_FLAG_ALCC1 GFXTIM_ISR_ALCC1F
#define GFXTIM_FLAG_ALCC2 GFXTIM_ISR_ALCC2F
#define GFXTIM_FLAG_RFC1R GFXTIM_ISR_RFC1RF
#define GFXTIM_FLAG_RFC2R GFXTIM_ISR_RFC2RF
#define GFXTIM_FLAG_EV1   GFXTIM_ISR_EV1F
#define GFXTIM_FLAG_EV2   GFXTIM_ISR_EV2F
#define GFXTIM_FLAG_EV3   GFXTIM_ISR_EV3F
#define GFXTIM_FLAG_EV4   GFXTIM_ISR_EV4F
#define GFXTIM_FLAG_WDGA  GFXTIM_ISR_WDGAF
#define GFXTIM_FLAG_WDGP  GFXTIM_ISR_WDGPF
/**
  * @}
  */

/**
  * @}
  */


/* Private macros ------------------------------------------------------------*/
/** @defgroup GFXTIM_Private_Macros  GFXTIM Private Macros
  * @{
  */
#define IS_GFXTIM_INTERRUPT(PARAM) (((PARAM) == GFXTIM_IT_ENABLE  ) || \
                                    ((PARAM) == GFXTIM_IT_DISABLE ))

#define IS_GFXTIM_SYNC_SRC(PARAM) (((PARAM) == GFXTIM_SYNC_SRC_HSYNC_VSYNC_0) || \
                                   ((PARAM) == GFXTIM_SYNC_SRC_HSYNC_VSYNC_1) || \
                                   ((PARAM) == GFXTIM_SYNC_SRC_HSYNC_VSYNC_2) || \
                                   ((PARAM) == GFXTIM_SYNC_SRC_HSYNC_VSYNC_3))

#define IS_GFXTIM_TE_SRC(PARAM) (((PARAM) == GFXTIM_TE_SRC_GPIO ) || \
                                 ((PARAM) == GFXTIM_TE_SRC_ITE       ) || \
                                 ((PARAM) == GFXTIM_TE_SRC_HSYNC     ) || \
                                 ((PARAM) == GFXTIM_TE_SRC_VSYNC     ))

#define IS_GFXTIM_TE_POLARITY(PARAM) (((PARAM) == GFXTIM_TE_RISING_EDGE  ) || \
                                      ((PARAM) == GFXTIM_TE_FALLING_EDGE ))

#define IS_GFXTIM_LCC_HW_RELOAD_SRC(PARAM) (((PARAM) == GFXTIM_LCC_HW_RELOAD_SRC_NONE         ) || \
                                            ((PARAM) == GFXTIM_LCC_HW_RELOAD_SRC_FCC_UNDERFLOW) || \
                                            ((PARAM) == GFXTIM_LCC_HW_RELOAD_SRC_HSYNC_RISING ) || \
                                            ((PARAM) == GFXTIM_LCC_HW_RELOAD_SRC_HSYNC_FALLING) || \
                                            ((PARAM) == GFXTIM_LCC_HW_RELOAD_SRC_VSYNC_RISING ) || \
                                            ((PARAM) == GFXTIM_LCC_HW_RELOAD_SRC_VSYNC_FALLING) || \
                                            ((PARAM) == GFXTIM_LCC_HW_RELOAD_SRC_TE_RISING    ) || \
                                            ((PARAM) == GFXTIM_LCC_HW_RELOAD_SRC_TE_FALLING    ))

#define IS_GFXTIM_LCC_CLK_SRC(PARAM) (((PARAM) == GFXTIM_LCC_CLK_SRC_DISABLE) || \
                                      ((PARAM) == GFXTIM_LCC_CLK_SRC_SYSCLOCK))

#define IS_GFXTIM_LINE_CLK_SRC(PARAM)      (((PARAM) == GFXTIM_LINE_CLK_SRC_LCC_UNDERFLOW) || \
                                            ((PARAM) == GFXTIM_LINE_CLK_SRC_FCC_UNDERFLOW) || \
                                            ((PARAM) == GFXTIM_LINE_CLK_SRC_HSYNC_RISING ) || \
                                            ((PARAM) == GFXTIM_LINE_CLK_SRC_HSYNC_FALLING) || \
                                            ((PARAM) == GFXTIM_LINE_CLK_SRC_VSYNC_RISING ) || \
                                            ((PARAM) == GFXTIM_LINE_CLK_SRC_VSYNC_FALLING) || \
                                            ((PARAM) == GFXTIM_LINE_CLK_SRC_TE_RISING) || \
                                            ((PARAM) == GFXTIM_LINE_CLK_SRC_TE_FALLING))

#define IS_GFXTIM_FCC_HW_RELOAD_SRC(PARAM) (((PARAM) == GFXTIM_FCC_HW_RELOAD_SRC_NONE) || \
                                            ((PARAM) == GFXTIM_FCC_HW_RELOAD_SRC_LCC_UNDERFLOW) || \
                                            ((PARAM) == GFXTIM_FCC_HW_RELOAD_SRC_HSYNC_RISING ) || \
                                            ((PARAM) == GFXTIM_FCC_HW_RELOAD_SRC_HSYNC_FALLING) || \
                                            ((PARAM) == GFXTIM_FCC_HW_RELOAD_SRC_VSYNC_RISING ) || \
                                            ((PARAM) == GFXTIM_FCC_HW_RELOAD_SRC_VSYNC_FALLING) || \
                                            ((PARAM) == GFXTIM_FCC_HW_RELOAD_SRC_TE_RISING) || \
                                            ((PARAM) == GFXTIM_FCC_HW_RELOAD_SRC_TE_FALLING))

#define IS_GFXTIM_FCC_CLK_SRC(PARAM) (((PARAM) == GFXTIM_FCC_CLK_SRC_DISABLE) || \
                                      ((PARAM) == GFXTIM_FCC_CLK_SRC_LCC_UNDERFLOW) || \
                                      ((PARAM) == GFXTIM_FCC_CLK_SRC_HSYNC_RISING ) || \
                                      ((PARAM) == GFXTIM_FCC_CLK_SRC_HSYNC_FALLING) || \
                                      ((PARAM) == GFXTIM_FCC_CLK_SRC_VSYNC_RISING ) || \
                                      ((PARAM) == GFXTIM_FCC_CLK_SRC_VSYNC_FALLING) || \
                                      ((PARAM) == GFXTIM_FCC_CLK_SRC_TE_RISING) || \
                                      ((PARAM) == GFXTIM_FCC_CLK_SRC_TE_FALLING))

#define IS_GFXTIM_FRAME_CLK_SRC(PARAM) (((PARAM) == GFXTIM_FRAME_CLK_SRC_LCC_UNDERFLOW) || \
                                        ((PARAM) == GFXTIM_FRAME_CLK_SRC_FCC_UNDERFLOW) || \
                                        ((PARAM) == GFXTIM_FRAME_CLK_SRC_HSYNC_RISING ) || \
                                        ((PARAM) == GFXTIM_FRAME_CLK_SRC_HSYNC_FALLING) || \
                                        ((PARAM) == GFXTIM_FRAME_CLK_SRC_VSYNC_RISING ) || \
                                        ((PARAM) == GFXTIM_FRAME_CLK_SRC_VSYNC_FALLING) || \
                                        ((PARAM) == GFXTIM_FRAME_CLK_SRC_TE_RISING) || \
                                        ((PARAM) == GFXTIM_FRAME_CLK_SRC_TE_FALLING))

#define IS_GFXTIM_LINE_CLK_CALIB(PARAM) (((PARAM) == GFXTIM_LINE_CLK_CALIB_DISABLE) || \
                                         ((PARAM) == GFXTIM_LINE_CLK_CALIB_ENABLE))

#define IS_GFXTIM_FRAME_CLK_CALIB(PARAM) (((PARAM) == GFXTIM_FRAME_CLK_CALIB_DISABLE) || \
                                          ((PARAM) == GFXTIM_FRAME_CLK_CALIB_ENABLE))

#define IS_GFXTIM_CLOCK_GENERATOR_COUNTER(PARAM) (((PARAM) == GFXTIM_LINE_CLK_COUNTER) || \
                                                  ((PARAM) == GFXTIM_FRAME_CLK_COUNTER) || \
                                                  ((PARAM) == (GFXTIM_LINE_CLK_COUNTER | \
                                                               GFXTIM_FRAME_CLK_COUNTER)))

#define IS_GFXTIM_ABSOLUTE_TIME(PARAM) (((PARAM) == GFXTIM_ABSOLUTE_GLOBAL_TIME) || \
                                        ((PARAM) == GFXTIM_ABSOLUTE_FRAME_TIME) || \
                                        ((PARAM) == GFXTIM_ABSOLUTE_LINE_TIME))

#define IS_GFXTIM_ABSOLUTE_LINE_COMPARATOR(PARAM) (((PARAM) == GFXTIM_ABSOLUTE_LINE_COMPARE1) || \
                                                   ((PARAM) == GFXTIM_ABSOLUTE_LINE_COMPARE2))

#define IS_GFXTIM_RELATIVE_TIMER(PARAM) (((PARAM) == GFXTIM_RELATIVE_TIMER1) || \
                                         ((PARAM) == GFXTIM_RELATIVE_TIMER2))

#define IS_GFXTIM_RELATIVE_COUNTER_MODE(PARAM) (((PARAM) == GFXTIM_MODE_ONE_SHOT) || \
                                                ((PARAM) == GFXTIM_MODE_CONTINUOUS))

#define IS_GFXTIM_EVENT_LINE(PARAM) (((PARAM) == GFXTIM_LINE_EVENT_NONE) || \
                                     ((PARAM) == GFXTIM_LINE_EVENT_ALC_OVERFLOW) || \
                                     ((PARAM) == GFXTIM_LINE_EVENT_TE) || \
                                     ((PARAM) == GFXTIM_LINE_EVENT_ALC1_COMPARE) || \
                                     ((PARAM) == GFXTIM_LINE_EVENT_ALC2_COMPARE))

#define IS_GFXTIM_EVENT_FRAME(PARAM) (((PARAM) == GFXTIM_FRAME_EVENT_NONE) || \
                                      ((PARAM) == GFXTIM_FRAME_EVENT_AFC_OVERFLOW) || \
                                      ((PARAM) == GFXTIM_FRAME_EVENT_AFC_COMPARE) || \
                                      ((PARAM) == GFXTIM_FRAME_EVENT_RFC1_RELOAD) || \
                                      ((PARAM) == GFXTIM_FRAME_EVENT_RFC2_RELOAD))

#define IS_GFXTIM_EVENT_GENERATOR(PARAM) (((PARAM) == GFXTIM_EVENT_GENERATOR_1) || \
                                          ((PARAM) == GFXTIM_EVENT_GENERATOR_2) || \
                                          ((PARAM) == GFXTIM_EVENT_GENERATOR_3) || \
                                          ((PARAM) == GFXTIM_EVENT_GENERATOR_4))


#define IS_GFXTIM_CLOCK_GENERATOR_COUNTER_FORCE_RELOAD(PARAM) (((PARAM) == GFXTIM_LINE_CLK_COUNTER) || \
                                                               ((PARAM) == GFXTIM_FRAME_CLK_COUNTER) || \
                                                               ((PARAM) == (GFXTIM_LINE_CLK_COUNTER | \
                                                                            GFXTIM_FRAME_CLK_COUNTER)))


#define IS_GFXTIM_WATCHDOG_HW_RELOAD_CONFIG(PARAM) (((PARAM) == GFXTIM_WATCHDOG_HW_RELOAD_DISABLE) || \
                                                    ((PARAM) == GFXTIM_WATCHDOG_HW_RELOAD_RISING_EDGE) || \
                                                    ((PARAM) == GFXTIM_WATCHDOG_HW_RELOAD_FALLING_EDGE))

#define IS_GFXTIM_WATCHDOG_CLOCK_SRC(PARAM) (((PARAM) == GFXTIM_WATCHDOG_CLK_SRC_LINE_CLK) || \
                                             ((PARAM) == GFXTIM_WATCHDOG_CLK_SRC_FRAME_CLK) || \
                                             ((PARAM) == GFXTIM_WATCHDOG_CLK_SRC_HSYNC_RISING) || \
                                             ((PARAM) == GFXTIM_WATCHDOG_CLK_SRC_HSYNC_FALLING) || \
                                             ((PARAM) == GFXTIM_WATCHDOG_CLK_SRC_VSYNC_RISING) || \
                                             ((PARAM) == GFXTIM_WATCHDOG_CLK_SRC_VSYNC_FALLING) || \
                                             ((PARAM) == GFXTIM_WATCHDOG_CLK_SRC_TE_RISING) || \
                                             ((PARAM) == GFXTIM_WATCHDOG_CLK_SRC_TE_FALLING) || \
                                             ((PARAM) == GFXTIM_WATCHDOG_CLK_SRC_EVENT_1) || \
                                             ((PARAM) == GFXTIM_WATCHDOG_CLK_SRC_EVENT_2) || \
                                             ((PARAM) == GFXTIM_WATCHDOG_CLK_SRC_EVENT_3) || \
                                             ((PARAM) == GFXTIM_WATCHDOG_CLK_SRC_EVENT_4))
#define IS_GFXTIM_WATCHDOG_VALUE(PARAM) ((PARAM) <= 65535U)
#define IS_GFXTIM_RELATIVE_FRAME_VALUE(PARAM) ((PARAM) <= 4095U)
#define IS_GFXTIM_ABSOLUTE_FRAME_VALUE(PARAM) ((PARAM) <= 1048575U)
#define IS_GFXTIM_ABSOLUTE_LINE_VALUE(PARAM) ((PARAM) <= 4095U)
#define IS_GFXTIM_LCC_RELOAD_VALUE(PARAM) ((PARAM) <= 4194303U)
#define IS_GFXTIM_FCC_RELOAD_VALUE(PARAM) ((PARAM) <= 4095U)


/**
  * @}
  */


/* Exported macro ------------------------------------------------------------*/
/** @defgroup GFXTIM_Exported_Macros  GFXTIM Exported Macros
  * @{
  */

/** @brief  Reset GFXTIM handle state.
  * @param  __HANDLE__ GFXTIM handle.
  * @retval None
  */
#if (USE_HAL_GFXTIM_REGISTER_CALLBACKS == 1)
#define __HAL_GFXTIM_RESET_HANDLE_STATE(__HANDLE__) do{                                             \
                                                        (__HANDLE__)->State = HAL_GFXTIM_STATE_RESET; \
                                                        (__HANDLE__)->MspInitCallback = NULL;      \
                                                        (__HANDLE__)->MspDeInitCallback = NULL;    \
                                                      } while(0)
#else /* USE_HAL_GFXTIM_REGISTER_CALLBACKS */
#define __HAL_GFXTIM_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_GFXTIM_STATE_RESET)
#endif /* USE_HAL_GFXTIM_REGISTER_CALLBACKS */



/**
  * @brief Check whether the specified GFXTIM flag is set or not.
  * @param __HANDLE__ GFXTIM handle
  * @param __FLAG__ GFXTIM flag
  *        This parameter can be one or a combination of the following values:
  *            @arg @ref GFXTIM_FLAG_AFCO   Absolute Frame Counter Overflow Flag
  *            @arg @ref GFXTIM_FLAG_ALCO   Absolute Line Counter Overflow Flag
  *            @arg @ref GFXTIM_FLAG_TE     Tearing Effect Flag
  *            @arg @ref GFXTIM_FLAG_AFCC1  Absolute Frame Counter Compare 1 Flag
  *            @arg @ref GFXTIM_FLAG_ALCC1  Absolute Line Counter Compare 1 Flag
  *            @arg @ref GFXTIM_FLAG_ALCC2  Absolute Line Counter Compare 2 Flag
  *            @arg @ref GFXTIM_FLAG_RFC1R  Relative Frame Counter 1 Reload Flag
  *            @arg @ref GFXTIM_FLAG_RFC2R  Relative Frame Counter 2 Reload Flag
  *            @arg @ref GFXTIM_FLAG_EV1    Event 1 Flag
  *            @arg @ref GFXTIM_FLAG_EV2    Event 2 Flag
  *            @arg @ref GFXTIM_FLAG_EV3    Event 3 Flag
  *            @arg @ref GFXTIM_FLAG_EV4    Event 4 Flag
  *            @arg @ref GFXTIM_FLAG_WDGA   Watchdog Alarm Flag
  *            @arg @ref GFXTIM_FLAG_WDGP   Watchdog Pre-alarm Flag
  * @retval State of flag (TRUE or FALSE).
  */
#define __HAL_GFXTIM_GET_FLAG(__HANDLE__, __FLAG__)\
  ((((__HANDLE__)->Instance->ISR) & (__FLAG__)) == (__FLAG__))

/**
  * @brief Clear the specified GFXTIM flag.
  * @param __HANDLE__ GFXTIM handle
  * @param __FLAG__ GFXTIM flag
  *        This parameter can be one or a combination of the following values:
  *            @arg @ref GFXTIM_FLAG_AFCO   Absolute Frame Counter Overflow Flag
  *            @arg @ref GFXTIM_FLAG_ALCO   Absolute Line Counter Overflow Flag
  *            @arg @ref GFXTIM_FLAG_TE     Tearing Effect Flag
  *            @arg @ref GFXTIM_FLAG_AFCC1  Absolute Frame Counter Compare 1 Flag
  *            @arg @ref GFXTIM_FLAG_ALCC1  Absolute Line Counter Compare 1 Flag
  *            @arg @ref GFXTIM_FLAG_ALCC2  Absolute Line Counter Compare 2 Flag
  *            @arg @ref GFXTIM_FLAG_RFC1R  Relative Frame Counter 1 Reload Flag
  *            @arg @ref GFXTIM_FLAG_RFC2R  Relative Frame Counter 2 Reload Flag
  *            @arg @ref GFXTIM_FLAG_EV1    Event 1 Flag
  *            @arg @ref GFXTIM_FLAG_EV2    Event 2 Flag
  *            @arg @ref GFXTIM_FLAG_EV3    Event 3 Flag
  *            @arg @ref GFXTIM_FLAG_EV4    Event 4 Flag
  *            @arg @ref GFXTIM_FLAG_WDGA   Watchdog Alarm Flag
  *            @arg @ref GFXTIM_FLAG_WDGP   Watchdog Pre-alarm Flag
  * @retval None
  */
#define __HAL_GFXTIM_CLEAR_FLAG(__HANDLE__, __FLAG__)\
  (((__HANDLE__)->Instance->ICR) = (__FLAG__))

/**
  * @}
  */


/* Exported functions --------------------------------------------------------*/
/** @addtogroup GFXTIM_Exported_Functions
  * @{
  */

/* Initialization and de-initialization functions  ****************************/
/** @addtogroup GFXTIM_Exported_Functions_Group1
  * @{
  */
HAL_StatusTypeDef HAL_GFXTIM_Init(GFXTIM_HandleTypeDef *hgfxtim);
HAL_StatusTypeDef HAL_GFXTIM_DeInit(GFXTIM_HandleTypeDef *hgfxtim);
void              HAL_GFXTIM_MspInit(GFXTIM_HandleTypeDef *hgfxtim);
void              HAL_GFXTIM_MspDeInit(GFXTIM_HandleTypeDef *hgfxtim);
#if (USE_HAL_GFXTIM_REGISTER_CALLBACKS == 1)
HAL_StatusTypeDef HAL_GFXTIM_RegisterCallback(GFXTIM_HandleTypeDef *hgfxtim,
                                              HAL_GFXTIM_CallbackIDTypeDef CallbackID,
                                              pGFXTIM_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_GFXTIM_UnRegisterCallback(GFXTIM_HandleTypeDef *hgfxtim,
                                                HAL_GFXTIM_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_GFXTIM_REGISTER_CALLBACKS */
void HAL_GFXTIM_TECallback(GFXTIM_HandleTypeDef *hgfxtim);
/**
  * @}
  */

/* Clock Generator functions  *****************************************************/
/** @addtogroup GFXTIM_Exported_Functions_Group2
  * @{
  */
HAL_StatusTypeDef HAL_GFXTIM_ClockGenerator_Config(GFXTIM_HandleTypeDef *hgfxtim,
                                                   const GFXTIM_ClockGeneratorConfigTypeDef *pClockGeneratorConfig);
HAL_StatusTypeDef HAL_GFXTIM_ClockGenerator_Reload(GFXTIM_HandleTypeDef *hgfxtim, uint32_t ClockGeneratorCounter);
/**
  * @}
  */

/* Absolute Timer functions  *****************************************/
/** @addtogroup GFXTIM_Exported_Functions_Group3
  * @{
  */
HAL_StatusTypeDef HAL_GFXTIM_AbsoluteTimer_Config(GFXTIM_HandleTypeDef *hgfxtim,
                                                  const GFXTIM_AbsoluteTimerConfigTypeDef *pAbsoluteTimerConfig);
HAL_StatusTypeDef HAL_GFXTIM_AbsoluteTimer_Start(GFXTIM_HandleTypeDef *hgfxtim);
HAL_StatusTypeDef HAL_GFXTIM_AbsoluteTimer_Stop(GFXTIM_HandleTypeDef *hgfxtim);
HAL_StatusTypeDef HAL_GFXTIM_AbsoluteTimer_Reset(GFXTIM_HandleTypeDef *hgfxtim);
HAL_StatusTypeDef HAL_GFXTIM_AbsoluteTimer_GetCounter(const GFXTIM_HandleTypeDef *hgfxtim, uint32_t AbsoluteTime,
                                                      uint32_t *pValue);
HAL_StatusTypeDef HAL_GFXTIM_AbsoluteTimer_SetFrameCompare(GFXTIM_HandleTypeDef *hgfxtim, uint32_t Value);
HAL_StatusTypeDef HAL_GFXTIM_AbsoluteTimer_SetLineCompare(GFXTIM_HandleTypeDef *hgfxtim,
                                                          uint32_t AbsoluteLineComparator,
                                                          uint32_t Value);
void HAL_GFXTIM_AbsoluteTimer_AFCC1Callback(GFXTIM_HandleTypeDef *hgfxtim);
void HAL_GFXTIM_AbsoluteTimer_AFCOFCallback(GFXTIM_HandleTypeDef *hgfxtim);
void HAL_GFXTIM_AbsoluteTimer_ALCC1Callback(GFXTIM_HandleTypeDef *hgfxtim);
void HAL_GFXTIM_AbsoluteTimer_ALCC2Callback(GFXTIM_HandleTypeDef *hgfxtim);
void HAL_GFXTIM_AbsoluteTimer_ALCOFCallback(GFXTIM_HandleTypeDef *hgfxtim);
/**
  * @}
  */

/* Relative Timer functions  *****************************************/
/** @addtogroup GFXTIM_Exported_Functions_Group4
  * @{
  */
HAL_StatusTypeDef HAL_GFXTIM_RelativeTimer_Config(GFXTIM_HandleTypeDef *hgfxtim,
                                                  const GFXTIM_RelativeTimerConfigTypeDef *pRelativeTimerConfig,
                                                  uint32_t RelativeTimer);
HAL_StatusTypeDef HAL_GFXTIM_RelativeTimer_Start(GFXTIM_HandleTypeDef *hgfxtim, uint32_t RelativeTimer);
HAL_StatusTypeDef HAL_GFXTIM_RelativeTimer_Stop(GFXTIM_HandleTypeDef *hgfxtim, uint32_t RelativeTimer);
HAL_StatusTypeDef HAL_GFXTIM_RelativeTimer_ForceReload(GFXTIM_HandleTypeDef *hgfxtim, uint32_t RelativeTimer);
HAL_StatusTypeDef HAL_GFXTIM_RelativeTimer_SetReload(GFXTIM_HandleTypeDef *hgfxtim, uint32_t RelativeTimer,
                                                     uint32_t Value);
HAL_StatusTypeDef HAL_GFXTIM_RelativeTimer_GetCounter(const GFXTIM_HandleTypeDef *hgfxtim, uint32_t RelativeTimer,
                                                      uint32_t *pValue);
void HAL_GFXTIM_RelativeTimer_RFC1RCallback(GFXTIM_HandleTypeDef *hgfxtim);
void HAL_GFXTIM_RelativeTimer_RFC2RCallback(GFXTIM_HandleTypeDef *hgfxtim);
/**
  * @}
  */

/* Event Generator functions  *****************************************/
/** @addtogroup GFXTIM_Exported_Functions_Group5
  * @{
  */
HAL_StatusTypeDef HAL_GFXTIM_EventGenerator_Config(GFXTIM_HandleTypeDef *hgfxtim, uint32_t EventGenerator,
                                                   const GFXTIM_EventGeneratorConfigTypeDef *pEventGeneratorConfig);
HAL_StatusTypeDef HAL_GFXTIM_EventGenerator_Enable(GFXTIM_HandleTypeDef *hgfxtim, uint32_t EventGenerator);
HAL_StatusTypeDef HAL_GFXTIM_EventGenerator_Disable(GFXTIM_HandleTypeDef *hgfxtim, uint32_t EventGenerator);
void HAL_GFXTIM_EventGenerator_EV1Callback(GFXTIM_HandleTypeDef *hgfxtim);
void HAL_GFXTIM_EventGenerator_EV2Callback(GFXTIM_HandleTypeDef *hgfxtim);
void HAL_GFXTIM_EventGenerator_EV3Callback(GFXTIM_HandleTypeDef *hgfxtim);
void HAL_GFXTIM_EventGenerator_EV4Callback(GFXTIM_HandleTypeDef *hgfxtim);
/**
  * @}
  */

/* Watchdog functions  *****************************************/
/** @addtogroup GFXTIM_Exported_Functions_Group6
  * @{
  */
HAL_StatusTypeDef HAL_GFXTIM_WatchdogTimer_Config(GFXTIM_HandleTypeDef *hgfxtim,
                                                  const GFXTIM_WatchdogConfigTypeDef *pWatchdogConfig);
HAL_StatusTypeDef HAL_GFXTIM_WatchdogTimer_Enable(GFXTIM_HandleTypeDef *hgfxtim);
HAL_StatusTypeDef HAL_GFXTIM_WatchdogTimer_Disable(GFXTIM_HandleTypeDef *hgfxtim);
HAL_StatusTypeDef HAL_GFXTIM_WatchdogTimer_Refresh(GFXTIM_HandleTypeDef *hgfxtim);
void HAL_GFXTIM_WatchdogTimer_AlarmCallback(GFXTIM_HandleTypeDef *hgfxtim);
void HAL_GFXTIM_WatchdogTimer_PreAlarmCallback(GFXTIM_HandleTypeDef *hgfxtim);
/**
  * @}
  */

/* Generic functions  *********************************************************/
/** @addtogroup GFXTIM_Exported_Functions_Group7
  * @{
  */
void                    HAL_GFXTIM_IRQHandler(GFXTIM_HandleTypeDef *hgfxtim);
void                    HAL_GFXTIM_ErrorCallback(GFXTIM_HandleTypeDef *hgfxtim);
uint32_t                HAL_GFXTIM_GetError(const GFXTIM_HandleTypeDef *hgfxtim);
HAL_GFXTIM_StateTypeDef HAL_GFXTIM_GetState(const GFXTIM_HandleTypeDef *hgfxtim);

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

#endif /* GFXTIM */

#ifdef __cplusplus
}
#endif

#endif /* STM32U5xx_HAL_GFXTIM_H */
