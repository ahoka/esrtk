#!/usr/bin/env bash
#
##
# Generate the interrupt vector table
##

rm -f InterruptVectors.is
rm -f InterruptVectorsInit.icc

#
# 10, 11, 12, 13, 14, 17 push error codes
#

for idt in {0..255}
do
    echo extern \"C\" void x86_isr_vector_$idt\(\)\; >> InterruptVectorsInit.icc
done

(
    echo
    echo void
    echo initInterruptVectors\(\)
    echo {
) >> InterruptVectorsInit.icc

for idt in {0..255}
do
    (
	echo -e .text
	echo -e .balign 16
        echo -e .global x86_isr_vector_$idt
        echo -e x86_isr_vector_$idt:
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
        echo -e \\t jmp x86_isr_vector_common
        echo
    ) >> InterruptVectors.is

    echo -e \\t x86_isr_init\($idt, \&x86_isr_vector_$idt\)\; >> InterruptVectorsInit.icc
done

echo -e } \\n >> InterruptVectorsInit.icc
