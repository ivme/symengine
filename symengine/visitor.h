/**
 *  \file visitor.h
 *  Class Visitor
 *
 **/
#ifndef SYMENGINE_VISITOR_H
#define SYMENGINE_VISITOR_H

#include <symengine/polys/uintpoly_flint.h>
#include <symengine/polys/uintpoly_piranha.h>
#include <symengine/polys/uexprpoly.h>
#include <symengine/polys/msymenginepoly.h>
#include <symengine/polys/uratpoly.h>
#include <symengine/complex_mpc.h>
#include <symengine/series_generic.h>
#include <symengine/series_piranha.h>
#include <symengine/series_flint.h>
#include <symengine/series_generic.h>
#include <symengine/series_piranha.h>
#include <symengine/sets.h>
#include <symengine/fields.h>
#include <symengine/logic.h>
#include <symengine/infinity.h>

namespace SymEngine
{

/*
 * Abstract base class for all visitors (see https://en.wikipedia.org/wiki/Visitor_pattern).
 * Has pure virtual visit methods for all symengine types, 
 * but these visit methods do not appear in doxygen 
 * because they are included via prepocesser directive.
 */
class Visitor
{
public:
#define SYMENGINE_ENUM(TypeID, Class) virtual void visit(const Class &) = 0;
#include "symengine/type_codes.inc"
#undef SYMENGINE_ENUM
};

/* Traverses the argument tree of \c b.
 * Calls \c b.accept(v), then recursively processes arguments of \c b.
 * @param[in] b The Basic instance to be processed.
 * @param[in] v The Visitor instance that will process nodes in \c b's tree.
 */
void preorder_traversal(const Basic &b, Visitor &v);

/* Traverses the argument tree of \c b.
 * Recursively processes arguments of \c b, then calls \c b.accept(v).
 * @param[in] b The Basic instance to be processed.
 * @param[in] v The Visitor instance that will process nodes in \c b's tree.
 */
void postorder_traversal(const Basic &b, Visitor &v);

/*
 * Templated base class for visitors.
 * Instantiations should specify a derived class of \c Visitor 
 * for the template parameter \c Base.  Instantiations of \c BaseVisitor 
 * thus inherit from Visitor.
 * BaseVisitor implements static polymorphism via the curiously recurring template pattern.
 * In particular, BaseVisitor implements the \c visit method for 
 * @tparam Derived A class that inherits from \c BaseVisitor.  Must implement \c bvisit(const Basic).
 * @tparam Base A class that inherits from \c Visitor (e.g. \c StopVisitor).
 */
template <class Derived, class Base = Visitor>
class BaseVisitor : public Base
{

public:
    // Following two ctors can be replaced by `using Base::Base` if inheriting
    // constructors are allowed by the compiler. GCC 4.8 is the latest version
    // supporting this.
    template <typename... Args,
              typename
              = enable_if_t<std::is_constructible<Base, Args...>::value>>
    BaseVisitor(Args &&... args)
        : Base(std::forward<Args>(args)...)
    {
    }

    BaseVisitor() : Base()
    {
    }

#define SYMENGINE_ENUM(TypeID, Class)                                          \
    virtual void visit(const Class &x)                                         \
    {                                                                          \
        static_cast<Derived *>(this)->bvisit(x);                               \
    };
#include "symengine/type_codes.inc"
#undef SYMENGINE_ENUM
};

/*
 * Abstract base class for \c Visitors that may stop
 * recursively processing nodes before the tree has
 * been fully traversed.  Processing stops when \c stop_ is set to \c true.
 */
class StopVisitor : public Visitor
{
public:
    bool stop_;
};

/* Traverses the argument tree of \c b.
 * Calls \c b.accept(v), then recursively processes arguments of \c b.
 * Recursive processing stops if \c v.stop_ == true.
 * @param[in] b The Basic instance to be processed.
 * @param[in] v The Visitor instance that will process nodes in \c b's tree.
 */
void preorder_traversal_stop(const Basic &b, StopVisitor &v);
/* Traverses the argument tree of \c b.
 * Calls \c b.accept(v), then recursively processes arguments of \c b.
 * Recursive processing stops if \c v.stop_ == true.
 * @param[in] b The Basic instance to be processed.
 * @param[in] v The Visitor instance that will process nodes in \c b's tree.
 */
void postorder_traversal_stop(const Basic &b, StopVisitor &v);

/*
 * Visitor for determining whether a given symbol
 * is present an expression.
 */
class HasSymbolVisitor : public BaseVisitor<HasSymbolVisitor, StopVisitor>
{
protected:
    /*
     * The symbol for which to search.
     */
    Ptr<const Symbol> x_;
    /*
     * If true, then \c x_ is present in the expression.
     */
    bool has_;

public:
    /*
     * @param x The symbol for which to search.
     */
    HasSymbolVisitor(Ptr<const Symbol> x) : x_(x)
    {
    }

