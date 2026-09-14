	.arch armv8-a+crc
	.file	"asm-offsets.c"
// GNU C11 (GCC) version 12.2.0 (aarch64-poky-linux-musl)
//	compiled by GNU C version 11.4.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version none
// GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
// options passed: -mstrict-align -mgeneral-regs-only -march=armv8-a+crc -mlittle-endian -mabi=lp64 -g -Os -std=gnu11 -fno-builtin -ffreestanding -fshort-wchar -fno-strict-aliasing -fno-stack-protector -fno-delete-null-pointer-checks -fstack-usage -fno-pic -ffunction-sections -fdata-sections -ffixed-r9 -fno-common -ffixed-x18
	.text
.Ltext0:
	.file 0 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/build/corstone1000_defconfig" "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c"
	.section	.text.startup.main,"ax",@progbits
	.align	2
	.global	main
	.type	main, %function
main:
.LFB99:
	.file 1 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c"
	.loc 1 26 1 view -0
	.cfi_startproc
	hint	34 // bti c
	.loc 1 118 2 view .LVU1
#APP
// 118 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c" 1
	
.ascii "->ARM_SMCCC_RES_X0_OFFS 0 offsetof(struct arm_smccc_res, a0)"	//
// 0 "" 2
	.loc 1 119 2 view .LVU2
// 119 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c" 1
	
.ascii "->ARM_SMCCC_RES_X2_OFFS 16 offsetof(struct arm_smccc_res, a2)"	//
// 0 "" 2
	.loc 1 120 2 view .LVU3
// 120 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c" 1
	
.ascii "->ARM_SMCCC_QUIRK_ID_OFFS 0 offsetof(struct arm_smccc_quirk, id)"	//
// 0 "" 2
	.loc 1 121 2 view .LVU4
// 121 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c" 1
	
.ascii "->ARM_SMCCC_QUIRK_STATE_OFFS 8 offsetof(struct arm_smccc_quirk, state)"	//
// 0 "" 2
	.loc 1 123 3 view .LVU5
// 123 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c" 1
	
.ascii "->ARM_SMCCC_1_2_REGS_X0_OFFS 0 offsetof(struct arm_smccc_1_2_regs, a0)"	//
// 0 "" 2
	.loc 1 124 3 view .LVU6
// 124 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c" 1
	
.ascii "->ARM_SMCCC_1_2_REGS_X2_OFFS 16 offsetof(struct arm_smccc_1_2_regs, a2)"	//
// 0 "" 2
	.loc 1 125 3 view .LVU7
// 125 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c" 1
	
.ascii "->ARM_SMCCC_1_2_REGS_X4_OFFS 32 offsetof(struct arm_smccc_1_2_regs, a4)"	//
// 0 "" 2
	.loc 1 126 3 view .LVU8
// 126 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c" 1
	
.ascii "->ARM_SMCCC_1_2_REGS_X6_OFFS 48 offsetof(struct arm_smccc_1_2_regs, a6)"	//
// 0 "" 2
	.loc 1 127 3 view .LVU9
// 127 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c" 1
	
.ascii "->ARM_SMCCC_1_2_REGS_X8_OFFS 64 offsetof(struct arm_smccc_1_2_regs, a8)"	//
// 0 "" 2
	.loc 1 128 3 view .LVU10
// 128 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c" 1
	
.ascii "->ARM_SMCCC_1_2_REGS_X10_OFFS 80 offsetof(struct arm_smccc_1_2_regs, a10)"	//
// 0 "" 2
	.loc 1 129 3 view .LVU11
// 129 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c" 1
	
.ascii "->ARM_SMCCC_1_2_REGS_X12_OFFS 96 offsetof(struct arm_smccc_1_2_regs, a12)"	//
// 0 "" 2
	.loc 1 130 3 view .LVU12
// 130 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c" 1
	
.ascii "->ARM_SMCCC_1_2_REGS_X14_OFFS 112 offsetof(struct arm_smccc_1_2_regs, a14)"	//
// 0 "" 2
	.loc 1 131 3 view .LVU13
// 131 "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c" 1
	
.ascii "->ARM_SMCCC_1_2_REGS_X16_OFFS 128 offsetof(struct arm_smccc_1_2_regs, a16)"	//
// 0 "" 2
	.loc 1 135 2 view .LVU14
