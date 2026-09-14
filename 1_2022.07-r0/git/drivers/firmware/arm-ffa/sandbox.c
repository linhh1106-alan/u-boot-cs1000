// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 ARM Limited
 * Abdellatif El Khlifi <abdellatif.elkhlifi@arm.com>
 */

#include "sandbox_arm_ffa_prv.h"
#include <asm/global_data.h>
#include <common.h>
#include <dm.h>
#include <dm/device-internal.h>
#include <dm/root.h>
#include <linux/errno.h>
#include <linux/sizes.h>
#include <mapmem.h>
#include <string.h>

DECLARE_GLOBAL_DATA_PTR;

/**
 * The device private data structure containing all the emulated secure world data
 */
static struct sandbox_ffa_prvdata  sandbox_ffa_priv_data = {0};

/* The partitions (SPs) table */
static struct ffa_partition_desc sandbox_partitions[SANDBOX_PARTITIONS_CNT] = {
	{
		.info = { .id = SANDBOX_SP1_ID, .exec_ctxt = 0x5687, .properties = 0x89325621 },
		.sp_uuid = {
			.a1 = SANDBOX_SERVICE1_UUID_A1,
			.a2 = SANDBOX_SERVICE1_UUID_A2,
			.a3 = SANDBOX_SERVICE1_UUID_A3,
			.a4 = SANDBOX_SERVICE1_UUID_A4,
		}
	},
	{
		.info = { .id = SANDBOX_SP2_ID, .exec_ctxt = 0x9587, .properties = 0x45325621 },
		.sp_uuid = {
			.a1 = SANDBOX_SERVICE2_UUID_A1,
			.a2 = SANDBOX_SERVICE2_UUID_A2,
			.a3 = SANDBOX_SERVICE2_UUID_A3,
			.a4 = SANDBOX_SERVICE2_UUID_A4,
		}
	},
	{
		.info = { .id = SANDBOX_SP3_ID, .exec_ctxt = 0x7687, .properties = 0x23325621 },
		.sp_uuid = {
			.a1 = SANDBOX_SERVICE1_UUID_A1,
			.a2 = SANDBOX_SERVICE1_UUID_A2,
			.a3 = SANDBOX_SERVICE1_UUID_A3,
			.a4 = SANDBOX_SERVICE1_UUID_A4,
		}
	},
	{
		.info = { .id = SANDBOX_SP4_ID, .exec_ctxt = 0x1487, .properties = 0x70325621 },
		.sp_uuid = {
			.a1 = SANDBOX_SERVICE2_UUID_A1,
			.a2 = SANDBOX_SERVICE2_UUID_A2,
			.a3 = SANDBOX_SERVICE2_UUID_A3,
			.a4 = SANDBOX_SERVICE2_UUID_A4,
		}
	}

};

/*
 * Driver functions
 */

/**
 * sandbox_ffa_get_device - probes the sandbox_arm_ffa device
 *
 * This function makes sure the sandbox_arm_ffa device is probed
 * This function makes sure the sandbox_arm_ffa device is
 * created, bound to this driver, probed and ready to use.
 *
 * sandbox_arm_ffa depends on arm_ffa device. This dependency is
 * handled by ffa_bus_discover function. arm_ffa is probed first then
 * sandbox_arm_ffa.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
int sandbox_ffa_device_get(void)
{
	int ret;

	if (sandbox_ffa_priv_data.dev)
		return 0;

	ret = device_bind(dm_root(),
			  DM_DRIVER_GET(sandbox_arm_ffa),
			  FFA_SANDBOX_DRV_NAME,
			  NULL,
			  ofnode_null(),
			  &sandbox_ffa_priv_data.dev);
	if (ret) {
		sandbox_ffa_priv_data.dev = NULL;
		return ret;
	}

	ret = device_probe(sandbox_ffa_priv_data.dev);
	if (ret) {
		ffa_err("[Sandbox] can not probe  the device");
		device_unbind(sandbox_ffa_priv_data.dev);
		sandbox_ffa_priv_data.dev = NULL;
		return ret;
	}

	return 0;
}

/**
 * sandbox_ffa_version - Emulated FFA_VERSION handler function
 * @{a0-a7} , res: The SMC call arguments and return structure.
 *
 * This is the function that emulates FFA_VERSION FF-A function.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
SANDBOX_SMC_FFA_ABI(ffa_version)
{
	sandbox_ffa_priv_data.fwk_version = FFA_VERSION_1_0;
	res->a0 = sandbox_ffa_priv_data.fwk_version;

	/* x1-x7 MBZ */
	memset(FFA_X1X7_MBZ_REG_START, 0, FFA_X1X7_MBZ_CNT * sizeof(unsigned long));

	return 0;
}

