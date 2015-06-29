#!/bin/sh

qemu-system-i386 -m 64M -M q35 -watchdog i6300esb -device rtl8139 -boot order=c -serial stdio -d cpu_reset -no-reboot -kernel ./obj.i686/kernel.elf -initrd Runtime/obj.i686/runtime.elf 2>&1 | tee run.log
