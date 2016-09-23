#include "catch.hpp"

#include <symengine/sets.h>
#include <symengine/logic.h>
#include <symengine/infinity.h>
#include <symengine/symengine_exception.h>

using SymEngine::Basic;
using SymEngine::Integer;
using SymEngine::integer;
using SymEngine::Rational;
using SymEngine::one;
using SymEngine::zero;
using SymEngine::Number;
using SymEngine::RCP;
using SymEngine::Interval;
using SymEngine::interval;
using SymEngine::FiniteSet;
using SymEngine::finiteset;
using SymEngine::Set;
using SymEngine::EmptySet;
using SymEngine::emptyset;
using SymEngine::UniversalSet;
using SymEngine::universalset;
using SymEngine::Union;
using SymEngine::set_union;
using SymEngine::rcp_dynamic_cast;
using SymEngine::Complex;
using SymEngine::symbol;
using SymEngine::is_a;
using SymEngine::symbol;
using SymEngine::boolean;
using SymEngine::Inf;
using SymEngine::NegInf;
using SymEngine::NotImplementedError;
using SymEngine::SymEngineException;
using SymEngine::boolTrue;
using SymEngine::boolFalse;
using SymEngine::Contains;
using SymEngine::make_rcp;
using SymEngine::set_set;

TEST_CASE("Interval : Basic", "[basic]")
{
    RCP<const Set> r1, r2, r3, r4;
    RCP<const Number> i2 = integer(2);
    RCP<const Number> i20 = integer(20);
    RCP<const Number> im5 = integer(-5);
    RCP<const Number> rat1 = Rational::from_two_ints(*integer(5), *integer(6));
    RCP<const Number> rat2
        = Rational::from_two_ints(*integer(500), *integer(6));

    r1 = interval(zero, i20); // [0, 20]
    r2 = interval(im5, i2);   // [-5, 2]

    REQUIRE(is_a<Interval>(*r1));
    REQUIRE(not is_a<EmptySet>(*r1));
    REQUIRE(not is_a<UniversalSet>(*r1));

    r3 = r1->set_intersection(r2); // [0, 2]
    REQUIRE(eq(*r1->contains(one), *boolTrue));
    r4 = interval(zero, i2); // [0, 2]
    REQUIRE(eq(*r3, *r4));
    r3 = interval(im5, i2, true, true); // (-5, 2)
    REQUIRE(eq(*r3->contains(i2), *boolFalse));
    REQUIRE(eq(*r3->contains(im5), *boolFalse));
    REQUIRE(eq(*r3->contains(rat2), *boolFalse));
    REQUIRE(eq(*r3->contains(integer(-7)), *boolFalse));
    r4 = r3->set_intersection(r2);
    REQUIRE(eq(*r3, *r4));
    r3 = r1->set_union(r2); // [-5, 20]
    REQUIRE(eq(*r3, *set_union({r1, r2})));
    r4 = interval(im5, i20);
    REQUIRE(eq(*r3, *r4));
    r3 = r2->set_union(r1); // [-5, 20]
    REQUIRE(eq(*r3, *set_union({r1, r2})));
    REQUIRE(eq(*r3, *r4));
    r3 = interval(integer(21), integer(22));
    r4 = r1->set_intersection(r3);
    REQUIRE(eq(*r4, *emptyset()));
    r3 = interval(im5, i2, false, false); // [-5, 2]
    r4 = interval(integer(3), i20, false, false);
    REQUIRE(r3->compare(*r4) == -1);

    REQUIRE(eq(*r3->set_union(r4), *set_union({r3, r4})));
    REQUIRE(eq(*r4->set_union(r3), *set_union({r3, r4})));

    r3 = interval(zero, i2, true, true); // (0, 2)
    REQUIRE(eq(*r3->contains(sqrt(i2)), *make_rcp<Contains>(sqrt(i2), r3)));

    r3 = interval(im5, i2, false, false); // [-5, 2]
    REQUIRE(r3->is_subset(r2));
    REQUIRE(not r3->is_subset(emptyset()));
    REQUIRE(not r3->is_proper_subset(emptyset()));
    REQUIRE(not r3->is_proper_subset(r2));
    REQUIRE(not r3->is_proper_superset(r2));
    r3 = interval(im5, i20);
    r4 = interval(zero, i2);
    REQUIRE(r3->is_superset(r4));
    REQUIRE(r3->is_proper_superset(r4));

    r1 = interval(rat1, rat2); // [5/6, 500/6]
    r2 = interval(im5, i2);    // [-5, 2]
    r3 = r1->set_intersection(r2);
    r4 = interval(rat1, i2);
    REQUIRE(eq(*r3, *r4));
    r3 = r2->set_intersection(r1);
    REQUIRE(eq(*r3, *r4));
    REQUIRE(eq(*emptyset(), *r1->set_intersection(emptyset())));
    REQUIRE(eq(*r1, *r1->set_union(emptyset())));
    REQUIRE(eq(*r1, *set_union({r1, emptyset()})));

    REQUIRE(r4->__str__() == "[5/6, 2]");
    REQUIRE(r4->compare(*r3) == 0);
    r4 = interval(rat1, i2, true, true);
    REQUIRE(r4->__str__() == "(5/6, 2)");

    r1 = interval(one, zero);
    REQUIRE(eq(*r1, *emptyset()));
    r1 = interval(one, one, true, true);
    REQUIRE(eq(*r1, *emptyset()));

    r1 = interval(zero, one);
    RCP<const Interval> r5 = rcp_dynamic_cast<const Interval>(r1);

    r2 = interval(zero, one, false, false);
    REQUIRE(eq(*r5->close(), *r1));
    r2 = interval(zero, one, true, false);
    REQUIRE(eq(*r5->Lopen(), *r2));
    r2 = interval(zero, one, false, true);
    REQUIRE(eq(*r5->Ropen(), *r2));
    r2 = interval(zero, one, true, true);
    REQUIRE(eq(*r5->open(), *r2));

    r1 = interval(zero, Inf, false, true);
    r2 = interval(NegInf, one, true, true);
    r3 = interval(zero, one, false, true);
    REQUIRE(eq(*r3, *r1->set_intersection(r2)));

    REQUIRE(not r5->__eq__(*r2));
    REQUIRE(r5->__hash__() != emptyset()->__hash__());
    REQUIRE(not r5->__eq__(*emptyset()));

    REQUIRE(r5->Lopen()->compare(*r5) == -1);
    REQUIRE(r5->compare(*r5->Lopen()) == 1);
    REQUIRE(r5->Ropen()->compare(*r5) == 1);
    REQUIRE(r5->compare(*r5->Ropen()) == -1);

    REQUIRE(eq(*r5->get_args()[0], *r5->start_));
    REQUIRE(eq(*r5->get_args()[1], *r5->end_));
    REQUIRE(eq(*r5->get_args()[2], *boolean(r5->left_open_)));
    REQUIRE(eq(*r5->get_args()[3], *boolean(r5->right_open_)));
    RCP<const Number> c1 = Complex::from_two_nums(*i2, *i20);
    CHECK_THROWS_AS(interval(c1, one), NotImplementedError);
    CHECK_THROWS_AS(r5->diff(symbol("x")), SymEngineException);
}

