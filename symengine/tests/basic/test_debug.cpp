#include "catch.hpp"

#include <symengine/basic.h>
#include <symengine/parser.h>
#include <symengine/polys/basic_conversions.h>
#include <iostream>

using std::cout;
using std::endl;

using SymEngine::RCP;
using SymEngine::UIntPoly;
using SymEngine::Basic;
using SymEngine::parse;
using SymEngine::from_basic;
using SymEngine::symbol;


TEST_CASE("Test Debug", "[debug]")
{
    std::string s;
    RCP<const UIntPoly> poly1;
    RCP<const Basic> x = symbol("x");

    s = "x + 2*x**2 + 1";
    poly1 = from_basic<UIntPoly>(parse(s));//error in Release mode w/boostmp
}