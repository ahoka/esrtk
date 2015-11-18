#!/bin/sh

MEMORY=256M

QEMU=qemu-system-i386
if [ -x /usr/bin/kvm ]
then
    QEMU=kvm
fi

if [ $# -eq 1 ]
then
    case $1 in
        kvm)
            echo "Using KVM"
            QEMU=kvm
            ;;
        qemu)
            echo "Using qemu"
            QEMU=qemu-system-i386
            ;;
        *)
            echo "Invalid emulator"
            exit 1
            ;;
    esac
fi

$QEMU -m ${MEMORY} -M q35 -watchdog i6300esb -device rtl8139 -boot order=c -serial stdio -d cpu_reset -no-reboot -cdrom esrtk.iso 2>&1 | tee run.log