TEST_CASE("EmptySet : Basic", "[basic]")
{
    RCP<const Set> r1 = emptyset();
    RCP<const Set> r2 = interval(zero, one);

    REQUIRE(not is_a<Interval>(*r1));
    REQUIRE(is_a<EmptySet>(*r1));
    REQUIRE(not is_a<UniversalSet>(*r1));
    REQUIRE(r1->is_subset(r2));
    REQUIRE(r1->is_proper_subset(r2));
    REQUIRE(not r1->is_proper_superset(r2));
    REQUIRE(r1->is_superset(r1));
    REQUIRE(not r1->is_superset(r2));
    REQUIRE(eq(*r1, *r1->set_intersection(r2)));
    REQUIRE(eq(*r2, *r1->set_union(r2)));
    REQUIRE(eq(*r2, *set_union({r1, r2})));
    REQUIRE(r1->__str__() == "EmptySet");
    REQUIRE(r1->__hash__() == emptyset()->__hash__());
    REQUIRE(not r1->is_proper_subset(r1));
    REQUIRE(not r1->__eq__(*r2));
    REQUIRE(r1->compare(*emptyset()) == 0);
    REQUIRE(eq(*r1->contains(zero), *boolFalse));
    CHECK_THROWS_AS(r1->diff(symbol("x")), SymEngineException);
}

