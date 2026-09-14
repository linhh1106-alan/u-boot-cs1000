// SPDX-License-Identifier: GPL-2.0+
/*
 * Test for armffa command
 *
 * (C) Copyright 2022 ARM Limited
 * Abdellatif El Khlifi <abdellatif.elkhlifi@arm.com>
 */

#include <common.h>
#include <dm.h>
#include <dm/test.h>
#include <sandbox_arm_ffa.h>
#include <string.h>
#include <test/test.h>
#include <test/ut.h>

#define PING_CMD_SIZE 19

/* Basic test of 'armffa' command */
static int dm_test_armffa_cmd(struct unit_test_state *uts)
{
	char ping_cmd[PING_CMD_SIZE] = {0};

	ut_assertok(ffa_bus_discover());

	/* armffa getpart <UUID> */
	ut_assertok(run_command("armffa getpart " SANDBOX_SERVICE1_UUID, 0));

	snprintf(ping_cmd, PING_CMD_SIZE, "armffa ping 0x%x", SANDBOX_SP1_ID);

	/* armffa ping <ID> */
	ut_assertok(run_command(ping_cmd, 0));

	/* armffa devlist */
	ut_assertok(run_command("armffa devlist", 0));

	return CMD_RET_SUCCESS;
}

DM_TEST(dm_test_armffa_cmd, UT_TESTF_SCAN_FDT | UT_TESTF_CONSOLE_REC);
