#!/bin/sh
#
##
# Generate the interrupt vector table
##

rm -f InterruptVectors.is
rm -f InterruptVectorsInit.icc

#
# 10, 11, 12, 13, 14, 17 push error codes
#

for idt in $(seq 0 255)
do
    echo extern \"C\" void isr$idt\(\)\; >> InterruptVectorsInit.icc
done

(
    echo
    echo void
    echo initInterruptVectors\(\)
    echo {
) >> InterruptVectorsInit.icc

for idt in $(seq 0 255)
do
    (
        echo -e .global isr$idt
        echo -e isr$idt:
        echo -e \\t cli
        if [ ! $idt -eq 10 -a \
            ! $idt -eq 11 -a \
            ! $idt -eq 12 -a \
            ! $idt -eq 13 -a \
            ! $idt -eq 14 -a \
            ! $idt -eq 17 ]
        then
            echo -e \\t pushl \$0            
        fi
        echo -e \\t pushl \$$idt
        echo -e \\t jmp call_isr
        echo
    ) >> InterruptVectors.is

    echo -e \\t initIsr\($idt, \&isr$idt\)\; >> InterruptVectorsInit.icc
done

echo -e } \\n >> InterruptVectorsInit.icc