TEST_CASE("UniversalSet : Basic", "[basic]")
{
    RCP<const Set> r1 = universalset();
    RCP<const Set> r2 = interval(zero, one);
    RCP<const Set> e = emptyset();

    REQUIRE(not is_a<Interval>(*r1));
    REQUIRE(not is_a<EmptySet>(*r1));
    REQUIRE(is_a<UniversalSet>(*r1));
    REQUIRE(not r1->is_subset(r2));
    REQUIRE(not r1->is_subset(e));
    REQUIRE(not r1->is_proper_subset(r2));
    REQUIRE(not r1->is_proper_subset(e));
    REQUIRE(r1->is_proper_superset(r2));
    REQUIRE(r1->is_proper_superset(e));
    REQUIRE(r1->is_superset(r2));
    REQUIRE(r1->is_superset(e));
    REQUIRE(r1->is_subset(r1));
    REQUIRE(not r1->is_proper_subset(r1));
    REQUIRE(r1->is_superset(r1));
    REQUIRE(not r1->is_proper_superset(r1));
    REQUIRE(eq(*r1, *r1->set_union(r2)));
    REQUIRE(eq(*r1, *r1->set_union(e)));
    REQUIRE(eq(*r1, *set_union({r1, r2})));
    REQUIRE(eq(*r1, *set_union({r1, e})));
    REQUIRE(eq(*r2, *r1->set_intersection(r2)));
    REQUIRE(eq(*e, *r1->set_intersection(e)));
    REQUIRE(eq(*r1->contains(zero), *boolTrue));
    REQUIRE(r1->__str__() == "UniversalSet");
    REQUIRE(r1->__hash__() == universalset()->__hash__());
    REQUIRE(not r1->__eq__(*r2));
    REQUIRE(r1->compare(*universalset()) == 0);
    CHECK_THROWS_AS(r1->diff(symbol("x")), SymEngineException);
}

TEST_CASE("FiniteSet : Basic", "[basic]")
{
    RCP<const Set> r1 = finiteset({zero, one, symbol("x")});
    RCP<const Set> r2 = finiteset({zero, one, integer(2)});
    RCP<const Set> r3 = r1->set_union(r2); // {0, 1, 2, x}
    REQUIRE(eq(*r3, *set_union({r1, r2})));
    r3 = r1->set_intersection(r2); // {0, 1}
    REQUIRE(eq(*r3, *finiteset({zero, one})));
    REQUIRE(r3->__hash__() == finiteset({zero, one})->__hash__());
    REQUIRE(r3->compare(*r2) == -1);
    REQUIRE(eq(*r3->contains(one), *boolTrue));
    REQUIRE(eq(*r3->contains(zero), *boolTrue));
    REQUIRE(eq(*r3->contains(integer(3)), *boolFalse));
    REQUIRE(r3->is_subset(r2));
    REQUIRE(r3->is_proper_subset(r2));

    r1 = finiteset({zero, one});
    RCP<const Set> r4 = interval(zero, one);
    r3 = r2->set_intersection(r4);
    REQUIRE(eq(*r3->contains(one), *boolTrue));
    REQUIRE(eq(*r3->contains(zero), *boolTrue));
    r2 = finiteset({zero, one});
    r3 = r2->set_union(r4);
    REQUIRE(eq(*r3, *set_union({r2, r4})));
    REQUIRE(r3->__str__() == "[0, 1]");
    REQUIRE(r1->is_subset(r4));
    REQUIRE(r1->is_proper_subset(r4));
    r4 = interval(zero, zero);
    r1 = finiteset({zero});
    REQUIRE(r1->is_subset(r4));
    REQUIRE(not r1->is_proper_subset(r4));
    REQUIRE(r1->__eq__(*r4));
    REQUIRE(r4->__eq__(*r1));
    r1 = finiteset({zero, one});
    r4 = interval(zero, one, true, true); // (0, 1)
    r3 = r1->set_union(r4);
    r2 = interval(zero, one); // [0, 1]
    REQUIRE(eq(*r2, *r3));
    REQUIRE(eq(*r2, *set_union({r1, r4})));
    r1 = finiteset({zero, one, integer(2)});
    r3 = r1->set_union(r4);
    REQUIRE(eq(*r3, *set_union({r1, r4})));
    r4 = interval(zero, one, false, true); // [0, 1)
    r3 = r1->set_union(r4);
    REQUIRE(eq(*r3, *set_union({r1, r4})));

    r4 = emptyset();
    r3 = r2->set_intersection(r4);
    REQUIRE(eq(*r3, *emptyset()));
    r3 = r2->set_union(r4);
    REQUIRE(eq(*r3, *r2));
    REQUIRE(eq(*r3, *set_union({r2, r4})));
    REQUIRE(r1->is_superset(r4));
    REQUIRE(not r1->is_proper_subset(r4));

    r4 = universalset();
    r3 = r2->set_intersection(r4);
    REQUIRE(eq(*r3, *r2));
    r3 = r2->set_union(r4);
    REQUIRE(eq(*r3, *set_union({r2, r4})));
    REQUIRE(eq(*r3, *universalset()));
    REQUIRE(not r1->is_superset(r4));
    REQUIRE(r1->is_proper_subset(r4));
}

