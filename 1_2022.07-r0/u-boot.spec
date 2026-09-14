Summary: Universal Boot Loader for embedded devices
Name: u-boot
Version: 2022.07
Release: r0
Epoch: 1
License: GPL-2.0-or-later
Group: bootloaders
Packager: Poky <poky@lists.yoctoproject.org>
URL: http://www.denx.de/wiki/U-Boot/WebHome
BuildRequires: bc-native
BuildRequires: bison-native
BuildRequires: dtc-native
BuildRequires: flex-native
BuildRequires: gnutls-native
BuildRequires: kern-tools-native
BuildRequires: openssl-native
BuildRequires: pkgconfig-native
BuildRequires: python3-native
BuildRequires: python3-setuptools-native
BuildRequires: swig-native
BuildRequires: virtual/aarch64-poky-linux-musl-compilerlibs
BuildRequires: virtual/aarch64-poky-linux-musl-gcc
BuildRequires: virtual/libc
Requires: u-boot-env

%description
U-Boot, a boot loader for Embedded boards based on PowerPC, ARM, MIPS and
several other processors, which can be installed in a boot ROM and used to
initialize and test the hardware or to download and run application code.

%package -n u-boot-src
Summary: Universal Boot Loader for embedded devices - Source files
License: GPL-2.0-or-later
Group: devel

%description -n u-boot-src
U-Boot, a boot loader for Embedded boards based on PowerPC, ARM, MIPS and
several other processors, which can be installed in a boot ROM and used to
initialize and test the hardware or to download and run application code.
This package contains sources for debugging purposes.

%package -n u-boot-dbg
Summary: Universal Boot Loader for embedded devices - Debugging files
License: GPL-2.0-or-later
Group: devel

%description -n u-boot-dbg
U-Boot, a boot loader for Embedded boards based on PowerPC, ARM, MIPS and
several other processors, which can be installed in a boot ROM and used to
initialize and test the hardware or to download and run application code.
This package contains ELF symbols and related sources for debugging
purposes.

%package -n u-boot-staticdev
Summary: Universal Boot Loader for embedded devices - Development files (Static Libraries)
License: GPL-2.0-or-later
Group: devel
Requires: u-boot-dev = 1:2022.07-r0

%description -n u-boot-staticdev
U-Boot, a boot loader for Embedded boards based on PowerPC, ARM, MIPS and
several other processors, which can be installed in a boot ROM and used to
initialize and test the hardware or to download and run application code.
This package contains static libraries for software development.

%package -n u-boot-dev
Summary: Universal Boot Loader for embedded devices - Development files
License: GPL-2.0-or-later
Group: devel
Recommends: u-boot = 1:2022.07-r0
Recommends: u-boot-env-dev

%description -n u-boot-dev
U-Boot, a boot loader for Embedded boards based on PowerPC, ARM, MIPS and
several other processors, which can be installed in a boot ROM and used to
initialize and test the hardware or to download and run application code.
This package contains symbolic links, header files, and related items
necessary for software development.

%package -n u-boot-doc
Summary: Universal Boot Loader for embedded devices - Documentation files
License: GPL-2.0-or-later
Group: doc

%description -n u-boot-doc
U-Boot, a boot loader for Embedded boards based on PowerPC, ARM, MIPS and
several other processors, which can be installed in a boot ROM and used to
initialize and test the hardware or to download and run application code.
This package contains documentation.

%package -n u-boot-locale
Summary: Universal Boot Loader for embedded devices
License: GPL-2.0-or-later
Group: bootloaders

%description -n u-boot-locale
U-Boot, a boot loader for Embedded boards based on PowerPC, ARM, MIPS and
several other processors, which can be installed in a boot ROM and used to
initialize and test the hardware or to download and run application code.

%package -n u-boot-env
Summary: Universal Boot Loader for embedded devices
License: GPL-2.0-or-later
Group: bootloaders
Provides: u-boot-default-env

%description -n u-boot-env
U-Boot, a boot loader for Embedded boards based on PowerPC, ARM, MIPS and
several other processors, which can be installed in a boot ROM and used to
initialize and test the hardware or to download and run application code.

%package -n u-boot-extlinux
Summary: Universal Boot Loader for embedded devices
License: GPL-2.0-or-later
Group: bootloaders

%description -n u-boot-extlinux
U-Boot, a boot loader for Embedded boards based on PowerPC, ARM, MIPS and
several other processors, which can be installed in a boot ROM and used to
initialize and test the hardware or to download and run application code.

%files
%defattr(-,-,-,-)
%dir "/boot"
"/boot/u-boot-EFI-2022.07-r0.bin"
"/boot/u-boot.bin-EFI"
"/boot/u-boot.bin"

%files -n u-boot-dbg
%defattr(-,-,-,-)

%files -n u-boot-dev
%defattr(-,-,-,-)

%files -n u-boot-env
%defattr(-,-,-,-)
%dir "/etc"
"/etc/u-boot-initial-env"
"/etc/u-boot-initial-env-corstone1000-fvp-EFI"
"/etc/u-boot-initial-env-EFI"
"/etc/u-boot-initial-env-corstone1000-fvp-EFI-2022.07-r0"

