cmd_lib/errno.o := aarch64-poky-linux-musl-gcc  --sysroot=/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot -Wp,-MD,lib/.errno.o.d -nostdinc -isystem /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot-native/usr/bin/aarch64-poky-linux-musl/../../lib/aarch64-poky-linux-musl/gcc/aarch64-poky-linux-musl/12.2.0/include -Iinclude  -I/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include  -I/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/include -include /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/linux/kconfig.h  -I/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/lib -Ilib -D__KERNEL__ -D__UBOOT__ -Wall -Wstrict-prototypes -Wno-format-security -fno-builtin -ffreestanding -std=gnu11 -fshort-wchar -fno-strict-aliasing -fno-PIE -Os -fno-stack-protector -fno-delete-null-pointer-checks -Wno-pointer-sign -Wno-stringop-truncation -Wno-zero-length-bounds -Wno-array-bounds -Wno-stringop-overflow -Wno-maybe-uninitialized -fmacro-prefix-map=/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/= -g -fstack-usage -Wno-format-nonliteral -Wno-address-of-packed-member -Wno-unused-but-set-variable -Werror=date-time -Wno-packed-not-aligned -D__ARM__ -fno-pic -mstrict-align -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -mgeneral-regs-only -fno-common -ffixed-x18 -pipe -march=armv8-a+crc -D__LINUX_ARM_ARCH__=8    -DKBUILD_BASENAME='"errno"'  -DKBUILD_MODNAME='"errno"' -c -o lib/errno.o /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/lib/errno.c

source_lib/errno.o := /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/lib/errno.c

deps_lib/errno.o := \
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
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/errno.h \
    $(wildcard include/config/errno/str.h) \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/linux/errno.h \

lib/errno.o: $(deps_lib/errno.o)

$(deps_lib/errno.o):