TEST_CASE("Union : Basic", "[basic]")
{
    auto check_union_str = [](std::string to_chk, set_set sets) {
        if ((size_t)std::count(to_chk.begin(), to_chk.end(), 'U')
            != sets.size() - 1)
            return false;
        for (auto &a : sets) {
            if (to_chk.find(a->__str__()) == std::string::npos)
                return false;
        }
        return true;
    };
    RCP<const Set> f1 = finiteset({zero, one, symbol("x")});
    RCP<const Set> r1 = set_union({f1, emptyset()});
    REQUIRE(eq(*r1, *f1));
    r1 = set_union({emptyset()});
    REQUIRE(eq(*r1, *emptyset()));
    r1 = set_union({universalset()});
    REQUIRE(eq(*r1, *universalset()));
    r1 = set_union({f1});
    REQUIRE(eq(*r1, *f1));
    r1 = set_union({f1, emptyset(), universalset()});
    REQUIRE(eq(*r1, *universalset()));
    RCP<const Set> i1 = interval(zero, integer(3));
    RCP<const Set> i2 = interval(integer(4), integer(5));
    RCP<const Set> i3 = interval(integer(3), integer(4));
    r1 = set_union({i1, i2, i3});
    REQUIRE(eq(*r1, *interval(integer(0), integer(5))));

    i1 = interval(zero, one);
    i2 = interval(integer(3), integer(4));
    i3 = interval(integer(2), integer(3));
    RCP<const Set> r2 = set_union({i1, i2, i3});
    RCP<const Union> u = rcp_dynamic_cast<const Union>(r2);
    REQUIRE(u->container_.size() == 2);
    REQUIRE(u->container_.find(interval(zero, one)) != u->container_.end());
    REQUIRE(u->container_.find(interval(integer(2), integer(4)))
            != u->container_.end());
    REQUIRE(
        check_union_str(u->__str__(), {i1, interval(integer(2), integer(4))}));

    r2 = set_union({r1, r2});
    REQUIRE(eq(*r1, *r2));

    r2 = set_union({i1, i2, i3});
    r1 = set_union({finiteset({zero}), i2});
    u = rcp_dynamic_cast<const Union>(set_union({r1, r2}));
    REQUIRE(u->container_.find(interval(integer(2), integer(4)))
            != u->container_.end());
    REQUIRE(u->container_.find(interval(zero, one)) != u->container_.end());
    REQUIRE(
        check_union_str(u->__str__(), {i1, interval(integer(2), integer(4))}));
    REQUIRE(eq(*u->contains(one), *boolTrue));
    REQUIRE(eq(*u->contains(integer(2)), *boolTrue));
    REQUIRE(eq(*u->contains(integer(7)), *boolFalse));
    REQUIRE(u->is_superset(r1));
    REQUIRE(u->is_superset(r2));
    REQUIRE(u->is_superset(u));
    REQUIRE(r2->is_subset(u));
    REQUIRE(r1->is_subset(u));
    REQUIRE(u->is_subset(u));
    REQUIRE(u->is_proper_superset(r1));
    REQUIRE(not u->is_proper_superset(r2));
    REQUIRE(not u->is_proper_superset(u));
    REQUIRE(not r2->is_proper_subset(u));
    REQUIRE(r1->is_proper_subset(u));
    REQUIRE(not u->is_proper_subset(u));
}
