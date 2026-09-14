/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2022 ARM Limited
 * Abdellatif El Khlifi <abdellatif.elkhlifi@arm.com>
 */

#ifndef __ARM_FFA_H
#define __ARM_FFA_H

#include <linux/printk.h>

/*
 * This header is public. It can be used by clients to access
 * data structures and definitions they need
 */

/*
 * Macros for displaying logs
 */

#define ffa_info(fmt, ...)  pr_info("[FFA] " fmt "\n", ##__VA_ARGS__)
#define ffa_err(fmt, ...)  pr_err("[FFA] " fmt "\n", ##__VA_ARGS__)

/*
 * struct ffa_partition_info - Partition information descriptor
 * @id:	Partition ID
 * @exec_ctxt:	Execution context count
 * @properties:	Partition properties
 *
 * Data structure containing information about partitions instantiated in the system
 * This structure is filled with the data queried by FFA_PARTITION_INFO_GET
 */
struct  __packed ffa_partition_info {
	u16 id;
	u16 exec_ctxt;
/* partition supports receipt of direct requests */
#define FFA_PARTITION_DIRECT_RECV	BIT(0)
/* partition can send direct requests. */
#define FFA_PARTITION_DIRECT_SEND	BIT(1)
/* partition can send and receive indirect messages. */
#define FFA_PARTITION_INDIRECT_MSG	BIT(2)
	u32 properties;
};

/*
 * struct ffa_send_direct_data - Data structure hosting the data
 *                                       used by FFA_MSG_SEND_DIRECT_{REQ,RESP}
 * @data0-4:	Data read/written from/to x3-x7 registers
 *
 * Data structure containing the data to be sent by FFA_MSG_SEND_DIRECT_REQ
 * or read from FFA_MSG_SEND_DIRECT_RESP
 */

/* For use with FFA_MSG_SEND_DIRECT_{REQ,RESP} which pass data via registers */
struct __packed ffa_send_direct_data {
	unsigned long data0; /* w3/x3 */
	unsigned long data1; /* w4/x4 */
	unsigned long data2; /* w5/x5 */
	unsigned long data3; /* w6/x6 */
	unsigned long data4; /* w7/x7 */
};

#if CONFIG_IS_ENABLED(ARM_FFA_EFI_RUNTIME_MODE)

#include <efi_loader.h>

/*
 *  __ffa_runtime - controls whether functions are
 * available after calling the EFI ExitBootServices service.
 * Functions tagged with these keywords are resident (available at boot time and
 * at runtime)
 */

#define __ffa_runtime_data __efi_runtime_data
#define __ffa_runtime __efi_runtime

#else

/*
 *  The FF-A driver is independent from EFI
 */

#define __ffa_runtime_data
#define __ffa_runtime

#endif

/**
 * struct ffa_bus_ops - The driver operations structure
 * @partition_info_get:	callback for the FFA_PARTITION_INFO_GET
 * @sync_send_receive:	callback for the FFA_MSG_SEND_DIRECT_REQ
 * @rxtx_unmap:	callback for the FFA_RXTX_UNMAP
 *
 * The data structure providing all the operations supported by the driver.
 * This structure is EFI runtime resident.
 */
struct ffa_bus_ops {
	int (*partition_info_get)(const char *uuid_str,
				  u32 *parts_size, struct ffa_partition_info *buffer);
	int (*sync_send_receive)(u16 dst_part_id, struct ffa_send_direct_data *msg, u8 is_smc64);
	int (*rxtx_unmap)(void);
};

/**
 * The device driver and the Uclass driver public functions
 */

/**
 * ffa_bus_ops_get - driver operations getter
 */
const struct ffa_bus_ops * __ffa_runtime ffa_bus_ops_get(void);

/**
 * ffa_bus_discover - discover FF-A bus and probes the arm_ffa and sandbox_arm_ffa devices
 */
int ffa_bus_discover(void);

#if CONFIG_IS_ENABLED(ARM_FFA_EFI_RUNTIME_MODE)

/**
 * ffa_copy_runtime_data - copy the private data structure and the SPs data to the runtime area
 */
efi_status_t ffa_copy_runtime_data(void);

#endif

#endif
