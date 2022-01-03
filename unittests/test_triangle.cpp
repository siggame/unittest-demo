#define CATCH_DEFINE_TRIANGLE
#include <dlfcn.h>
#include "catch.hpp"
#include "mutation.h"
#include "../triangle.h"
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

TEST_CASE("triangle tritype")
{
    MUTATION_START(triangle_tritype, "triangle.so")
    {
        typedef int (*triangle_tritype_func)(int, int, int);
        triangle_tritype_func func = (triangle_tritype_func) test_function();
        REQUIRE(func(0, 0, 0) == TRIANGLE_ERR);
        REQUIRE(func(5, 5, 11) == TRIANGLE_ERR);
        REQUIRE(func(0, 1, 1) == TRIANGLE_ERR);
        REQUIRE(func(1, 0, 1) == TRIANGLE_ERR);
        REQUIRE(func(1, 1, 0) == TRIANGLE_ERR);
        REQUIRE(func(2, 2, 2) == TRIANGLE_EQUI);

        REQUIRE(func(2, 2, 3) == TRIANGLE_ISO);
        REQUIRE(func(2, 3, 3) == TRIANGLE_ISO);
        REQUIRE(func(3, 2, 3) == TRIANGLE_ISO);
 
        REQUIRE(func(2, 3, 1) == TRIANGLE_SCAL);
        REQUIRE(func(3, 2, 1) == TRIANGLE_SCAL);
        REQUIRE(func(3, 1, 2) == TRIANGLE_SCAL); 
        REQUIRE(func(1, 3, 2) == TRIANGLE_SCAL);
 
    }  
    MUTATION_END 
} 


TEST_CASE("triangle area")
{
    MUTATION_START(triangle_area, "triangle.so")
    {
        typedef double (*AreaFunc)(double, double, double);
        AreaFunc func = (AreaFunc) test_function();
        REQUIRE(func(0, 0, 0) == 0);
        REQUIRE(0.432 < func(1, 1, 1));
        REQUIRE(func(1, 1, 1) < 0.434); 
    }
    MUTATION_END
}
