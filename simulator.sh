#!/bin/sh

MEMORY=256M

QEMU=qemu-system-i386
# if [ -x /usr/bin/kvm ]
# then
#     QEMU=kvm
# fi

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

$QEMU -m ${MEMORY} -s -S -M q35 -watchdog i6300esb -boot order=c -serial mon:stdio -d cpu_reset -no-reboot -nographic -cdrom esrtk.iso 2>&1 | tee run.log
