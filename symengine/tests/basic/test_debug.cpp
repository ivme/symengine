#include "catch.hpp"

#include <symengine/basic.h>
#include <symengine/parser.h>
#include <symengine/polys/basic_conversions.h>
#include <iostream>

using std::cout;
using std::endl;

using SymEngine::UIntPoly;
using SymEngine::Basic;
using SymEngine::parse;
using SymEngine::from_basic;
using SymEngine::symbol;
using SymEngine::integer_class;
using SymEngine::UIntDict;
using SymEngine::pow;


UIntDict debug_pow(const UIntDict &a, unsigned int p)
{
    //DEBUG
    cout << "body of debug_pow(const UIntDict &a, unsigned int p) in upolybase.h" << endl;
    UIntDict tmp = a, res(1);

    while (p != 1) {
        //DEBUG
        cout << "p == " << p << endl; 
        if (p % 2 == 0) {
            //DEBUG
            cout << "p mod 2 == 0" << endl;
            UIntDict tmp2 = a;
            tmp2 = tmp * tmp;
            cout << "after tmp2 = tmp * tmp" << endl;
            tmp = tmp2;
            cout << "after tmp = tmp2" << endl;
        } else {
            //DEBUG
            cout << "p mod 2 != 0" << endl;
            res = res * tmp;
            tmp = tmp * tmp;
        }
        cout << "dividing p by 2 via shift" << endl;
        p >>= 1;
    }
    //DEBUG
    cout << "returning from pow(const UIntDict &a, unsigned int p)" << endl;
    return (res * tmp);
}

TEST_CASE("Test Assignment", "[debug]")
{
    //assignment of one dict to another works here
    std::map<unsigned int, integer_class> dict1, dict2{{0,integer_class(0)},{1,integer_class(1)}};
    dict1 = dict2;

    //UIntDict assignment
    //a default-constructed UIntDict
    UIntDict cdefault;
    //a UIntDict constructed from a std::map
    UIntDict cmap{dict1};
    //a UIntDict constructed from a UIntDict, cmap
    UIntDict cdict{cmap};
    //a UintDict constructed from an int
    UIntDict cint{0};
    //another UIntDict constructed from cmap
    UIntDict cdict2{cmap};
    //a UIntDict constructed from a different UIntDict, cint
    UIntDict cdict3{cint};
    //a UIntDict constructed froma default-constructed UIntDict
    UIntDict cdict4{cdefault};
    
    // UNCOMMENT EACH OF THE FOLLOWING LINES INDIVIDUALLY TO CHECK
                                //first     //normal Release,
                                            //no SymEngineAssert
    // cdict = cdict;           //pass      //pass  
    // cdict = cdict2;          //fail      //pass  
    // cdict = cint;            //fail      //pass  
    // cdict = cmap;            //fail      //pass  
    // cdict = UIntDict(dict1); //fail      //pass  
    // cdict4 = cdefault;       //pass      //pass  
    // cmap = cmap;             //?         //pass  
    // cmap = cdict;            //pass      //fail 
    // cmap = cint;             //pass      //fail 
    // cmap = UIntDict(dict1);  //pass      //fail 
    // cint = cdict;            //pass      //pass  
    // cint = cint;             //pass      //pass
    // cint = cdict;            //pass      //pass
    // cint = cmap;             //pass      //pass

    //looks like assigning to a UIntDict that was constructed from another UIntDict fail 
    //unless it is a self-assignment, except cdict4 = cdefault

    //try directly assigning a map
    //cdict.dict_ = dict2; //fail ... wait, now pass  ? what?
    //cmap.dict_ = dict2; //pass  
    //cout << cdict.dict_[0] << endl;

    //cout << "&cmap.dict_ == " << &(cmap.dict_) << endl;
    //cout << "&cdict.dict_ == " << &(cdict.dict_) << endl;
}

TEST_CASE("cdict = cdict", "[debug]")
{
    //assignment of one dict to another works here
    std::map<unsigned int, integer_class> map1{{0,integer_class(0)},{1,integer_class(1)}};

    //a UIntDict constructed from a std::map
    UIntDict cmap{map1};
    //a UIntDict constructed from a UIntDict, cmap
    UIntDict cdict{cmap};

    cdict = cdict;
}

