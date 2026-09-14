// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 ARM Limited
 * Abdellatif El Khlifi <abdellatif.elkhlifi@arm.com>
 */

#include "arm_ffa_prv.h"
#include <asm/global_data.h>
#include <common.h>
#include <dm.h>
#include <dm/device-internal.h>
#include <dm/devres.h>
#include <dm/root.h>
#include <linux/errno.h>
#include <linux/sizes.h>
#include <log.h>
#include <malloc.h>
#include <string.h>
#include <uuid.h>

DECLARE_GLOBAL_DATA_PTR;

/**
 * The device private data structure containing all the resident
 * data read from secure world
 */
__ffa_runtime_data struct ffa_prvdata *ffa_priv_data;

/* Error mapping declarations */

__ffa_runtime_data int ffa_to_std_errmap[MAX_NUMBER_FFA_ERR] = {
	0,
	-EOPNOTSUPP,	/* NOT_SUPPORTED */
	-EINVAL,	/* INVALID_PARAMETERS */
	-ENOMEM,	/* NO_MEMORY */
	-EBUSY,		/* BUSY */
	-EINTR,		/* INTERRUPTED */
	-EACCES,	/* DENIED */
	-EAGAIN,	/* RETRY */
	-ECANCELED,	/* ABORTED */
};

struct ffa_abi_errmap err_msg_map[FFA_ERRMAP_COUNT] = {
	[FFA_ID_TO_ERRMAP_ID(FFA_VERSION)] = {
		{
			"",
			"NOT_SUPPORTED: A Firmware Framework implementation does not exist",
			"", /* INVALID_PARAMETERS */
			"", /* NO_MEMORY */
			"", /* BUSY */
			"", /* INTERRUPTED */
			"", /* DENIED */
			"", /* RETRY */
			"", /* ABORTED */
		},
	},
	[FFA_ID_TO_ERRMAP_ID(FFA_ID_GET)] = {
		{
			"",
			"NOT_SUPPORTED: This function is not implemented at this FF-A instance",
			"", /* INVALID_PARAMETERS */
			"", /* NO_MEMORY */
			"", /* BUSY */
			"", /* INTERRUPTED */
			"", /* DENIED */
			"", /* RETRY */
			"", /* ABORTED */
		},
	},
	[FFA_ID_TO_ERRMAP_ID(FFA_FEATURES)] = {
		{
			"",
			"NOT_SUPPORTED: FFA_RXTX_MAP is not implemented at this FF-A instance",
			"", /* INVALID_PARAMETERS */
			"", /* NO_MEMORY */
			"", /* BUSY */
			"", /* INTERRUPTED */
			"", /* DENIED */
			"", /* RETRY */
			"", /* ABORTED */
		},
	},
	[FFA_ID_TO_ERRMAP_ID(FFA_PARTITION_INFO_GET)] = {
		{
			"",
			"NOT_SUPPORTED: This function is not implemented at this FF-A instance",
			"INVALID_PARAMETERS: Unrecognized UUID",
			"NO_MEMORY: Results cannot fit in RX buffer of the caller",
			"BUSY: RX buffer of the caller is not free",
			"", /* INTERRUPTED */
			"DENIED: Callee is not in a state to handle this request",
			"", /* RETRY */
			"", /* ABORTED */
		},
	},
	[FFA_ID_TO_ERRMAP_ID(FFA_RXTX_UNMAP)] = {
			{
			"",
			"NOT_SUPPORTED: FFA_RXTX_UNMAP is not implemented at this FF-A instance",
			"INVALID_PARAMETERS: No buffer pair registered on behalf of the caller",
			"", /* NO_MEMORY */
			"", /* BUSY */
			"", /* INTERRUPTED */
			"", /* DENIED */
			"", /* RETRY */
			"", /* ABORTED */
		},
	},
	[FFA_ID_TO_ERRMAP_ID(FFA_RX_RELEASE)] = {
			{
			"",
			"NOT_SUPPORTED: FFA_RX_RELEASE is not implemented at this FF-A instance",
			"", /* INVALID_PARAMETERS */
			"", /* NO_MEMORY */
			"", /* BUSY */
			"", /* INTERRUPTED */
			"DENIED: Caller did not have ownership of the RX buffer",
			"", /* RETRY */
			"", /* ABORTED */
		},
	},
	[FFA_ID_TO_ERRMAP_ID(FFA_RXTX_MAP)] = {
			{
			"",
			"NOT_SUPPORTED: This function is not implemented at this FF-A instance",
			"INVALID_PARAMETERS: Field(s) in input parameters incorrectly encoded",
			"NO_MEMORY: Not enough memory",
			"", /* BUSY */
			"", /* INTERRUPTED */
			"DENIED: Buffer pair already registered",
			"", /* RETRY */
			"", /* ABORTED */
		},
	},
};

/**
 * ffa_to_std_errno - convert FF-A error code to standard error code
 * @ffa_errno:	Error code returned by the FF-A ABI
 *
 * This runtime function maps the given FF-A error code as specified
 * by the spec to a u-boot standard error code.
 *
 * Return:
 *
 * The standard error code on success. . Otherwise, failure
 */
__ffa_runtime int ffa_to_std_errno(int ffa_errno)
{
	int err_idx = -ffa_errno;

	/* map the FF-A error code to the standard u-boot error code */
	if (err_idx > 0 && err_idx < MAX_NUMBER_FFA_ERR)
		return ffa_to_std_errmap[err_idx];
	return -EINVAL;
}

