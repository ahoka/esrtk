#!/bin/sh

MEMORY=256M

QEMU=qemu-system-i386
if [ -x /usr/bin/kvm ]
then
    echo "Using KVM"
    QEMU=kvm
fi

$QEMU -m ${MEMORY} -M q35 -watchdog i6300esb -device rtl8139 -boot order=c -serial stdio -d cpu_reset -no-reboot -cdrom esrtk.iso 2>&1 | tee run.log
