#!/bin/bash

FVP=~/FVP_Corstone_1000/models/Linux64_armv8l_GCC-9.3/FVP_Corstone-1000
IMG=~/FVP_Corstone_1000/images/corstone1000-fvp

$FVP \
  -C board.flashloader0.fname=none \
  -C se.trustedBootROMloader.fname=$IMG/bl1.bin \
  -C board.xnvm_size=64 \
  -C se.trustedSRAM_config=6 \
  -C se.BootROM_config=3 \
  -C board.smsc_91c111.enabled=1 \
  -C board.hostbridge.userNetworking=true \
  -C board.hostbridge.userNetPorts=8022=22,8080=80 \
  -C board.se_flash_size=8192 \
  -C extsys_harness0.extsys_flashloader.fname=$IMG/es_flashfw.bin \
  -C diagnostics=4 \
  -C disable_visualisation=true \
  -C host.core1_ppu.default_power_state_on=1 \
  -C host.core2_ppu.default_power_state_on=1 \
  -C host.core3_ppu.default_power_state_on=1 \
  \
  \
--plugin "$HOME/FVP_Corstone_1000/plugins/Linux64_armv8l_GCC-9.3/TarmacTrace.so" \
-C TRACE.TarmacTrace.trace-file=trace.txt \
-C TRACE.TarmacTrace.trace_instructions=0 \
-C TRACE.TarmacTrace.trace_events=1 \
-C TRACE.TarmacTrace.trace_branches=0 \
-C TRACE.TarmacTrace.trace_core_registers=0 \
-C TRACE.TarmacTrace.trace_cp15=0 \
-C TRACE.TarmacTrace.trace_vfp=0 \
-C TRACE.TarmacTrace.trace_loads_stores=0 \
-C TRACE.TarmacTrace.trace_mmu=0 \
-C TRACE.TarmacTrace.trace_cache=0 \
-C TRACE.TarmacTrace.trace_gicv3=0 \
-C TRACE.TarmacTrace.trace_memory=0 \
-C TRACE.TarmacTrace.trace_bus_accesses=0 \
-C TRACE.TarmacTrace.trace_atomic=0 \
-C TRACE.TarmacTrace.trace_bte=0 \
-C TRACE.TarmacTrace.trace_gpt=0 \
-C TRACE.TarmacTrace.trace_ete=0 \
-C TRACE.TarmacTrace.trace_spe=0 \
-C TRACE.TarmacTrace.trace_tag_loads_stores=0 \
  \
  --data board.flash0=$IMG/corstone1000-image-corstone1000-fvp.wic.nopt@0x68100000 \
  -C se.uart0.out_file=se_uart.log \
  -C host.uart0.out_file=linux_uart.log \
  -C board.msd_mmc_2.p_mmc_file=$(readlink -f ./MMC/mmc.dat) \
  -C board.msd_mmc.p_mmc_file=$(readlink -f ./MMC/mmc.dat)