/**
 * ffa_print_error_log - print the error log corresponding to the selected FF-A ABI
 * @ffa_id:	FF-A ABI ID
 * @ffa_errno:	Error code returned by the FF-A ABI
 *
 * This boot time function maps the FF-A error code to the error log relevant to the
 * selected FF-A ABI. Then the error log is printed.
 *
 * Return:
 *
 * 0 on success. . Otherwise, failure
 */
int ffa_print_error_log(u32 ffa_id, int ffa_errno)
{
	int err_idx = -ffa_errno, abi_idx = 0;

	/* map the FF-A error code to the corresponding error log */

	if (err_idx <= 0 || err_idx >= MAX_NUMBER_FFA_ERR)
		return -EINVAL;

	if (ffa_id < FFA_FIRST_ID || ffa_id > FFA_LAST_ID)
		return -EINVAL;

	abi_idx = FFA_ID_TO_ERRMAP_ID(ffa_id);
	if (abi_idx < 0 || abi_idx >= FFA_ERRMAP_COUNT)
		return -EINVAL;

	if (!err_msg_map[abi_idx].err_str || !err_msg_map[abi_idx].err_str[err_idx])
		return -EINVAL;

	ffa_err("%s", err_msg_map[abi_idx].err_str[err_idx]);

	return 0;
}

/*
 * Driver core functions
 */

/**
 * ffa_remove_device - removes the arm_ffa device
 * @dev:	the device to be removed
 *
 * This boot time function makes sure the arm_ffa device is removed
 * No need to free the kmalloced data when the device is destroyed.
 * It's automatically done by devm management by
 * device_remove() -> device_free() -> devres_release_probe().
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
int ffa_remove_device(struct udevice *dev)
{
	int ret;

	if (!dev) {
		ffa_err("no udevice found");
		return -ENODEV;
	}

	ret = device_remove(dev, DM_REMOVE_NORMAL);
	if (ret) {
		ffa_err("unable to remove. err:%d\n", ret);
		return ret;
	}

	ffa_info("device removed and freed");

	ret = device_unbind(dev);
	if (ret) {
		ffa_err("unable to unbind. err:%d\n", ret);
		return ret;
	}

	ffa_info("device unbound");

	return 0;
}

/**
 * ffa_device_get - create, bind and probe the arm_ffa device
 *
 * This boot time function makes sure the arm_ffa device is
 * created, bound to this driver, probed and ready to use.
 * Arm FF-A transport is implemented through a single u-boot
 * device managing the FF-A bus (arm_ffa).
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
int ffa_device_get(void)
{
	int ret;
	struct udevice *dev = NULL;

	ret = device_bind(dm_root(),
			  DM_DRIVER_GET(arm_ffa),
			  FFA_DRV_NAME,
			  NULL,
			  ofnode_null(),
			  &dev);
	if (ret)
		return ret;

	/* The FF-A bus discovery succeeds when probing is successful */
	ret = device_probe(dev);
	if (ret) {
		ffa_err("arm_ffa device probing failed");
		ffa_remove_device(dev);
		return ret;
	}

	return 0;
}

/**
 * ffa_get_version - FFA_VERSION handler function
 *
 * This is the boot time function that implements FFA_VERSION FF-A function
 * to get from the secure world the FF-A framework version
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int ffa_get_version(void)
{
	u16 major, minor;
	ffa_value_t res = {0};
	int ffa_errno;

	ffa_priv_data->invoke_ffa_fn((ffa_value_t){
			.a0 = FFA_SMC_32(FFA_VERSION),
			.a1 = FFA_VERSION_1_0, .a2 = 0, .a3 = 0, .a4 = 0, .a5 = 0, .a6 = 0, .a7 = 0,
			}, &res);

	ffa_errno = (int)res.a0;
	if (ffa_errno < 0) {
		ffa_print_error_log(FFA_VERSION, ffa_errno);
		return ffa_to_std_errno(ffa_errno);
	}

	major = GET_FFA_MAJOR_VERSION((u32)res.a0);
	minor = GET_FFA_MINOR_VERSION((u32)res.a0);

	ffa_info("FF-A driver %d.%d\nFF-A framework %d.%d",
		 FFA_MAJOR_VERSION, FFA_MINOR_VERSION, major, minor);

	if ((major == FFA_MAJOR_VERSION && minor >= FFA_MINOR_VERSION)) {
		ffa_info("Versions are compatible ");

		ffa_priv_data->fwk_version = (u32)res.a0;

		return 0;
	}

	ffa_err("versions are incompatible\nExpected: %d.%d , Found: %d.%d\n",
		FFA_MAJOR_VERSION, FFA_MINOR_VERSION, major, minor);

	return -EPROTONOSUPPORT;
}

/**
 * ffa_get_endpoint_id - FFA_ID_GET handler function
 *
 * This is the boot time function that implements FFA_ID_GET FF-A function
 * to get from the secure world u-boot endpoint ID
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int ffa_get_endpoint_id(void)
{
	ffa_value_t res = {0};
	int ffa_errno;

	ffa_priv_data->invoke_ffa_fn((ffa_value_t){
			.a0 = FFA_SMC_32(FFA_ID_GET),
			.a1 = 0, .a2 = 0, .a3 = 0, .a4 = 0, .a5 = 0, .a6 = 0, .a7 = 0,
			}, &res);

	if (res.a0 == FFA_SMC_32(FFA_SUCCESS)) {
		ffa_priv_data->id = GET_SELF_ENDPOINT_ID((u32)res.a2);
		ffa_info("endpoint ID is %u", ffa_priv_data->id);

		return 0;
	}

	ffa_errno = (int)res.a2;

	ffa_print_error_log(FFA_ID_GET, ffa_errno);

	return ffa_to_std_errno(ffa_errno);
}

/**
 * ffa_set_rxtx_buffers_pages_cnt - sets the minimum number of pages in each of the RX/TX buffers
 * @prop_field: properties field obtained from FFA_FEATURES ABI
 *
 * This boot time function sets the minimum number of pages
 *  in each of the RX/TX buffers in the private data structure
 *
 * Return:
 *
 * buf_4k_pages points to the returned number of pages
 * 0 on success. Otherwise, failure
 */
