#include <stdlib.h>
#include <string.h>
#include "triangle.h"

int triangle_tritype(int a, int b, int c)
{
    // TODO implement tritype
    if (! (a > 0 && b > 0 && c > 0))
        return TRIANGLE_ERR;
    if (a == b && a == c)
        return TRIANGLE_EQUI;
    if (a == b || b == c)
        return TRIANGLE_ISO;
    if (a != b && a != c)
        return TRIANGLE_SCAL;
    if ((a + b) < c || (a + c) < b || (b + c) < a)
        return TRIANGLE_ERR;
    return TRIANGLE_ERR;
}