/**
 * sandbox_ffa_id_get - Emulated FFA_ID_GET handler function
 * @{a0-a7} , res: The SMC call arguments and return structure.
 *
 * This is the function that emulates FFA_ID_GET FF-A function.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
SANDBOX_SMC_FFA_ABI(ffa_id_get)
{
	res->a0 = FFA_SMC_32(FFA_SUCCESS);
	res->a1 = 0;

	sandbox_ffa_priv_data.id = NS_PHYS_ENDPOINT_ID;
	res->a2 = sandbox_ffa_priv_data.id;

	/* x3-x7 MBZ */
	memset(FFA_X3_MBZ_REG_START, 0, FFA_X3X7_MBZ_CNT * sizeof(unsigned long));

	return 0;
}

/**
 * sandbox_ffa_features - Emulated FFA_FEATURES handler function
 * @{a0-a7} , res: The SMC call arguments and return structure.
 *
 * This is the function that emulates FFA_FEATURES FF-A function.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
SANDBOX_SMC_FFA_ABI(ffa_features)
{
	if (pargs->a1 == FFA_SMC_64(FFA_RXTX_MAP)) {
		res->a0 = FFA_SMC_32(FFA_SUCCESS);
		res->a2 = RXTX_BUFFERS_MIN_SIZE;
		res->a3 = 0;
		/* x4-x7 MBZ */
		memset(FFA_X4X7_MBZ_REG_START,
		       0, FFA_X4X7_MBZ_CNT * sizeof(unsigned long));
	} else {
		res->a0 = FFA_SMC_32(FFA_ERROR);
		res->a2 = FFA_ERR_STAT_NOT_SUPPORTED;
		/* x3-x7 MBZ */
		memset(FFA_X3_MBZ_REG_START,
		       0, FFA_X3X7_MBZ_CNT * sizeof(unsigned long));
		ffa_err("[Sandbox] FF-A interface 0x%lx not implemented", pargs->a1);
	}

	res->a1 = 0;

	return 0;
}