static int ffa_set_rxtx_buffers_pages_cnt(u32 prop_field)
{
	if (!ffa_priv_data)
		return -EINVAL;

	switch (prop_field) {
	case RXTX_4K:
		ffa_priv_data->pair.rxtx_min_pages = 1;
		break;
	case RXTX_16K:
		ffa_priv_data->pair.rxtx_min_pages = 4;
		break;
	case RXTX_64K:
		ffa_priv_data->pair.rxtx_min_pages = 16;
		break;
	default:
		ffa_err("RX/TX buffer size not supported");
		return -EINVAL;
	}

	return 0;
}

/**
 * ffa_get_rxtx_map_features - FFA_FEATURES handler function with FFA_RXTX_MAP argument
 *
 * This is the boot time function that implements FFA_FEATURES FF-A function
 * to retrieve the FFA_RXTX_MAP features
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int ffa_get_rxtx_map_features(void)
{
	ffa_value_t res = {0};
	int ffa_errno;

	ffa_priv_data->invoke_ffa_fn((ffa_value_t){
			.a0 = FFA_SMC_32(FFA_FEATURES),
			.a1 = FFA_SMC_64(FFA_RXTX_MAP),
			.a2 = 0, .a3 = 0, .a4 = 0, .a5 = 0, .a6 = 0, .a7 = 0,
			}, &res);

	if (res.a0 == FFA_SMC_32(FFA_SUCCESS))
		return ffa_set_rxtx_buffers_pages_cnt((u32)res.a2);

	ffa_errno = (int)res.a2;
	ffa_print_error_log(FFA_FEATURES, ffa_errno);

	return ffa_to_std_errno(ffa_errno);
}

/**
 * ffa_free_rxtx_buffers - frees the RX/TX buffers
 *
 * This is the boot time function used to free the RX/TX buffers
 *
 */
static void ffa_free_rxtx_buffers(void)
{
	ffa_info("Freeing RX/TX buffers");

	if (ffa_priv_data->pair.rxbuf) {
		free((void *)ffa_priv_data->pair.rxbuf);
		ffa_priv_data->pair.rxbuf = 0;
	}

	if (ffa_priv_data->pair.txbuf) {
		free((void *)ffa_priv_data->pair.txbuf);
		ffa_priv_data->pair.txbuf = 0;
	}
}

/**
 * ffa_alloc_rxtx_buffers - allocates the RX/TX buffers
 *
 * This is the boot time function used by ffa_map_rxtx_buffers to allocate
 * the RX/TX buffers before mapping them. The allocated memory is physically
 * contiguous since memalign ends up calling malloc which allocates
 * contiguous memory in u-boot.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int ffa_alloc_rxtx_buffers(void)
{
	u64 bytes;

	ffa_info("Using %lu 4KB page(s) for RX/TX buffers size",
		 ffa_priv_data->pair.rxtx_min_pages);

	bytes = ffa_priv_data->pair.rxtx_min_pages * SZ_4K;

	/* RX/TX buffers addresses should be PAGE_SIZE aligned */

	ffa_priv_data->pair.rxbuf = (u64)memalign(PAGE_SIZE, bytes);
	if (!ffa_priv_data->pair.rxbuf) {
		ffa_err("failure to allocate RX buffer");
		return -ENOBUFS;
	}

	ffa_info("RX buffer at virtual address 0x%llx", ffa_priv_data->pair.rxbuf);

	ffa_priv_data->pair.txbuf = (u64)memalign(PAGE_SIZE, bytes);
	if (!ffa_priv_data->pair.txbuf) {
		free((void *)ffa_priv_data->pair.rxbuf);
		ffa_priv_data->pair.rxbuf = 0;
		ffa_err("failure to allocate the TX buffer");
		return -ENOBUFS;
	}

	ffa_info("TX buffer at virtual address 0x%llx", ffa_priv_data->pair.txbuf);

	/*
	 * make sure the buffers are cleared before use
	 */
	memset((void *)ffa_priv_data->pair.rxbuf, 0, bytes);
	memset((void *)ffa_priv_data->pair.txbuf, 0, bytes);

	return 0;
}

