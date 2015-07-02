#!/bin/sh

qemu-system-i386 -m 64M -M q35 -watchdog i6300esb -device rtl8139 -boot order=c -serial stdio -d cpu_reset -no-reboot -cdrom esrtk.iso 2>&1 | tr -d '\r' | tee run.log
