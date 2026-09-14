/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2022 ARM Limited
 * (C) Copyright 2022 Linaro
 * Rui Miguel Silva <rui.silva@linaro.org>
 * Abdellatif El Khlifi <abdellatif.elkhlifi@arm.com>
 *
 * Configuration for Corstone1000. Parts were derived from other ARM
 * configurations.
 */

#ifndef __CORSTONE1000_H
#define __CORSTONE1000_H

#include <linux/sizes.h>

/* The SE Proxy partition ID*/
#define CORSTONE1000_SEPROXY_PART_ID		(0x8002)

/* Update service ID provided by  the SE Proxy secure partition*/
#define CORSTONE1000_SEPROXY_UPDATE_SVC_ID	(0x4)

/* Notification events used with SE Proxy update service */
#define CORSTONE1000_BUFFER_READY_EVT		(0x1)
#define CORSTONE1000_UBOOT_EFI_STARTED_EVT	(0x2)

#define PREP_SEPROXY_SVC_ID_MASK	GENMASK(31, 16)
#define PREP_SEPROXY_SVC_ID(x)	 (FIELD_PREP(PREP_SEPROXY_SVC_ID_MASK, (x)))

#define PREP_SEPROXY_EVT_MASK		GENMASK(15, 0)
#define PREP_SEPROXY_EVT(x)	(FIELD_PREP(PREP_SEPROXY_EVT_MASK, (x)))

/* Size in 4KB pages of the EFI capsule buffer */
#define CORSTONE1000_CAPSULE_BUFFER_SIZE	(8192) /* 32 MB */

/* Capsule GUID */
#define EFI_CORSTONE1000_CAPSULE_ID_GUID \
	EFI_GUID(0x3a770ddc, 0x409b, 0x48b2, 0x81, 0x41, \
		 0x93, 0xb7, 0xc6, 0x0b, 0x20, 0x9e)

#define FFA_SHARED_MM_BUFFER_SIZE	SZ_4K /* 4 KB */

/*
 * shared buffer physical address used for communication between
 * u-boot and the MM SP
 */
#define FFA_SHARED_MM_BUFFER_ADDR	(0x02000000)
#define FFA_SHARED_MM_BUFFER_OFFSET	(0)

#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x03f00000)
#define CONFIG_SKIP_LOWLEVEL_INIT

#define CONFIG_SYS_HZ		1000

#define V2M_SRAM0		0x02000000
#define V2M_QSPI		0x08000000

#define V2M_DEBUG		0x10000000
#define V2M_BASE_PERIPH		0x1A000000

#define V2M_BASE		0x80000000

#define V2M_PERIPH_OFFSET(x)	(x << 16)

#define V2M_SYSID		(V2M_BASE_PERIPH)
#define V2M_SYSCTL		(V2M_BASE_PERIPH + V2M_PERIPH_OFFSET(1))

#define V2M_COUNTER_CTL		(V2M_BASE_PERIPH + V2M_PERIPH_OFFSET(32))
#define V2M_COUNTER_READ	(V2M_BASE_PERIPH + V2M_PERIPH_OFFSET(33))

#define V2M_TIMER_CTL		(V2M_BASE_PERIPH + V2M_PERIPH_OFFSET(34))
#define V2M_TIMER_BASE0		(V2M_BASE_PERIPH + V2M_PERIPH_OFFSET(35))

#define V2M_UART0		(V2M_BASE_PERIPH + V2M_PERIPH_OFFSET(81))
#define V2M_UART1		(V2M_BASE_PERIPH + V2M_PERIPH_OFFSET(82))

#define CONFIG_PL011_CLOCK	50000000

/* Physical Memory Map */
#define PHYS_SDRAM_1		(V2M_BASE)
#define PHYS_SDRAM_1_SIZE	0x80000000

#define CONFIG_ENV_SECT_SIZE	SZ_64K

#define CONFIG_SYS_SDRAM_BASE	PHYS_SDRAM_1

/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE	512	/* Console I/O Buffer Size */
#define CONFIG_SYS_MAXARGS	64	/* max command args */

#define BOOT_TARGET_DEVICES(func) \
	func(USB, usb, 0) \
	func(MMC, mmc, 0) \
	func(MMC, mmc, 1)

#include <config_distro_bootcmd.h>

#define CONFIG_EXTRA_ENV_SETTINGS							\
				BOOTENV							\
				"usb_pgood_delay=250\0"					\
				"boot_bank_flag=0x08002000\0"				\
				"kernel_addr_bank_0=0x083EE000\0"			\
				"kernel_addr_bank_1=0x0936E000\0"			\
				"retrieve_kernel_load_addr="				\
					"if itest.l *${boot_bank_flag} == 0; then "	\
					    "setenv kernel_addr $kernel_addr_bank_0;"	\
					"else "						\
					    "setenv kernel_addr $kernel_addr_bank_1;"	\
					"fi;"						\
					"\0"						\
				"kernel_addr_r=0x88200000\0"				\
				"fdt_high=0xffffffff\0"

/*
 * config_distro_bootcmd define the boot command to distro_bootcmd, but we here
 * want to first try to load a kernel if exists, override that config then
 */
#undef CONFIG_BOOTCOMMAND

#define CONFIG_BOOTCOMMAND								\
				"run retrieve_kernel_load_addr;"			\
				"echo Loading kernel from $kernel_addr to memory ... ;"	\
				"unzip $kernel_addr 0x90000000;"	\
				"loadm 0x90000000 $kernel_addr_r 0xf00000;"		\
				"usb start; usb reset;"					\
				"run distro_bootcmd;"					\
				"bootefi $kernel_addr_r $fdtcontroladdr;"
#endif
