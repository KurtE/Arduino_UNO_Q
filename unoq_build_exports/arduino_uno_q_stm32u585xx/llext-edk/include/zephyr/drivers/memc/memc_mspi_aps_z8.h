/*
 * Copyright (c) 2025, Ambiq Micro Inc. <www.ambiq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

enum memc_mspi_aps_z8_rlc {
	MEMC_MSPI_APS_Z8_RLC_4,
	MEMC_MSPI_APS_Z8_RLC_5,
	MEMC_MSPI_APS_Z8_RLC_6,
	MEMC_MSPI_APS_Z8_RLC_7,
	MEMC_MSPI_APS_Z8_RLC_8,
	MEMC_MSPI_APS_Z8_RLC_9,
};

enum memc_mspi_aps_z8_wlc {
	MEMC_MSPI_APS_Z8_WLC_3,
	MEMC_MSPI_APS_Z8_WLC_7,
	MEMC_MSPI_APS_Z8_WLC_5,
	MEMC_MSPI_APS_Z8_WLC_9,
	MEMC_MSPI_APS_Z8_WLC_4,
	MEMC_MSPI_APS_Z8_WLC_8,
	MEMC_MSPI_APS_Z8_WLC_6,
	MEMC_MSPI_APS_Z8_WLC_10,
};

/* default for APS51216BA */
#define MEMC_MSPI_APS_Z8_RX_DUMMY_DEFAULT 6
#define MEMC_MSPI_APS_Z8_TX_DUMMY_DEFAULT 6

#define MEMC_MSPI_APS_Z8_CMD_LENGTH_DEFAULT  1
#define MEMC_MSPI_APS_Z8_ADDR_LENGTH_DEFAULT 4

enum memc_mspi_aps_z8_ds {
	MEMC_MSPI_APS_Z8_DRIVE_STRENGTH_FULL,
	MEMC_MSPI_APS_Z8_DRIVE_STRENGTH_HALF,
	MEMC_MSPI_APS_Z8_DRIVE_STRENGTH_QUARTER,
	MEMC_MSPI_APS_Z8_DRIVE_STRENGTH_OCTUPLE,
};

struct memc_mspi_aps_z8_reg {
	union {
		uint8_t MR0;

		struct {
			uint8_t DS:         2;
			uint8_t RLC:        3;
			uint8_t LT:         1;
			uint8_t:            1;
			uint8_t TSO:        1;
		} MR0_b;
	};

	union {
		uint8_t MR1;

		struct {
			uint8_t VID:        5;
			uint8_t:            2;
			uint8_t ULP:        1;
		} MR1_b;
	};

	union {
		uint8_t MR2;

		struct {
			uint8_t DENSITY:    3;
			uint8_t GENERATION: 2;
			uint8_t GB:         3;
		} MR2_b;
	};

	union {
		uint8_t MR3;

		struct {
			uint8_t:            4;
			uint8_t SRF:        2;
			uint8_t:            1;
			uint8_t RBXen:      1;
		} MR3_b;
	};

	union {
		uint8_t MR4;

		struct {
			uint8_t PASR:       3;
			uint8_t RFS:        2;
			uint8_t WLC:        3;
		} MR4_b;
	};

	union {
		uint8_t MR6;

		struct {
			uint8_t ULPM:       8;
		} MR6_b;
	};

	union {
		uint8_t MR8;

		struct {
			uint8_t BL:         2;
			uint8_t BT:         1;
			uint8_t RBX:        1;
			uint8_t:            2;
			uint8_t IOM:        1;
			uint8_t:            1;
		} MR8_b;
	};
};
