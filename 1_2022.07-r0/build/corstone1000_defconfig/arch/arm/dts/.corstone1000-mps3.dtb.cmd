cmd_arch/arm/dts/corstone1000-mps3.dtb := mkdir -p arch/arm/dts/ ; (cat /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/dts/corstone1000-mps3.dts; ) > arch/arm/dts/.corstone1000-mps3.dtb.pre.tmp;  gcc  -isystem/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot-native/usr/include -O2 -pipe -L/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot-native/usr/lib                         -L/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot-native/lib                         -Wl,--enable-new-dtags                         -Wl,-rpath-link,/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot-native/usr/lib                         -Wl,-rpath-link,/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot-native/lib                         -Wl,-rpath,/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot-native/usr/lib                         -Wl,-rpath,/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot-native/lib                         -Wl,-O1 -Wl,--allow-shlib-undefined -Wl,--dynamic-linker=/work/build-cs1000/tmp/sysroots-uninative/aarch64-linux/lib/ld-linux-aarch64.so.1 -E -Wp,-MD,arch/arm/dts/.corstone1000-mps3.dtb.d.pre.tmp -nostdinc -Iinclude  -I/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include  -I/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/include -include /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/linux/kconfig.h -I/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/dts -I/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/dts/include -I/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include -D__ASSEMBLY__ -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/dts/.corstone1000-mps3.dtb.dts.tmp arch/arm/dts/.corstone1000-mps3.dtb.pre.tmp ; ./scripts/dtc/dtc -O dtb -o arch/arm/dts/corstone1000-mps3.dtb -b 0 -i /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/dts/ -Wno-unit_address_vs_reg -Wno-unit_address_format -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-graph_port -Wno-unique_unit_address -Wno-simple_bus_reg -Wno-pci_device_reg -Wno-pci_bridge -Wno-pci_device_bus_num  -a 0x8 -Wno-unit_address_vs_reg -Wno-unit_address_format -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-graph_port -Wno-unique_unit_address -Wno-simple_bus_reg -Wno-pci_device_reg -Wno-pci_bridge -Wno-pci_device_bus_num  -d arch/arm/dts/.corstone1000-mps3.dtb.d.dtc.tmp arch/arm/dts/.corstone1000-mps3.dtb.dts.tmp || (echo "Check /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/build/corstone1000_defconfig/arch/arm/dts/.corstone1000-mps3.dtb.pre.tmp for errors" && false) ; sed "s:arch/arm/dts/.corstone1000-mps3.dtb.pre.tmp:/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/dts/corstone1000-mps3.dts:" arch/arm/dts/.corstone1000-mps3.dtb.d.pre.tmp arch/arm/dts/.corstone1000-mps3.dtb.d.dtc.tmp > arch/arm/dts/.corstone1000-mps3.dtb.d

source_arch/arm/dts/corstone1000-mps3.dtb := /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/dts/corstone1000-mps3.dts

deps_arch/arm/dts/corstone1000-mps3.dtb := \
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
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/dts/corstone1000.dtsi \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/dt-bindings/interrupt-controller/arm-gic.h \
  /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/include/dt-bindings/interrupt-controller/irq.h \

arch/arm/dts/corstone1000-mps3.dtb: $(deps_arch/arm/dts/corstone1000-mps3.dtb)

$(deps_arch/arm/dts/corstone1000-mps3.dtb):
