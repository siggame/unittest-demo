/**
 * @file
 *  @brief Contains functions, macro definitions, and symbols relating to triangles
 *  @test unittests/test_triangle.h
 * 
*/

#ifndef TRIANGLE_H
#define TRIANGLE_H

#ifdef __cplusplus
extern "C" {
#endif



#define TRIANGLE_ERR     -1
#define TRIANGLE_ISO     0
#define TRIANGLE_EQUI    1
#define TRIANGLE_SCAL    2

/**
 * @brief Given sides of triangle a, b, c, return type of triangle
 * @return -1 if not trianglem 0 if iso, 1 if equi, 2 of scal
 */
int triangle_tritype(int a, int b, int c);
double triangle_area(double a, double b, double c);

#ifdef __cplusplus
}
#endif
#endif