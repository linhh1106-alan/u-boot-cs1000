CONFIG_SYS_CBSIZE=512
CONFIG_SKIP_LOWLEVEL_INIT=y
CONFIG_PL011_CLOCK=50000000
CONFIG_SYS_MAXARGS=64
CONFIG_EXTRA_ENV_SETTINGS="BOOTENV "usb_pgood_delay=2500boot_bank_flag=0x080020000kernel_addr_bank_0=0x083EE0000kernel_addr_bank_1=0x0936E0000retrieve_kernel_load_addr= "if itest.l *${boot_bank_flag} == 0; then setenv kernel_addr $kernel_addr_bank_0;else setenv kernel_addr $kernel_addr_bank_1;fi;0kernel_addr_r=0x882000000fdt_high=0xffffffff0"
CONFIG_SYS_BOOT_RAMDISK_HIGH=y
CONFIG_SYS_BAUDRATE_TABLE="{ 9600, 19200, 38400, 57600, 115200 }"
CONFIG_SYS_SDRAM_BASE="PHYS_SDRAM_1"
CONFIG_SYS_INIT_SP_ADDR="(CONFIG_SYS_SDRAM_BASE + 0x03f00000)"
CONFIG_SYS_PBSIZE="(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)"
CONFIG_ENV_SECT_SIZE="SZ_64K"
CONFIG_BOARDDIR="board/armltd/corstone1000"