/**
 * sandbox_ffa_partition_info_get - Emulated FFA_PARTITION_INFO_GET handler function
 * @{a0-a7} , res: The SMC call arguments and return structure.
 *
 * This is the function that emulates FFA_PARTITION_INFO_GET FF-A function.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
SANDBOX_SMC_FFA_ABI(ffa_partition_info_get)
{
	struct ffa_partition_info *rxbuf_desc_info = NULL;
	u32 descs_cnt;
	u32 descs_size_bytes;

	res->a0 = FFA_SMC_32(FFA_ERROR);

	if (!sandbox_ffa_priv_data.pair.rxbuf) {
		res->a2 = FFA_ERR_STAT_DENIED;
		goto cleanup;
	}

	if (sandbox_ffa_priv_data.pair_info.rxbuf_owned) {
		res->a2 = FFA_ERR_STAT_BUSY;
		goto cleanup;
	}

	if (!sandbox_ffa_priv_data.partitions.descs) {
		sandbox_ffa_priv_data.partitions.descs = sandbox_partitions;
		sandbox_ffa_priv_data.partitions.count = SANDBOX_PARTITIONS_CNT;
	}

	descs_size_bytes = SANDBOX_PARTITIONS_CNT * sizeof(struct ffa_partition_desc);

	/* Abort if the RX buffer size is smaller than the descriptors buffer size */
	if ((sandbox_ffa_priv_data.pair_info.rxtx_buf_size * SZ_4K) < descs_size_bytes) {
		res->a2 = FFA_ERR_STAT_NO_MEMORY;
		goto cleanup;
	}

	rxbuf_desc_info = (struct ffa_partition_info *)sandbox_ffa_priv_data.pair.rxbuf;

	/* No UUID specified. Return the information of all partitions */
	if (!pargs->a1 && !pargs->a2 && !pargs->a3 && !pargs->a4) {
		for (descs_cnt = 0 ; descs_cnt < SANDBOX_PARTITIONS_CNT ; descs_cnt++)
			*(rxbuf_desc_info++) =
				sandbox_ffa_priv_data.partitions.descs[descs_cnt].info;

		res->a0 = FFA_SMC_32(FFA_SUCCESS);
		res->a2 = SANDBOX_PARTITIONS_CNT;
		/* transfer ownership to the consumer: the non secure world */
		sandbox_ffa_priv_data.pair_info.rxbuf_owned = 1;

		goto cleanup;
	}

	/*
	 * A UUID is specified. Return the information of all partitions matching
	 * the UUID
	 */

	for (descs_cnt = 0 ; descs_cnt < SANDBOX_PARTITIONS_CNT ; descs_cnt++)
		if (pargs->a1 == sandbox_ffa_priv_data.partitions.descs[descs_cnt].sp_uuid.a1 &&
		    pargs->a2 == sandbox_ffa_priv_data.partitions.descs[descs_cnt].sp_uuid.a2 &&
		    pargs->a3 == sandbox_ffa_priv_data.partitions.descs[descs_cnt].sp_uuid.a3 &&
		    pargs->a4 == sandbox_ffa_priv_data.partitions.descs[descs_cnt].sp_uuid.a4) {
			*(rxbuf_desc_info++) =
				sandbox_ffa_priv_data.partitions.descs[descs_cnt].info;
		}

	if (rxbuf_desc_info != ((struct ffa_partition_info *)sandbox_ffa_priv_data.pair.rxbuf)) {
		res->a0 = FFA_SMC_32(FFA_SUCCESS);
		/* store the partitions count */
		res->a2 = (unsigned long)
			(rxbuf_desc_info - (struct ffa_partition_info *)
			 sandbox_ffa_priv_data.pair.rxbuf);

		/* transfer ownership to the consumer: the non secure world */
		sandbox_ffa_priv_data.pair_info.rxbuf_owned = 1;
	} else {
		/* Unrecognized UUID */
		res->a2 = FFA_ERR_STAT_INVALID_PARAMETERS;
	}

cleanup:

	ffa_err("[Sandbox] FFA_PARTITION_INFO_GET (%ld)", res->a2);

	res->a1 = 0;

	/* x3-x7 MBZ */
	memset(FFA_X3_MBZ_REG_START, 0, FFA_X3X7_MBZ_CNT * sizeof(unsigned long));

	return 0;
}

/**
 * sandbox_ffa_rxtx_map - Emulated FFA_RXTX_MAP handler function
 * @{a0-a7} , res: The SMC call arguments and return structure.
 *
 * This is the function that emulates FFA_RXTX_MAP FF-A function.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
SANDBOX_SMC_FFA_ABI(ffa_rxtx_map)
{
	res->a0 = FFA_SMC_32(FFA_ERROR);

	if (sandbox_ffa_priv_data.pair.txbuf && sandbox_ffa_priv_data.pair.rxbuf) {
		res->a2 = FFA_ERR_STAT_DENIED;
		goto feedback;
	}

	if (pargs->a3 >= RXTX_BUFFERS_MIN_PAGES && pargs->a1 && pargs->a2) {
		sandbox_ffa_priv_data.pair.txbuf = pargs->a1;
		sandbox_ffa_priv_data.pair.rxbuf = pargs->a2;
		sandbox_ffa_priv_data.pair_info.rxtx_buf_size = pargs->a3;
		sandbox_ffa_priv_data.pair_info.rxbuf_mapped = 1;
		res->a0 = FFA_SMC_32(FFA_SUCCESS);
		res->a2 = 0;
		goto feedback;
	}

	if (!pargs->a1 || !pargs->a2)
		res->a2 = FFA_ERR_STAT_INVALID_PARAMETERS;
	else
		res->a2 = FFA_ERR_STAT_NO_MEMORY;

	ffa_err("[Sandbox] error in FFA_RXTX_MAP arguments (%d)", (int)res->a2);

feedback:

	res->a1 = 0;

	/* x3-x7 MBZ */
	memset(FFA_X3_MBZ_REG_START,
	       0, FFA_X3X7_MBZ_CNT * sizeof(unsigned long));

	return 0;
}

