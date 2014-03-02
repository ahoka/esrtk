#ifndef ASSEMBLY_HH
#define ASSEMBLY_HH

#define FUNCTION(x) .text; .balign 16; .global x; .type x, @function; x:
#define END(x) .size x, . - x

#endif
