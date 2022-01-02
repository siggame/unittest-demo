/**
 * This file defines GNU extension/keywords as empty strings. pycparser doesn't know how
 * to handle these
*/
#define __restrict
#define __attribute__(x)
#define __extension__
#define __builtin_va_list struct __builtin_va_list
#define __asm__(x)

typedef  long double _Float128; // <math.h> does not define this