/**
 * sandbox_ffa_rxtx_unmap - Emulated FFA_RXTX_UNMAP handler function
 * @{a0-a7} , res: The SMC call arguments and return structure.
 *
 * This is the function that emulates FFA_RXTX_UNMAP FF-A function.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
SANDBOX_SMC_FFA_ABI(ffa_rxtx_unmap)
{
	res->a0 = FFA_SMC_32(FFA_ERROR);
	res->a2 = FFA_ERR_STAT_INVALID_PARAMETERS;

	if (GET_NS_PHYS_ENDPOINT_ID(pargs->a1) != sandbox_ffa_priv_data.id)
		goto feedback;

	if (sandbox_ffa_priv_data.pair.txbuf && sandbox_ffa_priv_data.pair.rxbuf) {
		sandbox_ffa_priv_data.pair.txbuf = 0;
		sandbox_ffa_priv_data.pair.rxbuf = 0;
		sandbox_ffa_priv_data.pair_info.rxtx_buf_size = 0;
		sandbox_ffa_priv_data.pair_info.rxbuf_mapped = 0;
		res->a0 = FFA_SMC_32(FFA_SUCCESS);
		res->a2 = 0;
		goto feedback;
	}

	ffa_err("[Sandbox] No buffer pair registered on behalf of the caller");

feedback:

	res->a1 = 0;

	/* x3-x7 MBZ */
	memset(FFA_X3_MBZ_REG_START,
	       0, FFA_X3X7_MBZ_CNT * sizeof(unsigned long));

	return 0;
}

/**
 * sandbox_ffa_rx_release - Emulated FFA_RX_RELEASE handler function
 * @{a0-a7} , res: The SMC call arguments and return structure.
 *
 * This is the function that emulates FFA_RX_RELEASE FF-A function.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
SANDBOX_SMC_FFA_ABI(ffa_rx_release)
{
	if (!sandbox_ffa_priv_data.pair_info.rxbuf_owned) {
		res->a0 = FFA_SMC_32(FFA_ERROR);
		res->a2 = FFA_ERR_STAT_DENIED;
	} else {
		sandbox_ffa_priv_data.pair_info.rxbuf_owned = 0;
		res->a0 = FFA_SMC_32(FFA_SUCCESS);
		res->a2 = 0;
	}

	res->a1 = 0;

	/* x3-x7 MBZ */
	memset(FFA_X3_MBZ_REG_START,
	       0, FFA_X3X7_MBZ_CNT * sizeof(unsigned long));

	return 0;
}

/**
 * sandbox_ffa_sp_valid - Checks SP validity
 * @part_id: partition ID to check
 *
 * This is the function searches the input ID in the descriptors table.
 *
 * Return:
 *
 * 1 on success (Partition found). Otherwise, failure
 */
static int sandbox_ffa_sp_valid(u16 part_id)
{
	u32 descs_cnt;

	for (descs_cnt = 0 ; descs_cnt < SANDBOX_PARTITIONS_CNT ; descs_cnt++)
		if (sandbox_ffa_priv_data.partitions.descs[descs_cnt].info.id == part_id)
			return 1;

	return 0;
}

