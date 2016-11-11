/**
 *  \file uintpoly.h
 *  Class for sparse Polynomial: UIntPoly
 **/
#ifndef SYMENGINE_UINTPOLY_H
#define SYMENGINE_UINTPOLY_H

#include <symengine/polys/usymenginepoly.h>
//DEBUG
#include <iostream>
using std::cout;
using std::endl;

namespace SymEngine
{
// Calculates bit length of number, used in UIntDict*= only
template <typename T>
unsigned int bit_length(T t)
{
    unsigned int count = 0;
    while (t > 0) {
        count++;
        t = t >> 1;
    }
    return count;
}

class UIntDict : public ODictWrapper<unsigned int, integer_class, UIntDict>
{

public:
    UIntDict() SYMENGINE_NOEXCEPT
    {
        cout << "UIntDict()" << endl;
    }
    ~UIntDict() SYMENGINE_NOEXCEPT
    {
    }
    UIntDict(UIntDict &&other) SYMENGINE_NOEXCEPT
        : ODictWrapper(std::move(other))
    {
        cout << "UIntDict(UIntDict &&other)" << endl;
    }
    UIntDict(const int &i) : ODictWrapper(i)
    {
        cout << "UIntDict(const int &i)" << endl;
    }
    UIntDict(const map_uint_mpz &p) : ODictWrapper(p)
    {
        cout << "UIntDict(const map_uint_mpz &p)" << endl;
    }
    UIntDict(const integer_class &i) : ODictWrapper(i)
    {
        cout << "UIntDict(const integer_class &i)" << endl;
    }

    UIntDict(const UIntDict &other) : ODictWrapper(other)
    {
        cout << "UIntDict(const UIntDict &)" << endl;
    }
    
    UIntDict &operator=(const UIntDict &other) {
        cout << "assignment operator UIntDict::operator=(const UIntDict &)" << endl;
        cout << "this == " << this << endl;
        cout << "&other == " << &other << endl;
        cout << "this == &other: " << std::boolalpha << (this == &other) << endl;
        cout << "&(this->dict_) == " << &(this->dict_) << endl;
        cout << "&(other.dict_) == " << &(other.dict_) << endl;
        cout << "&(this->dict_) == &(other.dict_): " << (&(this->dict_) == &(other.dict_)) << endl;
        this->dict_ = other.dict_;
        cout << "after this->dict_ = other.dict_;" << endl;
        return *this;
    }

    //! Evaluates the dict_ at value 2**x
    integer_class eval_bit(const unsigned int &x) const
    {
        unsigned int last_deg = dict_.rbegin()->first;
        integer_class result(0);

        for (auto it = dict_.rbegin(); it != dict_.rend(); ++it) {
            result <<= x * (last_deg - (*it).first);
            result += (*it).second;
            last_deg = (*it).first;
        }
        result <<= x * last_deg;

        return result;
    }

    static UIntDict mul(const UIntDict &a, const UIntDict &b)
    {
        //DEBUG
        cout << "body of UIntDict::mul(const UIntDict &a, const UIntDict &b)" << endl;
        int mul = 1;

        unsigned int N = bit_length(std::min(a.degree() + 1, b.degree() + 1))
                         + bit_length(a.max_abs_coef())
                         + bit_length(b.max_abs_coef());

        integer_class full = integer_class(1), temp, res;
        full <<= N;
        integer_class thresh = full / 2;
        integer_class mask = full - 1;
        integer_class s_val = a.eval_bit(N) * b.eval_bit(N);
        if (s_val < 0)
            mul = -1;
        s_val = mp_abs(s_val);

        unsigned int deg = 0, carry = 0;
        UIntDict r;

        //DEBUG
        cout << "before while loop in UIntDict::mul" << endl;
        while (s_val != 0 or carry != 0) {
            cout << "mp_and(" << temp << "," << s_val << "," << mask << ")" << endl;
            mp_and(temp, s_val, mask);
            if (temp < thresh) {
                cout << "temp < thresh: temp == " << temp << ", thresh == " << thresh << endl;
                res = mul * (temp + carry);
                if (res != 0) {
                    //DEBUG
                    cout << "deg == " << deg << endl;
                    r.dict_[deg] = res;
                    cout << "after r.dic_[deg] = res;" << endl;
                }
                carry = 0;
            } else {
                cout << "temp >= thresh: temp == " << temp << ", thresh == " << thresh << endl;
                res = mul * (temp - full + carry);
                if (res != 0)
                    r.dict_[deg] = res;
                carry = 1;
            }
            cout << "s_val == " << s_val << endl;
            s_val >>= N;
            cout << "after s_val >>= N" << endl;
            deg++;
        }
        cout << "returning from UIntDict::mul..." << endl;
        return r;
    }

    int compare(const UIntDict &other) const
    {
        if (dict_.size() != other.dict_.size())
            return (dict_.size() < other.dict_.size()) ? -1 : 1;
        return unified_compare(dict_, other.dict_);
    }

    integer_class max_abs_coef() const
    {
        integer_class curr(mp_abs(dict_.begin()->second));
        for (const auto &it : dict_) {
            if (mp_abs(it.second) > curr)
                curr = mp_abs(it.second);
        }
        return curr;
    }

}; // UIntDict

class UIntPoly : public USymEnginePoly<UIntDict, UIntPolyBase, UIntPoly>
{
public:
    IMPLEMENT_TYPEID(UINTPOLY)
    //! Constructor of UIntPoly class
    UIntPoly(const RCP<const Basic> &var, UIntDict &&dict);

    //! \return size of the hash
    hash_t __hash__() const;
}; // UIntPoly

// true & sets `out` to b/a if a exactly divides b, otherwise false & undefined
bool divides_upoly(const UIntPoly &a, const UIntPoly &b,
                   const Ptr<RCP<const UIntPoly>> &res);

} // SymEngine

#endif
