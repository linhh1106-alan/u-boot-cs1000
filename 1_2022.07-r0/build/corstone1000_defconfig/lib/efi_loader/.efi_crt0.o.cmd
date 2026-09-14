cmd_lib/efi_loader/efi_crt0.o := aarch64-poky-linux-musl-gcc  --sysroot=/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot -Wp,-MD,lib/efi_loader/.efi_crt0.o.d -nostdinc -isystem /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot-native/usr/bin/aarch64-poky-linux-musl/../../lib/aarch64-poky-linux-musl/gcc/aarch64-poky-linux-musl/12.2.0/include -Iinclude  -I/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include  -I/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/include -include /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/linux/kconfig.h -D__KERNEL__ -D__UBOOT__ -D__ASSEMBLY__ -fno-PIE -g -DHOST_ARCH="0xaa64"  -I/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/. -D__ARM__ -fno-pic -mstrict-align -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -mgeneral-regs-only -fno-common -ffixed-x18 -pipe -march=armv8-a+crc -D__LINUX_ARM_ARCH__=8   -c -o lib/efi_loader/efi_crt0.o /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/crt0_aarch64_efi.S

source_lib/efi_loader/efi_crt0.o := /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/crt0_aarch64_efi.S

deps_lib/efi_loader/efi_crt0.o := \
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
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/asm-generic/pe.h \

lib/efi_loader/efi_crt0.o: $(deps_lib/efi_loader/efi_crt0.o)

$(deps_lib/efi_loader/efi_crt0.o):
