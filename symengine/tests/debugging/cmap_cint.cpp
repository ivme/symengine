#include "catch.hpp"
#include <symengine/polys/uintpoly.h>
#include <map>

using SymEngine::integer_class;
using SymEngine::UIntDict;

TEST_CASE("cmap = cint", "[debug]")
{
    std::map<unsigned int, integer_class> map1{{0,integer_class(0)},{1,integer_class(1)}};

    //a UIntDict constructed from a std::map
    UIntDict cmap{map1};
    //a UintDict constructed from an int
    UIntDict cint{0};

    cmap = cint;
}