TEST_CASE("cdict = cdict2", "[debug]")
{
    //assignment of one dict to another works here
    std::map<unsigned int, integer_class> map1{{0,integer_class(0)},{1,integer_class(1)}};

    //a UIntDict constructed from a std::map
    UIntDict cmap{map1};
    //a UIntDict constructed from a UIntDict, cmap
    UIntDict cdict{cmap};
    //another UIntDict constructed from cmap
    UIntDict cdict2{cmap};

    cdict = cdict2;
}

TEST_CASE("cdict = cint", "[debug]")
{
    //assignment of one dict to another works here
    std::map<unsigned int, integer_class> map1{{0,integer_class(0)},{1,integer_class(1)}};

    //a UIntDict constructed from a std::map
    UIntDict cmap{map1};
    //a UIntDict constructed from a UIntDict, cmap
    UIntDict cdict{cmap};
    //a UintDict constructed from an int
    UIntDict cint{0};

    cdict = cint;
}

TEST_CASE("cdict = cmap", "[debug]")
{
    //assignment of one dict to another works here
    std::map<unsigned int, integer_class> map1{{0,integer_class(0)},{1,integer_class(1)}};

    //a UIntDict constructed from a std::map
    UIntDict cmap{map1};
    //a UIntDict constructed from a UIntDict, cmap
    UIntDict cdict{cmap};

    cdict = cmap;
}

TEST_CASE("cdict = UIntDict(map1)", "[debug]")
{
    //assignment of one dict to another works here
    std::map<unsigned int, integer_class> map1{{0,integer_class(0)},{1,integer_class(1)}};

    //a UIntDict constructed from a std::map
    UIntDict cmap{map1};
    //a UIntDict constructed from a UIntDict, cmap
    UIntDict cdict{cmap};

    cdict = UIntDict(map1);
}

TEST_CASE("cdict = cdefault", "[debug]")
{
    //assignment of one dict to another works here
    std::map<unsigned int, integer_class> map1{{0,integer_class(0)},{1,integer_class(1)}};

    //a default-constructed UIntDict
    UIntDict cdefault;
    //a UIntDict constructed from a std::map
    UIntDict cmap{map1};
    //a UIntDict constructed from a UIntDict, cmap
    UIntDict cdict{cmap};

    cdict = cdefault;
}

TEST_CASE("cmap = cmap", "[debug]")
{
    //assignment of one dict to another works here
    std::map<unsigned int, integer_class> map1{{0,integer_class(0)},{1,integer_class(1)}};
    //a UIntDict constructed from a std::map
    UIntDict cmap{map1};

    cmap = cmap;
}

TEST_CASE("cmap = cdict", "[debug]")
{
    //assignment of one dict to another works here
    std::map<unsigned int, integer_class> map1{{0,integer_class(0)},{1,integer_class(1)}};

    //a UIntDict constructed from a std::map
    UIntDict cmap{map1};
    //a UIntDict constructed from a UIntDict, cmap
    UIntDict cdict{cmap};

    cmap = cdict;
}

TEST_CASE("cmap = cint", "[debug]")
{
    //assignment of one dict to another works here
    std::map<unsigned int, integer_class> map1{{0,integer_class(0)},{1,integer_class(1)}};

    //a UIntDict constructed from a std::map
    UIntDict cmap{map1};
    //a UintDict constructed from an int
    UIntDict cint{0};

    cmap = cint;
}

TEST_CASE("Test pow","[debug]")
{
    //std::string s;
    //RCP<const UIntPoly> poly1;
    //RCP<const Basic> x = symbol("x");

    //debug_pow(uid2,2);

    //UIntDict pow - fail !
    //UIntDict::pow(uid2,2);

    //s = "x + 2*x**2 + 1";
    //assigning one dict to another deep in this call to from_basic fail .
    //poly1 = from_basic<UIntPoly>(parse(s));//error in Release mode w/boostmp
}