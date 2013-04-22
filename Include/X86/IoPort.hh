#ifndef _IOPORT_H_
#define _IOPORT_H_

#include <stdint.h>

extern "C" uint8_t inb(unsigned int port);
extern "C" uint16_t inw(unsigned int port);
extern "C" uint32_t inl(unsigned int port);

extern "C" void outb(unsigned int port, uint8_t value);
extern "C" void outw(unsigned int port, uint16_t value);
extern "C" void outl(unsigned int port, uint32_t value);

#endif