/**
 * ffa_map_rxtx_buffers - FFA_RXTX_MAP handler function
 *
 * This is the boot time function that implements FFA_RXTX_MAP FF-A function
 * to map the RX/TX buffers
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int ffa_map_rxtx_buffers(void)
{
	int ret;
	ffa_value_t res = {0};
	int ffa_errno;

	ret = ffa_alloc_rxtx_buffers();
	if (ret)
		return ret;

	/*
	 * we need to pass the physical addresses of the RX/TX buffers
	 * in u-boot physical/virtual mapping is 1:1
	 *no need to convert from virtual to physical
	 */

	ffa_priv_data->invoke_ffa_fn((ffa_value_t){
			.a0 = FFA_SMC_64(FFA_RXTX_MAP),
			.a1 = ffa_priv_data->pair.txbuf,
			.a2 = ffa_priv_data->pair.rxbuf,
			.a3 = ffa_priv_data->pair.rxtx_min_pages,
			.a4 = 0, .a5 = 0, .a6 = 0, .a7 = 0,
			}, &res);

	if (res.a0 == FFA_SMC_32(FFA_SUCCESS)) {
		ffa_info("RX/TX buffers mapped");
		return 0;
	}

	ffa_errno = (int)res.a2;
	ffa_print_error_log(FFA_RXTX_MAP, ffa_errno);

	ffa_free_rxtx_buffers();

	return ffa_to_std_errno(ffa_errno);
}

/**
 * ffa_unmap_rxtx_buffers - FFA_RXTX_UNMAP handler function
 *
 * This is the boot time function that implements FFA_RXTX_UNMAP FF-A function
 * to unmap the RX/TX buffers
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int ffa_unmap_rxtx_buffers(void)
{
	ffa_value_t res = {0};
	int ffa_errno;

	ffa_priv_data->invoke_ffa_fn((ffa_value_t){
			.a0 = FFA_SMC_32(FFA_RXTX_UNMAP),
			.a1 = PREP_SELF_ENDPOINT_ID(ffa_priv_data->id),
			.a2 = 0, .a3 = 0, .a4 = 0, .a5 = 0, .a6 = 0, .a7 = 0,
			}, &res);

	if (res.a0 == FFA_SMC_32(FFA_SUCCESS)) {
		ffa_free_rxtx_buffers();
		return 0;
	}

	ffa_errno = (int)res.a2;
	ffa_print_error_log(FFA_RXTX_UNMAP, ffa_errno);

	return ffa_to_std_errno(ffa_errno);
}

/**
 * ffa_release_rx_buffer - FFA_RX_RELEASE handler function
 *
 * This is the boot time function that invokes FFA_RX_RELEASE FF-A function
 * to release the ownership of the RX buffer
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int ffa_release_rx_buffer(void)
{
	ffa_value_t res = {0};
	int ffa_errno;

	ffa_priv_data->invoke_ffa_fn((ffa_value_t){
			.a0 = FFA_SMC_32(FFA_RX_RELEASE),
			.a1 = 0, .a2 = 0, .a3 = 0, .a4 = 0, .a5 = 0, .a6 = 0, .a7 = 0,
			}, &res);

	if (res.a0 == FFA_SMC_32(FFA_SUCCESS))
		return 0;

	ffa_errno = (int)res.a2;
	ffa_print_error_log(FFA_RX_RELEASE, ffa_errno);

	return ffa_to_std_errno(ffa_errno);
}

/**
 * ffa_uuid_are_identical - checks whether two given UUIDs are identical
 * @uuid1: first UUID
 * @uuid2: second UUID
 *
 * This is a boot time function used by ffa_read_partitions_info to search
 * for a UUID in the partitions descriptors table
 *
 * Return:
 *
 * 1 when UUIDs match. Otherwise, 0
 */
int ffa_uuid_are_identical(const struct ffa_partition_uuid *uuid1,
			   const struct ffa_partition_uuid *uuid2)
{
	if (!uuid1 || !uuid2)
		return 0;

	return (!memcmp(uuid1, uuid2, sizeof(struct ffa_partition_uuid)));
}

/**
 * ffa_read_partitions_info - reads the data queried by FFA_PARTITION_INFO_GET
 *							and saves it in the private structure
 * @count: The number of partitions queried
 * @part_uuid: Pointer to the partition(s) UUID
 *
 * This is the boot time function that reads the partitions information
 * returned by the FFA_PARTITION_INFO_GET and saves it in the private
 * data structure.
 *
 * Return:
 *
 * The private data structure is updated with the partition(s) information
 * 0 is returned on success. Otherwise, failure
 */
