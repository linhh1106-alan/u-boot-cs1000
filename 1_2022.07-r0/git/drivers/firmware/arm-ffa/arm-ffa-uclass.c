// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 ARM Limited
 * Abdellatif El Khlifi <abdellatif.elkhlifi@arm.com>
 */

#include <common.h>
#include <dm.h>
#include <asm/global_data.h>

DECLARE_GLOBAL_DATA_PTR;

UCLASS_DRIVER(ffa) = {
	.name		= "ffa",
	.id		= UCLASS_FFA,
};
