/*
 * Copyright (c) 2025 Croxel, Inc.
 * Copyright (c) 2025 CogniPilot Foundation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_DRIVERS_SENSOR_PNI_RM3100_REG_H_
#define ZEPHYR_DRIVERS_SENSOR_PNI_RM3100_REG_H_

/* Address value has a read bit */
#define REG_READ_BIT BIT(7)

/* RM3100 register addresses */
#define RM3100_REG_CMM         0x01
#define RM3100_REG_CCX_MSB     0x04
#define RM3100_REG_CCX_LSB     0x05
#define RM3100_REG_CCY_MSB     0x06
#define RM3100_REG_CCY_LSB     0x07
#define RM3100_REG_CCZ_MSB     0x08
#define RM3100_REG_CCZ_LSB     0x09
#define RM3100_REG_TMRC        0x0B
#define RM3100_REG_MX          0x24
#define RM3100_REG_MY          0x27
#define RM3100_REG_MZ          0x2A
#define RM3100_REG_STATUS      0x34
#define RM3100_REG_REVID       0x36

/* Default values */
#define RM3100_REVID_VALUE     0x22

#define RM3100_CMM_ALL_AXIS 0x71

#define RM3100_CYCLE_COUNT_DEFAULT 0x00C8
#define RM3100_CYCLE_COUNT_HIGH_ODR 0x0064


#endif /* ZEPHYR_DRIVERS_SENSOR_PNI_RM3100_REG_H_ */