static int ffa_read_partitions_info(u32 count, struct ffa_partition_uuid *part_uuid)
{
	if (!count) {
		ffa_err("no partition detected");
		return -ENODATA;
	}

	ffa_info("Reading partitions data from the RX buffer");

	if (!part_uuid) {
		/*
		 * querying information of all partitions
		 */
		u64 buf_bytes;
		u64 data_bytes;
		u32 desc_idx;
		struct ffa_partition_info *parts_info;

		data_bytes = count * sizeof(struct ffa_partition_desc);

		buf_bytes = ffa_priv_data->pair.rxtx_min_pages * SZ_4K;

		if (data_bytes > buf_bytes) {
			ffa_err("partitions data size exceeds the RX buffer size:");
			ffa_err("    sizes in bytes: data %llu , RX buffer %llu ",
				data_bytes,
				buf_bytes);

			return -ENOMEM;
		}

		ffa_priv_data->partitions.descs = (struct ffa_partition_desc *)
			devm_kmalloc(ffa_priv_data->dev, data_bytes, __GFP_ZERO);
		if (!ffa_priv_data->partitions.descs) {
			ffa_err("cannot  allocate partitions data buffer");
			return -ENOMEM;
		}

		parts_info = (struct ffa_partition_info *)ffa_priv_data->pair.rxbuf;

		for (desc_idx = 0 ; desc_idx < count ; desc_idx++) {
			ffa_priv_data->partitions.descs[desc_idx].info =
				parts_info[desc_idx];

			ffa_info("Partition ID %x : info cached",
				 ffa_priv_data->partitions.descs[desc_idx].info.id);
		}

		ffa_priv_data->partitions.count = count;

		ffa_info("%d partition(s) found and cached", count);

	} else {
		u32 rx_desc_idx, cached_desc_idx;
		struct ffa_partition_info *parts_info;
		u8 desc_found;

		parts_info = (struct ffa_partition_info *)ffa_priv_data->pair.rxbuf;

		/*
		 * search for the SP IDs read from the RX buffer
		 * in the already cached SPs.
		 * Update the UUID when ID found.
		 */
		for (rx_desc_idx = 0; rx_desc_idx < count ; rx_desc_idx++) {
			desc_found = 0;

			/*
			 * search the current ID in the cached partitions
			 */
			for (cached_desc_idx = 0;
			     cached_desc_idx < ffa_priv_data->partitions.count;
			     cached_desc_idx++) {
				/*
				 * save the UUID
				 */
				if (ffa_priv_data->partitions.descs[cached_desc_idx].info.id ==
				    parts_info[rx_desc_idx].id) {
					ffa_priv_data->partitions.descs[cached_desc_idx].sp_uuid =
						*part_uuid;

					desc_found = 1;
					break;
				}
			}

			if (!desc_found)
				return -ENODATA;
		}
	}

	return  0;
}

/**
 * ffa_query_partitions_info - invokes FFA_PARTITION_INFO_GET and saves partitions data
 *
 * @part_uuid: Pointer to the partition(s) UUID
 * @pcount: Pointer to the number of partitions variable filled when querying
 *
 * This is the boot time function that executes the FFA_PARTITION_INFO_GET
 * to query the partitions data. Then, it calls ffa_read_partitions_info
 * to save the data in the private data structure.
 *
 * After reading the data the RX buffer is released using ffa_release_rx_buffer
 *
 * Return:
 *
 * When part_uuid is NULL, all partitions data are retrieved from secure world
 * When part_uuid is non NULL, data for partitions matching the given UUID are
 * retrieved and the number of partitions is returned
 * 0 is returned on success. Otherwise, failure
 */
static int ffa_query_partitions_info(struct ffa_partition_uuid *part_uuid,
				     u32 *pcount)
{
	struct ffa_partition_uuid query_uuid = {0};
	ffa_value_t res = {0};
	int ffa_errno;

	/*
	 * If a UUID is specified. Information for one or more
	 * partitions in the system is queried. Otherwise, information
	 * for all installed partitions is queried
	 */

	if (part_uuid) {
		if (!pcount)
			return -EINVAL;

		query_uuid = *part_uuid;
	} else if (pcount) {
		return -EINVAL;
	}

	ffa_priv_data->invoke_ffa_fn((ffa_value_t){
			.a0 = FFA_SMC_32(FFA_PARTITION_INFO_GET),
			.a1 = query_uuid.a1,
			.a2 = query_uuid.a2,
			.a3 = query_uuid.a3,
			.a4 = query_uuid.a4,
			.a5 = 0,
			.a6 = 0,
			.a7 = 0,
			}, &res);

	if (res.a0 == FFA_SMC_32(FFA_SUCCESS)) {
		int ret;

		/*
		 * res.a2 contains the count of partition information descriptors
		 * populated in the RX buffer
		 */
		if (res.a2) {
			ret = ffa_read_partitions_info((u32)res.a2, part_uuid);
			if (ret) {
				ffa_err("failed to read partition(s) data , error (%d)", ret);
				ffa_release_rx_buffer();
				return -EINVAL;
			}
		}

		/*
		 * return the SP count (when querying using a UUID)
		 */
		if (pcount)
			*pcount = (u32)res.a2;

		/*
		 * After calling FFA_PARTITION_INFO_GET the buffer ownership
		 * is assigned to the consumer (u-boot). So, we need to give
		 * the ownership back to the SPM or hypervisor
		 */
		ret = ffa_release_rx_buffer();

		return ret;
	}

	ffa_errno = (int)res.a2;
	ffa_print_error_log(FFA_PARTITION_INFO_GET, ffa_errno);

	return ffa_to_std_errno(ffa_errno);
}

