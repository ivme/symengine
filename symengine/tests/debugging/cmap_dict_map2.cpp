#include "catch.hpp"
#include <symengine/polys/uintpoly.h>
#include <map>

using SymEngine::integer_class;
using SymEngine::UIntDict;

TEST_CASE("cdict = cdefault", "[debug]")
{
    std::map<unsigned int, integer_class> map1{{0,integer_class(0)},{1,integer_class(1)}}; //x + 0
    std::map<unsigned int, integer_class> map2{{0,integer_class(0)},{1,integer_class(2)}}; //2x + 0

    //a UIntDict constructed from a std::map
    UIntDict cmap{map1};

    cmap.dict_ = map2;
}