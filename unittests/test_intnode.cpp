#define CATCH_DEFINE_INTNODE
#include "catch.hpp"
#include "../intnode.h"

TEST_CASE("Leak")
{
    int *node;
    intnode_create(& node, 10);

    REQUIRE(intnode_print(node, stdout) == 2);
}