/**
 * ffa_get_partitions_info - FFA_PARTITION_INFO_GET handler function
 *
 * The passed arguments:
 * Mode 1: When getting from the driver the number of
 *	secure partitions:
 *	@uuid_str: pointer to the UUID string
 *	@parts_size: pointer to the variable that contains the number of partitions
 *			 The variable will be set by the driver
 *	@buffer: NULL
 *
 * Mode 2: When requesting the driver to return the
 *	partitions information:
 *	@uuid_str: pointer to the UUID string
 *	@parts_size: pointer to the size of the SPs information buffer in bytes
 *	@buffer: pointer to SPs information buffer
 *		(allocated by the client).
 *		The buffer will be filled by the driver
 *
 * This is the boot time function that queries the secure partition data from
 * the private data structure. If not found, it invokes FFA_PARTITION_INFO_GET
 * FF-A function to query the partition information from secure world.
 *
 * A client of the FF-A driver should know the UUID of the service it wants to
 * access. It should use the UUID to request the FF-A driver to provide the
 * partition(s) information of the service. The FF-A driver uses
 * PARTITION_INFO_GET to obtain this information. This is implemented through
 * ffa_get_partitions_info function.
 * A new FFA_PARTITION_INFO_GET call is issued (first one performed through
 * ffa_cache_partitions_info) allowing to retrieve the partition(s) information.
 * They are not saved (already done). We only update the UUID in the cached area.
 * This assumes that partitions data does not change in the secure world.
 * Otherwise u-boot will have an outdated partition data. The benefit of caching
 * the information in the FF-A driver is to accommodate discovery after
 * ExitBootServices().
 *
 * When invoked through a client request, ffa_get_partitions_info should be
 * called twice. First call is to get from the driver the number of secure
 * partitions (SPs) associated to a particular UUID.
 * Then, the caller (client) allocates the buffer to host the SPs data and
 * issues a 2nd call. Then, the driver fills the SPs data in the pre-allocated
 * buffer.
 *
 * To achieve the mechanism described above, ffa_get_partitions_info uses the
 * following functions:
 *		ffa_read_partitions_info
 *		ffa_query_partitions_info
 *
 * Return:
 *
 * @parts_size: When pointing to the number of partitions variable, the number is
 * set by the driver.
 * When pointing to the partitions information buffer size, the buffer will be
 * filled by the driver.
 *
 * On success 0 is returned. Otherwise, failure
 */
static int ffa_get_partitions_info(const char *uuid_str,
				   u32 *parts_size, struct ffa_partition_info *buffer)
{
	/*
	 * fill_data:
	 * 0: return the SP count
	 * 1: fill SP data and return it to the caller
	 * -1: undefined mode
	 */
	int fill_data = -1;
	u32 desc_idx, client_desc_idx;
	struct ffa_partition_uuid part_uuid = {0};
	u32 client_desc_max_cnt;
	u32 parts_found = 0;

	if (!ffa_priv_data->partitions.count || !ffa_priv_data->partitions.descs) {
		ffa_err("no partition installed");
		return -EINVAL;
	}

	if (!uuid_str) {
		ffa_err("no UUID provided");
		return -EINVAL;
	}

	if (!parts_size) {
		ffa_err("no size/count provided");
		return -EINVAL;
	}

	if (be_uuid_str_to_le_bin(uuid_str, (unsigned char *)&part_uuid)) {
		ffa_err("invalid UUID");
		return -EINVAL;
	}

	if (!buffer) {
		/* Mode 1: getting the number of secure partitions */

		fill_data = 0;

		ffa_info("Preparing for checking partitions count");

	} else if ((*parts_size >= sizeof(struct ffa_partition_info)) &&
		   !(*parts_size % sizeof(struct ffa_partition_info))) {
		/* Mode 2: retrieving the partitions information */

		fill_data = 1;

		client_desc_idx = 0;

		/*
		 * number of empty descriptors preallocated by the caller
		 */
		client_desc_max_cnt = *parts_size / sizeof(struct ffa_partition_info);

		ffa_info("Preparing for filling partitions info");

	} else {
		ffa_err("invalid function arguments provided");
		return -EINVAL;
	}

	ffa_info("Searching partitions using the provided UUID");

	/*
	 * search in the cached partitions
	 */
	for (desc_idx = 0;
	     desc_idx < ffa_priv_data->partitions.count;
	     desc_idx++) {
		if (ffa_uuid_are_identical(&ffa_priv_data->partitions.descs[desc_idx].sp_uuid,
					   &part_uuid)) {
			ffa_info("Partition ID %x matches the provided UUID",
				 ffa_priv_data->partitions.descs[desc_idx].info.id);

			parts_found++;

			if (fill_data) {
				/*
				 * trying to fill the partition info in the input buffer
				 */

				if (client_desc_idx < client_desc_max_cnt) {
					buffer[client_desc_idx++] =
						ffa_priv_data->partitions.descs[desc_idx].info;
					continue;
				}

				ffa_err("failed to fill the current descriptor client buffer full");
				return -ENOBUFS;
			}
		}
	}

	if (!parts_found) {
		int ret;

		ffa_info("No partition found. Querying framework ...");

		ret = ffa_query_partitions_info(&part_uuid, &parts_found);

		if (ret == 0) {
			if (!fill_data) {
				*parts_size = parts_found;

				ffa_info("Number of partition(s) found matching the UUID: %d",
					 parts_found);
			} else {
				/*
				 * If SPs data detected, they are already in the private data
				 * structure, retry searching SP data again to return them
				 *  to the caller
				 */
				if (parts_found)
					ret = ffa_get_partitions_info(uuid_str, parts_size, buffer);
				else
					ret = -ENODATA;
			}
		}

		return ret;
	}

	/* partition(s) found */
	if (!fill_data)
		*parts_size = parts_found;

	return 0;
}

