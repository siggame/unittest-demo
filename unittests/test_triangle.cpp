#define CATCH_DEFINE_TRIANGLE
#include "catch.hpp"
#include "../triangle.h"


TEST_CASE("Triangle Error")
{
    // Enumerate all conditions in which not a triangle
    REQUIRE(triangle_tritype(0, 0, 0) == TRIANGLE_ERR);
    REQUIRE(triangle_tritype(0, 0, 1) == TRIANGLE_ERR);
    REQUIRE(triangle_tritype(0, 1, 0) == TRIANGLE_ERR);
    REQUIRE(triangle_tritype(1, 0, -1) == TRIANGLE_ERR);
}


TEST_CASE("Triangle Iso")
{
    // TODO

    // Enumerate all conditions in which ISO;
    REQUIRE(triangle_tritype(1, 2, 2) == TRIANGLE_ISO);

    REQUIRE(triangle_tritype(2, 2, 1) == TRIANGLE_ISO);

}


TEST_CASE("Triangle Equi")
{

    REQUIRE(triangle_tritype(2, 2, 2) == TRIANGLE_EQUI);
    
    // Equilateral triangle is also a subset of ISO. How do we make this work?
    REQUIRE(triangle_tritype(2, 2, 2) == TRIANGLE_ISO);

}

TEST_CASE("Triangle Scal")
{
    REQUIRE(triangle_tritype(1, 2, 3) == TRIANGLE_SCAL);
}