    /*
     * Determines whether \c x is the symbol
     * we are looking for.  If so, recursive traversal stops.
     * @param x The symbol to be compared against the desired symbol, \c x_.
     */
    void bvisit(const Symbol &x)
    {
        if (x_->__eq__(x)) {
            has_ = true;
            stop_ = true;
        }
    }

    /*
     * Visit function for all SymEngine types that are not Symbols.
     * Does nothing.
     * @param x A SymEngine object that is not a Symbol.
     */
    void bvisit(const Basic &x){};

    /*
     * Determines whether \c b's argument tree contains \c x_.
     * \return {\c true if \c b's argument tree contains \c x_, otherwise \c false.}
     */
    bool apply(const Basic &b)
    {
        has_ = false;
        stop_ = false;
        preorder_traversal_stop(b, *this);
        return has_;
    }
};

/*
 * Determines whether the symbol \c x is present in \c b's argument tree.
 * @param b an expression to be searched.
 * @param x the desired symbol.
 * \return {\c true if \c b's argument tree contains \c x, otherwise \c false.}
 */
bool has_symbol(const Basic &b, const Symbol &x);

/*
 * 
 */
class CoeffVisitor : public BaseVisitor<CoeffVisitor, StopVisitor>
{
protected:
    Ptr<const Symbol> x_;
    Ptr<const Basic> n_;
    RCP<const Basic> coeff_;

public:
    CoeffVisitor(Ptr<const Symbol> x, Ptr<const Basic> n) : x_(x), n_(n)
    {
    }

    void bvisit(const Add &x)
    {
        umap_basic_num dict;
        RCP<const Number> coef = zero;
        for (auto &p : x.dict_) {
            p.first->accept(*this);
            if (neq(*coeff_, *zero)) {
                Add::coef_dict_add_term(outArg(coef), dict, p.second, coeff_);
            }
        }
        coeff_ = Add::from_dict(coef, std::move(dict));
    }

    /*
     * Sets coeff_ equal to the coefficient of \c x_**n_ in a product x.
     * Examples:
     * If *x_ == a, *n_ == 2 and x == 3*a**2, then bvisit(x) assigns \c coeff_ = 3.
     * If *x_ == a, *n_ == 2 and x == 3*a**2*b, then bvisit(x) assigns \c coeff_ = 3b.
     * If *x_ == a, *n_ == 1 and x == 3*a**2*b, then bvisit(x) assigns \c coeff_ = 0.
     * If *x_ == c, *n_ == 1 and x == 3*a**2*b, then bvisit(x) assigns \c coeff_ = 0.
     * @param x the term in which to find the coefficient of \c x_**n_
     */ 
    void bvisit(const Mul &x)
    {
        for (auto &p : x.dict_) {
            if (eq(*p.first, *x_) and eq(*p.second, *n_)) {
                map_basic_basic dict = x.dict_;
                dict.erase(p.first);
                coeff_ = Mul::from_dict(x.coef_, std::move(dict));
                return;
            }
        }
        coeff_ = zero;
    }

    /*
     * Assigns \c coeff_ = 1 if the base of \c x is \c x_ and the exponent of \c x is \c n_.
     * Otherwise \c coeff_ = 0.
     */
    void bvisit(const Pow &x)
    {
        if (eq(*x.get_base(), *x_) and eq(*x.get_exp(), *n_)) {
            coeff_ = one;
        } else {
            coeff_ = zero;
        }
    }

    /*
     * Assigns \c coeff_ = 1 if x_ == x and n_ == 1.
     * Otherwise \c coeff_ = 0.
     */
    void bvisit(const Symbol &x)
    {
        if (eq(x, *x_) and eq(*one, *n_)) {
            coeff_ = one;
        } else {
            coeff_ = zero;
        }
    }

    /*
     * Assigns \c coeff_ = 0
     * This overload is called for any argument that is not a \c Mul, \c Pow, or \c Symbol.
     */
    void bvisit(const Basic &x)
    {
        coeff_ = zero;
    }

    /*
     * Visits b.
     * @param b an expression in which to find the coefficient of x_**n_.
     * \return {The coefficient of \c x_**n_ in expression \c b.}
     */
    RCP<const Basic> apply(const Basic &b)
    {
        coeff_ = zero;
        b.accept(*this);
        return coeff_;
    }
};

/*
 * Determines the coefficient of \c x**n in expression \c b.
 * @param b an expression in which to find the coefficient of x**n.
 * @param x the base
 * @param n the exponent
 * \return {the coefficient of \c x**n in expression \c b}
 */
RCP<const Basic> coeff(const Basic &b, const Basic &x, const Basic &n);

/*
 * \return {the set of free symbols expression \c b.}
 */
set_basic free_symbols(const Basic &b);

} // SymEngine

#endif