/**
 * ffa_cache_partitions_info - Queries and saves all secure partitions data
 *
 * This is a boot time function that invokes FFA_PARTITION_INFO_GET FF-A
 * function to query from secure world all partitions information.
 *
 * The FFA_PARTITION_INFO_GET call is issued with nil UUID as an argument.
 * All installed partitions information are returned. We cache them in the
 * resident private data structure and we keep the UUID field empty
 * (in FF-A 1.0 UUID is not provided by the partition descriptor)
 *
 * This function is called at the device probing level.
 * ffa_cache_partitions_info uses ffa_query_partitions_info to get the data
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int ffa_cache_partitions_info(void)
{
	return ffa_query_partitions_info(NULL, NULL);
}

/**
 * ffa_msg_send_direct_req - FFA_MSG_SEND_DIRECT_{REQ,RESP} handler function
 * @dst_part_id: destination partition ID
 * @msg: pointer to the message data preallocated by the client (in/out)
 * @is_smc64: select 64-bit or 32-bit FF-A ABI
 *
 * This is the runtime function that implements FFA_MSG_SEND_DIRECT_{REQ,RESP}
 * FF-A functions.
 *
 * FFA_MSG_SEND_DIRECT_REQ is used to send the data to the secure partition.
 * The response from the secure partition is handled by reading the
 * FFA_MSG_SEND_DIRECT_RESP arguments.
 *
 * The maximum size of the data that can be exchanged is 40 bytes which is
 * sizeof(struct ffa_send_direct_data) as defined by the FF-A specification 1.0
 * in the section relevant to FFA_MSG_SEND_DIRECT_{REQ,RESP}
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int __ffa_runtime ffa_msg_send_direct_req(u16 dst_part_id, struct ffa_send_direct_data *msg,
						 u8 is_smc64)
{
	ffa_value_t res = {0};
	int ffa_errno;
	u64 req_mode, resp_mode;

	if (!ffa_priv_data || !ffa_priv_data->invoke_ffa_fn)
		return -EINVAL;

	/* No partition installed */
	if (!ffa_priv_data->partitions.count || !ffa_priv_data->partitions.descs)
		return -ENODEV;

	if(is_smc64) {
		req_mode = FFA_SMC_64(FFA_MSG_SEND_DIRECT_REQ);
		resp_mode = FFA_SMC_64(FFA_MSG_SEND_DIRECT_RESP);
	} else {
		req_mode = FFA_SMC_32(FFA_MSG_SEND_DIRECT_REQ);
		resp_mode = FFA_SMC_32(FFA_MSG_SEND_DIRECT_RESP);
	}

	ffa_priv_data->invoke_ffa_fn((ffa_value_t){
			.a0 = req_mode,
			.a1 = PREP_SELF_ENDPOINT_ID(ffa_priv_data->id) |
				PREP_PART_ENDPOINT_ID(dst_part_id),
			.a2 = 0,
			.a3 = msg->data0,
			.a4 = msg->data1,
			.a5 = msg->data2,
			.a6 = msg->data3,
			.a7 = msg->data4,
			}, &res);

	while (res.a0 == FFA_SMC_32(FFA_INTERRUPT))
		ffa_priv_data->invoke_ffa_fn((ffa_value_t){
			.a0 = FFA_SMC_32(FFA_RUN),
			.a1 = res.a1, .a2 = 0, .a3 = 0, .a4 = 0, .a5 = 0, .a6 = 0, .a7 = 0,
			}, &res);

	if (res.a0 == FFA_SMC_32(FFA_SUCCESS)) {
		/* Message sent with no response */
		return 0;
	}

	if (res.a0 == resp_mode){
		/*
		 * Message sent with response
		 * extract the return data
		 */
		msg->data0 = res.a3;
		msg->data1 = res.a4;
		msg->data2 = res.a5;
		msg->data3 = res.a6;
		msg->data4 = res.a7;

		return 0;
	}

	ffa_errno = (int)res.a2;
	return ffa_to_std_errno(ffa_errno);
}

#if !CONFIG_IS_ENABLED(SANDBOX_FFA)
/**
 * __arm_ffa_fn_smc - SMC wrapper
 * @args: FF-A ABI arguments to be copied to Xn registers
 * @res: FF-A ABI return data to be copied from Xn registers
 *
 * Calls low level SMC assembly function
 *
 * Return: void
 */
void __ffa_runtime __arm_ffa_fn_smc(ffa_value_t args, ffa_value_t *res)
{
	arm_smccc_1_2_smc(&args, res);
}
#endif

/**
 * ffa_set_smc_conduit - Set the SMC conduit
 *
 * This boot time function selects the SMC conduit by setting the driver invoke function
 * to SMC assembly function
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int ffa_set_smc_conduit(void)
{
#if CONFIG_IS_ENABLED(SANDBOX_FFA)
		ffa_priv_data->invoke_ffa_fn = sandbox_arm_ffa_smccc_smc;
		ffa_info("Using SMC emulation");
#else
		ffa_priv_data->invoke_ffa_fn = __arm_ffa_fn_smc;
#endif

	if (!ffa_priv_data->invoke_ffa_fn) {
		ffa_err("failure to set the invoke function");
		return -EINVAL;
	}

	ffa_info("Conduit is SMC");

	return 0;
}

/**
 * ffa_set_bus_ops - Set the bus driver operations
 *
 * Setting the driver callbacks.
 *
 */
