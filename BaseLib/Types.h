
#ifndef _TYPES_H_
#define _TYPES_H_

#include <cstdint>
#include <cstddef>
#include "String.h"


typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned char ubyte;

// Signed integer types.
using int_t = std::intmax_t;
using index_t = std::ptrdiff_t;


#endif // _TYPES_H_
