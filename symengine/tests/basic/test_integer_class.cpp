//#include "catch.hpp"

#include <symengine/mp_class.h>
#include <symengine/basic.h>
#include <symengine/integer.h>
#include <symengine/rational.h>
#include <symengine/mul.h>
#include <symengine/pow.h>
#include <symengine/complex.h>
#include <symengine/complex_double.h>
#include <symengine/constants.h>
#include <symengine/add.h>
#include <symengine/functions.h>
#include <iostream>

using std::cout;
using std::endl;

using SymEngine::integer_class;
using SymEngine::Integer;
using SymEngine::integer;
using SymEngine::Rational;
using SymEngine::rational;
using SymEngine::make_rcp;
using SymEngine::RCP;
using SymEngine::pi;
using SymEngine::sub;
using SymEngine::Basic;
using SymEngine::Symbol;
using SymEngine::symbol;
using SymEngine::mul;


using namespace SymEngine::literals;

//break point
void print(std::ostream &os, const std::vector<RCP<const Basic>> &vars) {
    os << "calling print..." << std::endl;
    for (auto var : vars) {os << *var << std::endl;}
}

int main() {
    RCP<const Symbol> x = symbol("x");
    /*
    RCP<const Symbol> y = symbol("y");
    RCP<const Symbol> z = symbol("z");

    RCP<const Basic> im1 = integer(-1);
    RCP<const Basic> i2 = integer(2);
    RCP<const Basic> i3 = integer(3);
    RCP<const Basic> i6 = integer(6);
    RCP<const Basic> i12 = integer(12);
    */
    //RCP<const Basic> r1;
    /*
    RCP<const Basic> r2;
    RCP<const Basic> r3;

    r1 = mul(i3,pi);
    r2 = sub(r1, y);
    print(std::cout,{r1,r2});
    */
    sin(sub(mul(integer(3),pi),x));

    // sin(3*pi - y) = sin(y)
    // r1 = sin(sub(mul(i3, pi), y));
}
