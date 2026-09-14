// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 ARM Limited
 * (C) Copyright 2022 Linaro
 * Rui Miguel Silva <rui.silva@linaro.org>
 */

#include <common.h>
#include <dm.h>
#include <dm/platform_data/serial_pl01x.h>
#include <asm/armv8/mmu.h>
#include <asm/global_data.h>


static const struct pl01x_serial_plat serial_plat = {
	.base = V2M_UART0,
	.type = TYPE_PL011,
	.clock = CONFIG_PL011_CLOCK,
};

U_BOOT_DRVINFO(corstone1000_serials) = {
	.name = "serial_pl01x",
	.plat = &serial_plat,
};

static struct mm_region corstone1000_mem_map[] = {
	{
		/* CVM */
		.virt = 0x02000000UL,
		.phys = 0x02000000UL,
		.size = 0x02000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		/* QSPI */
		.virt = 0x08000000UL,
		.phys = 0x08000000UL,
		.size = 0x08000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		/* Host Peripherals */
		.virt = 0x1A000000UL,
		.phys = 0x1A000000UL,
		.size = 0x26000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* USB */
		.virt = 0x40200000UL,
			.phys = 0x40200000UL,
			.size = 0x00100000UL,
			.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
				PTE_BLOCK_NON_SHARE |
				PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* MMC0 */
		.virt = 0x40300000UL,
		.phys = 0x40300000UL,
		.size = 0x00100000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
				 PTE_BLOCK_NON_SHARE |
				 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* ethernet */
		.virt = 0x40100000UL,
			.phys = 0x40100000UL,
			.size = 0x00100000UL,
			.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
				PTE_BLOCK_NON_SHARE |
				PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* MMC1 */
		.virt = 0x50000000UL,
		.phys = 0x50000000UL,
		.size = 0x00100000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
				 PTE_BLOCK_NON_SHARE |
				 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* OCVM */
		.virt = 0x80000000UL,
		.phys = 0x80000000UL,
		.size = 0x80000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = corstone1000_mem_map;

void set_dfu_alt_info(char *interface, char *devstr)
{
}

int board_init(void)
{
	return 0;
}

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;

	return 0;
}

int dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

	return 0;
}

/*
 * Board specific ethernet initialization routine.
 * */
int board_eth_init(struct bd_info *bis)
{
	int rc = 0;

#ifndef CONFIG_DM_ETH
#ifdef CONFIG_SMC91111
	rc = smc91111_initialize(0, CONFIG_SMC91111_BASE);
#endif
#ifdef CONFIG_SMC911X
	rc = smc911x_initialize(0, CONFIG_SMC911X_BASE);
#endif
#endif

	return rc;
}

void reset_cpu(ulong addr)
{
}