/**
 * sandbox_ffa_msg_send_direct_req - Emulated FFA_MSG_SEND_DIRECT_{REQ,RESP} handler function
 * @{a0-a7} , res: The SMC call arguments and return structure.
 *
 * This is the function that emulates FFA_MSG_SEND_DIRECT_{REQ,RESP}
 * FF-A functions. Only SMC 64-bit is supported in Sandbox.
 *
 * Emulating interrupts is not supported. So, FFA_RUN and FFA_INTERRUPT are not supported.
 * In case of success FFA_MSG_SEND_DIRECT_RESP is returned with default pattern data (0xff).
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
SANDBOX_SMC_FFA_ABI(ffa_msg_send_direct_req)
{
	u16 part_id;

	part_id = GET_DST_SP_ID(pargs->a1);

	if ((GET_NS_PHYS_ENDPOINT_ID(pargs->a1) != sandbox_ffa_priv_data.id) ||
	    !sandbox_ffa_sp_valid(part_id) ||
		pargs->a2) {
		res->a0 = FFA_SMC_32(FFA_ERROR);
		res->a1 = 0;
		res->a2 = FFA_ERR_STAT_INVALID_PARAMETERS;

		/* x3-x7 MBZ */
		memset(FFA_X3_MBZ_REG_START,
		       0, FFA_X3X7_MBZ_CNT * sizeof(unsigned long));

		return 0;
	}

	res->a0 = FFA_SMC_64(FFA_MSG_SEND_DIRECT_RESP);

	res->a1 = PREP_SRC_SP_ID(part_id) |
		PREP_NS_PHYS_ENDPOINT_ID(sandbox_ffa_priv_data.id);

	res->a2 = 0;

	/*
	 * return 0xff bytes as a response
	 */
	res->a3 = 0xffffffffffffffff;
	res->a4 = 0xffffffffffffffff;
	res->a5 = 0xffffffffffffffff;
	res->a6 = 0xffffffffffffffff;
	res->a7 = 0xffffffffffffffff;

	return 0;
}

/**
 * sandbox_ffa_get_prv_data - Returns the pointer to FF-A core pivate data
 * @func_data:  Pointer to the FF-A function arguments container structure
 *
 * This is the handler that returns the address of the FF-A core pivate data.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int sandbox_ffa_get_prv_data(struct ffa_sandbox_data *func_data)
{
	if (!func_data)
		return -EINVAL;

	if (!func_data->data0 || func_data->data0_size != sizeof(struct ffa_prvdata **))
		return -EINVAL;

	if (!func_data->data1 || func_data->data1_size != sizeof(struct sandbox_ffa_prvdata **))
		return -EINVAL;

	*((struct ffa_prvdata **)func_data->data0) = *(ffa_bus_prvdata_get());
	*((struct sandbox_ffa_prvdata **)func_data->data1) = &sandbox_ffa_priv_data;

	return 0;
}

/**
 * sandbox_ffa_get_rxbuf_flags - Reading the mapping/ownership flags
 * @queried_func_id:	The FF-A function to be queried
 * @func_data:  Pointer to the FF-A function arguments container structure
 *
 * This is the handler that queries the status flags of the following emulated ABIs:
 * FFA_RXTX_MAP, FFA_RXTX_UNMAP, FFA_RX_RELEASE
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int sandbox_ffa_get_rxbuf_flags(u32 queried_func_id, struct ffa_sandbox_data *func_data)
{
	if (!func_data)
		return -EINVAL;

	if (!func_data->data0 || func_data->data0_size != sizeof(u8))
		return -EINVAL;

	switch (queried_func_id) {
	case FFA_RXTX_MAP:
	case FFA_RXTX_UNMAP:
		*((u8 *)func_data->data0) = sandbox_ffa_priv_data.pair_info.rxbuf_mapped;
		return 0;
	case FFA_RX_RELEASE:
		*((u8 *)func_data->data0) = sandbox_ffa_priv_data.pair_info.rxbuf_owned;
		return 0;
	default:
		ffa_err("[Sandbox] The querried  FF-A interface flag (%d) undefined",
			queried_func_id);
		return -EINVAL;
	}
}

/**
 * sandbox_ffa_query_core_state - The driver dispatcher function
 * @queried_func_id:	The FF-A function to be queried
 * @func_data:  Pointer to the FF-A function arguments container structure
 *
 * Queries the status of FF-A ABI specified in the input argument.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
int sandbox_ffa_query_core_state(u32 queried_func_id, struct ffa_sandbox_data *func_data)
{
	switch (queried_func_id) {
	case FFA_VERSION:
	case FFA_ID_GET:
	case FFA_FEATURES:
		return sandbox_ffa_get_prv_data(func_data);
	case FFA_RXTX_MAP:
	case FFA_RXTX_UNMAP:
	case FFA_RX_RELEASE:
		return sandbox_ffa_get_rxbuf_flags(queried_func_id, func_data);
	default:
		ffa_err("[Sandbox] The querried  FF-A interface (%d) undefined", queried_func_id);
		return -EINVAL;
	}
}

/**
 * sandbox_arm_ffa_smccc_smc - FF-A SMC call emulation
 * @args:	the SMC call arguments
 * @res:	the SMC call returned data
 *
 * Sandbox driver emulates the FF-A ABIs SMC call using this function.
 * The emulated FF-A ABI is identified and invoked.
 * FF-A emulation is based on the FF-A specification 1.0
 *
 * Return:
 *
 * 0 on success. Otherwise, failure.
 * FF-A protocol error codes are returned using the registers arguments as described
 * by the specification
 */
