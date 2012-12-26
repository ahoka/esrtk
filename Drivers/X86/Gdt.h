#ifndef __GDT_H__
#define __GDT_H__

#include <stdint.h>

struct gdtDescriptor
{
   uint16_t segmentLimit;
   uint16_t segmentBaseLow;
   uint8_t segmentBaseMiddle;
   uint8_t attributesAccess;
   uint8_t attributesGranuality;
   uint8_t segmentBaseHigh;
} __attribute__((packed));

#endif
