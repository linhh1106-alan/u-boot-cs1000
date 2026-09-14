/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2022 ARM Limited
 * Abdellatif El Khlifi <abdellatif.elkhlifi@arm.com>
 */

#ifndef __ARM_FFA_PRV_H
#define __ARM_FFA_PRV_H

#include <arm_ffa.h>
#include <linux/bitfield.h>
#include <linux/bitops.h>
#include <linux/arm-smccc.h>

/*
 * This header is private. It is exclusively used by the FF-A driver
 */

/* FF-A core driver name */
#define FFA_DRV_NAME "arm_ffa"

/* The FF-A SMC function definitions */

#if CONFIG_IS_ENABLED(SANDBOX_FFA)
#include "sandbox_arm_ffa.h"
#else
typedef struct arm_smccc_1_2_regs ffa_value_t;
#endif

typedef void (*invoke_ffa_fn_t)(ffa_value_t args, ffa_value_t *res);

/* FF-A driver version definitions */

#define MAJOR_VERSION_MASK		GENMASK(30, 16)
#define MINOR_VERSION_MASK		GENMASK(15, 0)
#define GET_FFA_MAJOR_VERSION(x)		\
				((u16)(FIELD_GET(MAJOR_VERSION_MASK, (x))))
#define GET_FFA_MINOR_VERSION(x)		\
				((u16)(FIELD_GET(MINOR_VERSION_MASK, (x))))
#define PACK_VERSION_INFO(major, minor)			\
	(FIELD_PREP(MAJOR_VERSION_MASK, (major)) |	\
	 FIELD_PREP(MINOR_VERSION_MASK, (minor)))

#define FFA_MAJOR_VERSION		(1)
#define FFA_MINOR_VERSION		(0)
#define FFA_VERSION_1_0		\
			PACK_VERSION_INFO(FFA_MAJOR_VERSION, FFA_MINOR_VERSION)

/* Endpoint ID mask (u-boot endpoint ID) */

#define GET_SELF_ENDPOINT_ID_MASK		GENMASK(15, 0)
#define GET_SELF_ENDPOINT_ID(x)		\
			((u16)(FIELD_GET(GET_SELF_ENDPOINT_ID_MASK, (x))))

#define PREP_SELF_ENDPOINT_ID_MASK		GENMASK(31, 16)
#define PREP_SELF_ENDPOINT_ID(x)		\
			(FIELD_PREP(PREP_SELF_ENDPOINT_ID_MASK, (x)))

/* Partition endpoint ID mask  (partition with which u-boot communicates with) */

#define PREP_PART_ENDPOINT_ID_MASK		GENMASK(15, 0)
#define PREP_PART_ENDPOINT_ID(x)		\
			(FIELD_PREP(PREP_PART_ENDPOINT_ID_MASK, (x)))

/*
 * Definitions of the Arm FF-A interfaces supported by the Arm FF-A driver
 */

#define FFA_SMC(calling_convention, func_num)				\
	ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL, (calling_convention),	\
			   ARM_SMCCC_OWNER_STANDARD, (func_num))

#define FFA_SMC_32(func_num)				FFA_SMC(ARM_SMCCC_SMC_32, (func_num))
#define FFA_SMC_64(func_num)				FFA_SMC(ARM_SMCCC_SMC_64, (func_num))

enum ffa_abis {
	FFA_ERROR					= 0x60,
	FFA_SUCCESS					= 0x61,
	FFA_INTERRUPT				= 0x62,
	FFA_VERSION					= 0x63,
	FFA_FEATURES				= 0x64,
	FFA_RX_RELEASE				= 0x65,
	FFA_RXTX_MAP				= 0x66,
	FFA_RXTX_UNMAP			= 0x67,
	FFA_PARTITION_INFO_GET		= 0x68,
	FFA_ID_GET					= 0x69,
	FFA_RUN						= 0x6D,
	FFA_MSG_SEND_DIRECT_REQ	= 0x6F,
	FFA_MSG_SEND_DIRECT_RESP	= 0x70,

