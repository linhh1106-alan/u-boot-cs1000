cmd_arch/arm/cpu/armv8/exceptions.o := aarch64-poky-linux-musl-gcc  --sysroot=/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot -Wp,-MD,arch/arm/cpu/armv8/.exceptions.o.d -nostdinc -isystem /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot-native/usr/bin/aarch64-poky-linux-musl/../../lib/aarch64-poky-linux-musl/gcc/aarch64-poky-linux-musl/12.2.0/include -Iinclude  -I/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include  -I/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/include -include /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/linux/kconfig.h -D__KERNEL__ -D__UBOOT__ -D__ASSEMBLY__ -fno-PIE -g -D__ARM__ -fno-pic -mstrict-align -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -mgeneral-regs-only -fno-common -ffixed-x18 -pipe -march=armv8-a+crc -D__LINUX_ARM_ARCH__=8   -c -o arch/arm/cpu/armv8/exceptions.o /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/cpu/armv8/exceptions.S

source_arch/arm/cpu/armv8/exceptions.o := /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/cpu/armv8/exceptions.S

deps_arch/arm/cpu/armv8/exceptions.o := \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/linux/kconfig.h \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
    $(wildcard include/config/spl/.h) \
    $(wildcard include/config/tpl/.h) \
    $(wildcard include/config/tools/.h) \
    $(wildcard include/config/tpl/build.h) \
    $(wildcard include/config/vpl/build.h) \
    $(wildcard include/config/spl/build.h) \
    $(wildcard include/config/tools/foo.h) \
    $(wildcard include/config/spl/foo.h) \
    $(wildcard include/config/tpl/foo.h) \
    $(wildcard include/config/vpl/foo.h) \
    $(wildcard include/config/option.h) \
    $(wildcard include/config/acme.h) \
    $(wildcard include/config/spl/acme.h) \
    $(wildcard include/config/tpl/acme.h) \
    $(wildcard include/config/if/enabled/int.h) \
    $(wildcard include/config/int/option.h) \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/asm-offsets.h \
  include/generated/generic-asm-offsets.h \
  include/config.h \
    $(wildcard include/config/boarddir.h) \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/config_uncmd_spl.h \
    $(wildcard include/config/spl/dm.h) \
    $(wildcard include/config/dm/serial.h) \
    $(wildcard include/config/dm/i2c.h) \
    $(wildcard include/config/dm/stdio.h) \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/configs/corstone1000.h \
    $(wildcard include/config/sys/init/sp/addr.h) \
    $(wildcard include/config/sys/sdram/base.h) \
    $(wildcard include/config/skip/lowlevel/init.h) \
    $(wildcard include/config/sys/hz.h) \
    $(wildcard include/config/pl011/clock.h) \
    $(wildcard include/config/env/sect/size.h) \
    $(wildcard include/config/sys/cbsize.h) \
    $(wildcard include/config/sys/maxargs.h) \
    $(wildcard include/config/extra/env/settings.h) \
    $(wildcard include/config/bootcommand.h) \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/linux/sizes.h \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/linux/const.h \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/config_distro_bootcmd.h \
    $(wildcard include/config/cmd/mmc.h) \
    $(wildcard include/config/sandbox.h) \
    $(wildcard include/config/cmd/ubifs.h) \
    $(wildcard include/config/efi/loader.h) \
    $(wildcard include/config/arm64.h) \
    $(wildcard include/config/arm.h) \
    $(wildcard include/config/x86/run/32bit.h) \
    $(wildcard include/config/x86/run/64bit.h) \
    $(wildcard include/config/arch/rv32i.h) \
    $(wildcard include/config/arch/rv64i.h) \
    $(wildcard include/config/cmd/bootefi/bootmgr.h) \
    $(wildcard include/config/sata.h) \
    $(wildcard include/config/nvme.h) \
    $(wildcard include/config/scsi.h) \
    $(wildcard include/config/ide.h) \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/cmd/usb.h) \
    $(wildcard include/config/cmd/virtio.h) \
    $(wildcard include/config/cmd/dhcp.h) \
    $(wildcard include/config/x86.h) \
    $(wildcard include/config/cmd/pxe.h) \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/include/asm/config.h \
    $(wildcard include/config/sys/boot/ramdisk/high.h) \
    $(wildcard include/config/arch/ls1021a.h) \
    $(wildcard include/config/cpu/pxa27x.h) \
    $(wildcard include/config/cpu/monahans.h) \
    $(wildcard include/config/cpu/pxa25x.h) \
    $(wildcard include/config/fsl/layerscape.h) \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/linux/kconfig.h \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/config_fallbacks.h \
    $(wildcard include/config/spl.h) \
    $(wildcard include/config/spl/pad/to.h) \
    $(wildcard include/config/spl/max/size.h) \
    $(wildcard include/config/sys/baudrate/table.h) \
    $(wildcard include/config/cmd/kgdb.h) \
    $(wildcard include/config/sys/pbsize.h) \
    $(wildcard include/config/sys/prompt.h) \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/include/asm/ptrace.h \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/include/asm/proc-armv/ptrace.h \
    $(wildcard include/config/sys/thumb/build.h) \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/include/asm/macro.h \
    $(wildcard include/config/armv8/multientry.h) \
    $(wildcard include/config/armv8/psci.h) \
    $(wildcard include/config/armv8/ea/el3/first.h) \
    $(wildcard include/config/gicv3.h) \
    $(wildcard include/config/gicv2.h) \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/include/asm/system.h \
    $(wildcard include/config/armv7/lpae.h) \
    $(wildcard include/config/cpu/v7a.h) \
    $(wildcard include/config/sys/arm/cache/writethrough.h) \
    $(wildcard include/config/sys/arm/cache/writealloc.h) \
    $(wildcard include/config/sys/arm/cache/writeback.h) \
    $(wildcard include/config/armv7/psci.h) \
    $(wildcard include/config/sys/noncached/memory.h) \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/stack/validation.h) \
    $(wildcard include/config/kasan.h) \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/linux/compiler_types.h \
    $(wildcard include/config/have/arch/compiler/h.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/cc/has/asm/inline.h) \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/include/asm/barriers.h \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/linux/linkage.h \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/include/asm/linkage.h \

arch/arm/cpu/armv8/exceptions.o: $(deps_arch/arm/cpu/armv8/exceptions.o)

$(deps_arch/arm/cpu/armv8/exceptions.o):