// /work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c:136: }
	.loc 1 136 1 is_stmt 0 view .LVU15
#NO_APP
	mov	w0, 0	//,
	ret	
	.cfi_endproc
.LFE99:
	.size	main, .-main
	.text
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0x99
	.2byte	0x5
	.byte	0x1
	.byte	0x8
	.4byte	.Ldebug_abbrev0
	.uleb128 0x2
	.4byte	.LASF13
	.byte	0x1d
	.4byte	.LASF0
	.4byte	.LASF1
	.4byte	.LLRL0
	.8byte	0
	.4byte	.Ldebug_line0
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x1
	.byte	0x8
	.byte	0x7
	.4byte	.LASF2
	.uleb128 0x1
	.byte	0x1
	.byte	0x8
	.4byte	.LASF3
	.uleb128 0x1
	.byte	0x4
	.byte	0x7
	.4byte	.LASF4
	.uleb128 0x1
	.byte	0x2
	.byte	0x7
	.4byte	.LASF5
	.uleb128 0x1
	.byte	0x8
	.byte	0x5
	.4byte	.LASF6
	.uleb128 0x1
	.byte	0x8
	.byte	0x5
	.4byte	.LASF7
	.uleb128 0x1
	.byte	0x1
	.byte	0x6
	.4byte	.LASF8
	.uleb128 0x1
	.byte	0x1
	.byte	0x8
	.4byte	.LASF9
	.uleb128 0x1
	.byte	0x2
	.byte	0x5
	.4byte	.LASF10
	.uleb128 0x1
	.byte	0x8
	.byte	0x7
	.4byte	.LASF11
	.uleb128 0x1
	.byte	0x10
	.byte	0x4
	.4byte	.LASF12
	.uleb128 0x4
	.4byte	.LASF14
	.byte	0x1
	.byte	0x19
	.byte	0x5
	.4byte	0x2a
	.8byte	.LFB99
	.8byte	.LFE99-.LFB99
	.uleb128 0x1
	.byte	0x9c
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x1f
	.uleb128 0x1b
	.uleb128 0x1f
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.4byte	0x2c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x8
	.byte	0
	.2byte	0
	.2byte	0
	.8byte	.LFB99
	.8byte	.LFE99-.LFB99
	.8byte	0
	.8byte	0
	.section	.debug_rnglists,"",@progbits
.Ldebug_ranges0:
	.4byte	.Ldebug_ranges3-.Ldebug_ranges2
.Ldebug_ranges2:
	.2byte	0x5
	.byte	0x8
	.byte	0
	.4byte	0
.LLRL0:
	.byte	0x7
	.8byte	.LFB99
	.uleb128 .LFE99-.LFB99
	.byte	0
.Ldebug_ranges3:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF4:
	.string	"unsigned int"
.LASF2:
	.string	"long unsigned int"
.LASF8:
	.string	"signed char"
.LASF12:
	.string	"long double"
.LASF10:
	.string	"short int"
.LASF13:
	.ascii	"GNU C11 12.2.0 -mstrict-align -mgeneral-regs-only -march=arm"
	.ascii	"v8-a+cr"
	.string	"c -mlittle-endian -mabi=lp64 -g -Os -std=gnu11 -fno-builtin -ffreestanding -fshort-wchar -fno-strict-aliasing -fno-stack-protector -fno-delete-null-pointer-checks -fstack-usage -fno-pic -ffunction-sections -fdata-sections -ffixed-r9 -fno-common -ffixed-x18"
.LASF11:
	.string	"long long unsigned int"
.LASF9:
	.string	"unsigned char"
.LASF6:
	.string	"long int"
.LASF14:
	.string	"main"
.LASF3:
	.string	"char"
.LASF5:
	.string	"short unsigned int"
.LASF7:
	.string	"long long int"
	.section	.debug_line_str,"MS",@progbits,1
.LASF1:
	.string	"/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/build/corstone1000_defconfig"
.LASF0:
	.string	"/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/git/arch/arm/lib/asm-offsets.c"
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align	3
	.word	4
	.word	16
	.word	5
	.string	"GNU"
	.word	3221225472
	.word	4
	.word	3
	.align	3
