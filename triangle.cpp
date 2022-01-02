#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "triangle.h"


static int is_valid_sides(int a, int b, int c)
{
    return a > 0 && b > 0 && c > 0;
}

static int is_equilateral(int a, int b, int c)
{
    return a == b && b == c;
}

static int is_isoceles(int a, int b, int c)
{
    return (a == b && b != c) || (a == c && a != b) || (b == c && b != a);
}

static int is_scalene(int a, int b, int c)
{
    return a != b && a != c && b != c;
}

static int is_valid_triangle(int a, int b, int c)
{
    return is_valid_sides(a, b, c) && a + b > c;
}

int triangle_tritype(int a, int b, int c)
{
    if (! is_valid_triangle(a, b, c)) {
        return TRIANGLE_ERR;
    }
    if (is_equilateral(a, b, c)) {
        return TRIANGLE_EQUI;
    }
    if (is_isoceles(a, b, c)) {
        return TRIANGLE_ISO;
    }
    if (is_scalene(a, b, c)) {
        return TRIANGLE_SCAL;
    }
    return TRIANGLE_ERR;
}

double triangle_area(double a, double b, double c)
{
    double s = ((double) (a + b + c)) / 2;
    return sqrt(s * (s - a) * (s - b) * (s - c));
    
}