// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 ARM Limited
 * Abdellatif El Khlifi <abdellatif.elkhlifi@arm.com>
 */

#include <arm_ffa.h>
#include <asm/io.h>
#include <common.h>
#include <command.h>
#include <dm.h>
#include <mapmem.h>
#include <stdlib.h>

/**
 * do_ffa_get_singular_partition_info - implementation of the getpart subcommand
 * @cmdtp:		Command Table
 * @flag:		flags
 * @argc:		number of arguments
 * @argv:		arguments
 *
 * This function queries the secure partition information which the UUID is provided
 * as an argument. The function uses the arm_ffa driver partition_info_get operation
 * to retrieve the data.
 * The input UUID string is expected to be in big endian format.
 *
 * Return:
 *
 * CMD_RET_SUCCESS: on success, otherwise failure
 */
static int do_ffa_get_singular_partition_info(struct cmd_tbl *cmdtp, int flag, int argc,
					      char *const argv[])
{
	u32 count = 0, size = 0;
	int ret;
	struct ffa_partition_info *parts_info;
	u32 info_idx;

	if (argc != 1)
		return -EINVAL;

	/* Mode 1: getting the number of secure partitions */
	ret = ffa_bus_ops_get()->partition_info_get(argv[0], &count, NULL);
	if (ret != 0) {
		ffa_err("Failure in querying partitions count (error code: %d)", ret);
		return ret;
	}

	if (!count) {
		ffa_info("No secure partition found");
		return ret;
	}

	/*
	 * pre-allocate a buffer to be filled by the driver
	 * with ffa_partition_info structs
	 */

	ffa_info("Pre-allocating %d partition(s) info structures", count);

	parts_info = calloc(count, sizeof(struct ffa_partition_info));
	if (!parts_info)
		return -EINVAL;

	size = count * sizeof(struct ffa_partition_info);

	/*
	 * ask the driver to fill the buffer with the SPs info
	 */

	ret = ffa_bus_ops_get()->partition_info_get(argv[0], &size, parts_info);
	if (ret != 0) {
		ffa_err("Failure in querying partition(s) info (error code: %d)", ret);
		free(parts_info);
		return ret;
	}

	/*
	 * SPs found , show the partition information
	 */
	for (info_idx = 0; info_idx < count ; info_idx++) {
		ffa_info("Partition: id = 0x%x , exec_ctxt 0x%x , properties 0x%x",
			 parts_info[info_idx].id,
			 parts_info[info_idx].exec_ctxt,
			 parts_info[info_idx].properties);
	}

	free(parts_info);

	return 0;
}

/**
 * do_ffa_msg_send_direct_req - implementation of the ping subcommand
 * @cmdtp:		Command Table
 * @flag:		flags
 * @argc:		number of arguments
 * @argv:		arguments
 *
 * This function sends data to the secure partition which the ID is provided
 * as an argument. The function uses the arm_ffa driver sync_send_receive operation
 * to send data.
 *
 * Return:
 *
 * CMD_RET_SUCCESS: on success, otherwise failure
 */
int  do_ffa_msg_send_direct_req(struct cmd_tbl *cmdtp, int flag, int argc,
				char *const argv[])
{
	struct ffa_send_direct_data msg = {
			.data0 = 0xaaaaaaaa,
			.data1 = 0xbbbbbbbb,
			.data2 = 0xcccccccc,
			.data3 = 0xdddddddd,
			.data4 = 0xeeeeeeee,
	};
	u16 part_id;
	int ret;

	if (argc != 1)
		return -EINVAL;

	errno = 0;
	part_id = strtoul(argv[0], NULL, 16);

	if (errno) {
		ffa_err("Invalid partition ID");
		return -EINVAL;
	}

	ret = ffa_bus_ops_get()->sync_send_receive(part_id, &msg, 1);
	if (ret == 0) {
		u8 cnt;

		ffa_info("SP response:\n[LSB]");
		for (cnt = 0;
		     cnt < sizeof(struct ffa_send_direct_data) / sizeof(u64);
		     cnt++)
			ffa_info("0x%llx", ((u64 *)&msg)[cnt]);
	} else {
		ffa_err("Sending direct request error (%d)", ret);
	}

	return ret;
}

/**
 *do_ffa_dev_list - implementation of the devlist subcommand
 * @cmdtp: [in]		Command Table
 * @flag:		flags
 * @argc:		number of arguments
 * @argv:		arguments
 *
 * This function queries the devices belonging to the UCLASS_FFA
 * class. Currently, one device is expected to show up: the arm_ffa device
 *
 * Return:
 *
 * CMD_RET_SUCCESS: on success, otherwise failure
 */
int do_ffa_dev_list(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	struct udevice *dev = NULL;
	int i, ret;

	ffa_info("arm_ffa uclass entries:");

	for (i = 0, ret = uclass_first_device(UCLASS_FFA, &dev);
	     dev;
	     ret = uclass_next_device(&dev), i++) {
		if (ret)
			break;

		ffa_info("entry %d - instance %08x, ops %08x, plat %08x",
			 i,
			 (u32)map_to_sysmem(dev),
			 (u32)map_to_sysmem(dev->driver->ops),
			 (u32)map_to_sysmem(dev_get_plat(dev)));
	}

	return cmd_process_error(cmdtp, ret);
}

static struct cmd_tbl armffa_commands[] = {
	U_BOOT_CMD_MKENT(getpart, 1, 1, do_ffa_get_singular_partition_info, "", ""),
	U_BOOT_CMD_MKENT(ping, 1, 1, do_ffa_msg_send_direct_req, "", ""),
	U_BOOT_CMD_MKENT(devlist, 0, 1, do_ffa_dev_list, "", ""),
};

/**
 * do_armffa - the armffa command main function
 * @cmdtp:	Command Table
 * @flag:		flags
 * @argc:		number of arguments
 * @argv:		arguments
 *
 * This function identifies which armffa subcommand to run.
 * Then, it makes sure the arm_ffa device is probed and
 * ready for use.
 * Then, it runs the subcommand.
 *
 * Return:
 *
 * CMD_RET_SUCCESS: on success, otherwise failure
 */
static int do_armffa(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	struct cmd_tbl *armffa_cmd;
	int ret;

	if (argc < 2)
		return CMD_RET_USAGE;

	armffa_cmd = find_cmd_tbl(argv[1], armffa_commands, ARRAY_SIZE(armffa_commands));

	argc -= 2;
	argv += 2;

	if (!armffa_cmd || argc > armffa_cmd->maxargs)
		return CMD_RET_USAGE;

	ret = ffa_bus_discover();
	if (ret != 0)
		return cmd_process_error(cmdtp, ret);

	if (!ffa_bus_ops_get())
		return -EINVAL;

	ret = armffa_cmd->cmd(armffa_cmd, flag, argc, argv);

	return cmd_process_error(armffa_cmd, ret);
}

U_BOOT_CMD(armffa, 4, 1, do_armffa,
	   "Arm FF-A operations test command",
	   "getpart <partition UUID>\n"
	   "	 - lists the partition(s) info\n"
	   "ping <partition ID>\n"
	   "	 - sends a data pattern to the specified partition\n"
	   "devlist\n"
	   "	 - displays the arm_ffa device info\n");