void sandbox_arm_ffa_smccc_smc(ffa_value_t args, ffa_value_t *res)
{
	int ret = 0;

	switch (args.a0) {
	case FFA_SMC_32(FFA_VERSION):
		ret = sandbox_ffa_version(&args, res);
		break;
	case FFA_SMC_32(FFA_PARTITION_INFO_GET):
		ret = sandbox_ffa_partition_info_get(&args, res);
		break;
	case FFA_SMC_32(FFA_RXTX_UNMAP):
		ret = sandbox_ffa_rxtx_unmap(&args, res);
		break;
	case FFA_SMC_64(FFA_MSG_SEND_DIRECT_REQ):
		ret = sandbox_ffa_msg_send_direct_req(&args, res);
		break;
	case FFA_SMC_32(FFA_ID_GET):
		ret = sandbox_ffa_id_get(&args, res);
		break;
	case FFA_SMC_32(FFA_FEATURES):
		ret = sandbox_ffa_features(&args, res);
		break;
	case FFA_SMC_64(FFA_RXTX_MAP):
		ret = sandbox_ffa_rxtx_map(&args, res);
		break;
	case FFA_SMC_32(FFA_RX_RELEASE):
		ret = sandbox_ffa_rx_release(&args, res);
		break;
	default:
		ffa_err("[Sandbox] Undefined FF-A interface (0x%lx)", args.a0);
	}

	if (ret != 0)
		ffa_err("[Sandbox] FF-A ABI internal failure  (%d)", ret);
}

/**
 * sandbox_ffa_probe - The driver probe function
 * @dev:	the sandbox_arm_ffa device
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int sandbox_ffa_probe(struct udevice *dev)
{
	return 0;
}

/**
 * sandbox_ffa_remove - The driver remove function
 * @dev:	the sandbox_arm_ffa device
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int sandbox_ffa_remove(struct udevice *dev)
{
	ffa_info("[Sandbox] removing the device");
	memset(&sandbox_ffa_priv_data, 0, sizeof(sandbox_ffa_priv_data));
	return 0;
}

/**
 * Declaring the sandbox_arm_ffa driver under UCLASS_FFA
 */
U_BOOT_DRIVER(sandbox_arm_ffa) = {
	.name		= FFA_SANDBOX_DRV_NAME,
	.id		= UCLASS_FFA,
	.probe		= sandbox_ffa_probe,
	.remove		= sandbox_ffa_remove,
};
