// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 ARM Limited
 * Abdellatif El Khlifi <abdellatif.elkhlifi@arm.com>
 */

#include "arm_ffa_prv.h"

/**
 * ffa_copy_runtime_data - copy the private data structure  to the runtime area
 *
 * This boot time function copies the arm_ffa driver data structures including
 *  partitions data to the EFI runtime data section.
 *
 * Return:
 *
 * 0 on success. Otherwise, failure
 */
efi_status_t ffa_copy_runtime_data(void)
{
	efi_status_t efi_ret;
	efi_uintn_t prvdata_pages;
	efi_uintn_t descs_pages;
	struct ffa_prvdata **prvdata = NULL; /* Pointer to the current structure */
	struct ffa_prvdata *runtime_prvdata = NULL; /* Pointer to the structure runtime copy */
	u64 runtime_descs = 0;

	prvdata = ffa_bus_prvdata_get();

	printf("INFO: EFI: FFA: prv data area at 0x%llx\n", (u64)prvdata);

	/* allocate private data runtime area */

	prvdata_pages = efi_size_in_pages(sizeof(struct ffa_prvdata));
	efi_ret = efi_allocate_pages(EFI_ALLOCATE_ANY_PAGES,
				     EFI_RUNTIME_SERVICES_DATA,
				     prvdata_pages,
				     (u64 *)&runtime_prvdata);

	if (efi_ret != EFI_SUCCESS) {
		printf("ERROR: EFI: FFA: allocating runtime data (err: 0x%lx, addr 0x%llx)\n",
		       efi_ret, (u64)runtime_prvdata);

		return efi_ret;
	}

	printf("INFO: EFI: FFA: runtime data area at 0x%llx\n", (u64)runtime_prvdata);

	if (!runtime_prvdata)
		return EFI_INVALID_PARAMETER;

	/* allocate the partition data runtime area */

	descs_pages = efi_size_in_pages((*prvdata)->partitions.count *
					sizeof(struct ffa_partition_desc));
	efi_ret = efi_allocate_pages(EFI_ALLOCATE_ANY_PAGES,
				     EFI_RUNTIME_SERVICES_DATA,
				     descs_pages,
				     &runtime_descs);

	if (efi_ret != EFI_SUCCESS) {
		printf("ERROR: EFI: FFA: allocating runtime SPs data (err: 0x%lx, addr 0x%llx)\n",
		       efi_ret, runtime_descs);

		efi_free_pages((u64)runtime_prvdata, prvdata_pages);

		return efi_ret;
	}

	printf("INFO: EFI: FFA: SPs runtime area at 0x%llx\n", (u64)runtime_descs);

	if (!runtime_descs)
		return EFI_INVALID_PARAMETER;

	*runtime_prvdata = **prvdata;

	runtime_prvdata->dev = NULL;
	runtime_prvdata->ffa_ops.partition_info_get = NULL;
	runtime_prvdata->ffa_ops.rxtx_unmap = NULL;
	runtime_prvdata->partitions.descs = (struct ffa_partition_desc *)runtime_descs;
	runtime_prvdata->pair.rxbuf = 0;
	runtime_prvdata->pair.txbuf = 0;

	/*
	 * Update the private data structure pointer in the driver
	 * no need to free the old structure. devm takes care of that
	 */
	*prvdata = runtime_prvdata;

	printf("INFO: EFI: FFA: runtime prv data now at 0x%llx , SPs count %d\n",
	       (u64)*prvdata, (*prvdata)->partitions.count);

	return 0;
}