	/* to be updated when adding new FFA IDs */
	FFA_FIRST_ID = FFA_ERROR, /* lowest number ID*/
	FFA_LAST_ID = FFA_MSG_SEND_DIRECT_RESP, /* highest number ID*/
};

/* number of the errors supported by the FF-A specification */
#define MAX_NUMBER_FFA_ERR 9

/* container structure and helper macros to map between an FF-A error and relevant error log */
struct ffa_abi_errmap {
	char *err_str[MAX_NUMBER_FFA_ERR];
};

#define FFA_ERRMAP_COUNT (FFA_LAST_ID - FFA_FIRST_ID + 1)
#define FFA_ID_TO_ERRMAP_ID(ffa_id) ((ffa_id) - FFA_FIRST_ID)

/*
 * struct ffa_partition_uuid - 16 bytes UUID transmitted by FFA_PARTITION_INFO_GET
 * @a1-4:	32-bit words access to the UUID data
 *
 */
struct ffa_partition_uuid {
	u32 a1; /* w1 */
	u32 a2; /* w2 */
	u32 a3; /* w3 */
	u32 a4; /* w4 */
};

/**
 * enum ffa_rxtx_buf_sizes - minimum sizes supported
 * for the RX/TX buffers
 */
enum ffa_rxtx_buf_sizes {
	RXTX_4K,
	RXTX_64K,
	RXTX_16K
};

/**
 * struct ffa_rxtxpair - structure hosting the RX/TX buffers virtual addresses
 * @rxbuf:	virtual address of the RX buffer
 * @txbuf:	virtual address of the TX buffer
 * @rxtx_min_pages:	RX/TX buffers minimum size in pages
 *
 * Data structure hosting the virtual addresses of the mapped RX/TX buffers
 * These addresses are used by the FF-A functions that use the RX/TX buffers
 */
struct ffa_rxtxpair {
	u64 rxbuf; /* virtual address */
	u64 txbuf; /* virtual address */
	size_t rxtx_min_pages; /* minimum number of pages in each of the RX/TX buffers */
};

/**
 * struct ffa_partition_desc - the secure partition descriptor
 * @info:	partition information
 * @sp_uuid:	the secure partition UUID
 *
 * Each partition has its descriptor containing the partitions information and the UUID
 */
struct ffa_partition_desc {
	struct ffa_partition_info info;
	struct ffa_partition_uuid sp_uuid;
};

/**
 * struct ffa_partitions - descriptors for all secure partitions
 * @count:	The number of partitions descriptors
 * @descs	The partitions descriptors table
 *
 * This data structure contains the partitions descriptors table
 */
struct ffa_partitions {
	u32 count;
	struct ffa_partition_desc *descs; /* virtual address */
};

/**
 * struct ffa_prvdata - the driver private data structure
 *
 * @dev:	The arm_ffa device under u-boot driver model
 * @ffa_ops:	The driver operations structure
 * @fwk_version:	FF-A framework version
 * @id:	u-boot endpoint ID
 * @partitions:	The partitions descriptors structure
 * @pair:	The RX/TX buffers pair
 * @invoke_ffa_fn:	The function executing the FF-A function
 *
 * The driver data structure hosting all resident data.
 */
struct ffa_prvdata {
	struct udevice *dev;
	struct ffa_bus_ops ffa_ops;
	u32 fwk_version;
	u16 id;
	struct ffa_partitions partitions;
	struct ffa_rxtxpair pair;
	invoke_ffa_fn_t invoke_ffa_fn;
};

/**
 * ffa_device_get - create, bind and probe the arm_ffa device
 */
int ffa_device_get(void);

/**
 * ffa_bus_prvdata_get - bus driver private data getter
 */
struct ffa_prvdata **ffa_bus_prvdata_get(void);

#endif