static void ffa_set_bus_ops(void)
{
	ffa_priv_data->ffa_ops.partition_info_get = ffa_get_partitions_info;
	ffa_priv_data->ffa_ops.sync_send_receive = ffa_msg_send_direct_req;
	ffa_priv_data->ffa_ops.rxtx_unmap = ffa_unmap_rxtx_buffers;
}

/**
 * ffa_alloc_prvdata - allocate the driver main data structure and sets the device
 * @dev:	the arm_ffa device
 *
 * This boot time function creates the main data structure embedding all the driver data.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int ffa_alloc_prvdata(struct udevice *dev)
{
	if (!dev) {
		ffa_err("no udevice found");
		return -ENODEV;
	}

	/* The device is registered with the DM. Let's create the driver main data structure*/

	ffa_priv_data = devm_kmalloc(dev, sizeof(struct ffa_prvdata), __GFP_ZERO);
	if (!ffa_priv_data) {
		ffa_err("can not allocate the driver main data structure");
		return -ENOMEM;
	}

	ffa_priv_data->dev = dev;

	return 0;
}

/**
 * ffa_probe - The driver probe function
 * @dev:	the arm_ffa device
 *
 * Probing is done at boot time and triggered by the uclass device discovery.
 * At probe level the following actions are done:
 *	- setting the conduit
 *	- querying the FF-A framework version
 *	- querying from secure world the u-boot endpoint ID
 *	- querying from secure world the supported features of FFA_RXTX_MAP
 *	- mapping the RX/TX buffers
 *	- querying from secure world all the partitions information
 *
 * All data queried from secure world is saved in the resident private data structure.
 *
 * The probe will fail if either FF-A framework is not detected or the
 * FF-A requests are not behaving correctly. This ensures that the
 * driver is not installed and its operations are not exported to the clients.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
static int ffa_probe(struct udevice *dev)
{
	int ret;

	ret = ffa_alloc_prvdata(dev);
	if (ret != 0)
		return ret;

	ffa_set_bus_ops();

	ret = ffa_set_smc_conduit();
	if (ret != 0)
		return ret;

	ret = ffa_get_version();
	if (ret != 0)
		return ret;

	ret = ffa_get_endpoint_id();
	if (ret != 0)
		return ret;

	ret = ffa_get_rxtx_map_features();
	if (ret != 0)
		return ret;

	ret = ffa_map_rxtx_buffers();
	if (ret != 0)
		return ret;

	ret = ffa_cache_partitions_info();
	if (ret != 0) {
		ffa_free_rxtx_buffers();
		return ret;
	}

	return 0;
}

/**
 * ffa_remove - The driver remove function
 * @dev:	the arm_ffa device
 * When the device is about to be removed ,  unmap the RX/TX buffers and free the memory
 * Return:
 *
 * 0 on success.
 */
static int ffa_remove(struct udevice *dev)
{
	ffa_info("removing the device");

	ffa_unmap_rxtx_buffers();

	if (ffa_priv_data->pair.rxbuf || ffa_priv_data->pair.txbuf)
		ffa_free_rxtx_buffers();

	return 0;
}

/**
 * ffa_unbind - The driver unbind function
 * @dev:	the arm_ffa device
 * After the device is removed and memory freed the device is unbound
 * Return:
 *
 * 0 on success.
 */
static int ffa_unbind(struct udevice *dev)
{
	ffa_info("unbinding the device , private data already released");

	ffa_priv_data = NULL;

	return 0;
}

/**
 * ffa_bus_ops_get - bus driver operations getter
 *
 * Return:
 * This runtime function returns a pointer to the driver operations structure
 */
const struct ffa_bus_ops * __ffa_runtime ffa_bus_ops_get(void)
{
	return &ffa_priv_data->ffa_ops;
}

/**
 * ffa_bus_prvdata_get - bus driver private data getter
 *
 * Return:
 * This boot time function returns a pointer to the main private data structure
 */
struct ffa_prvdata **ffa_bus_prvdata_get(void)
{
	return &ffa_priv_data;
}

/**
 * ffa_bus_discover - discover FF-A bus and probe arm_ffa and sandbox_arm_ffa devices
 *
 * This boot time function makes sure the FF-A bus is discoverable.
 * Then, the arm_ffa and sandbox_arm_ffa devices are ready to use.
 *
 * This function is called automatically at initcalls
 * level (after u-boot relocation).
 *
 * When the bus was already discovered successfully the discovery will not run again.
 *
 * Arm FF-A transport is implemented through arm_ffa u-boot device managing the FF-A
 * communication. In Sandbox mode sandbox_arm_ffa is used to test arm_ffa driver.
 * All FF-A clients should use the arm_ffa device to use the FF-A transport.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
int ffa_bus_discover(void)
{
	int ret = 0;

	if (!ffa_priv_data) {
		ret = ffa_device_get();

#if CONFIG_IS_ENABLED(SANDBOX_FFA)
		if (ret == 0)
			ret = sandbox_ffa_device_get();
#endif
	}

	return ret;
}

/**
 * Declaring the arm_ffa driver under UCLASS_FFA
 */

U_BOOT_DRIVER(arm_ffa) = {
	.name		= FFA_DRV_NAME,
	.id		= UCLASS_FFA,
	.probe		= ffa_probe,
	.remove		= ffa_remove,
	.unbind		= ffa_unbind,
};
