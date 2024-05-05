#ifndef FRACTION_INCLUDED
#define FRACTION_INCLUDED

#include<iostream>
#include<limits.h>
using sizet = std::size_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

// -----------------------------------------
//
//   Compile time GCD
//
// -----------------------------------------

namespace {
    template<int64 N, int64 M, int64 K>
    struct A;

    template<int64 N, int64 M>
    struct GCD {
        static constexpr int64 value = A<N, M, N% M>::value;
    };
    template<int64 N, int64 M, int64 K>
    struct A {
        static constexpr int64 value = A<M, K, M% K>::value;
    };
    template<int64 N, int64 M>
    struct A<N, M, 0> {
        static constexpr int64 value = M;
    };

    inline constexpr int64 constexpr_abs(int64 N) {
        return N < 0 ? -N : N;
    }
}


// -----------------------------------------
//
//   declaration
//
// -----------------------------------------

template<int64 _Num, int64 _DeNum>
struct Fraction;
template<typename _Fract>
struct Negative;
template<typename _Fract>
struct Inverse;
template<typename _Fract, int64 n>
struct Power;

template<typename _Fract1, typename _Fract2, typename ... _Rest>
struct FractionAdd;
template<typename _Fract1, int64 scalar>
struct FractionAddScalar;

template<typename _Fract1, typename _Fract2, typename ... _Rest>
struct FractionSub;
template<typename _Fract1, int64 scalar>
struct FractionSubScalar;

template<typename _Fract1, typename _Fract2, typename ... _Rest>
struct FractionMul;
template<typename _Fract1, int64 scalar>
struct FractionMulScalar;

template<typename _Fract1, typename _Fract2>
struct FractionDiv;
template<typename _Fract1, int64 scalar>
struct FractionDivScalar;

template<typename _Fract1, typename _Fract2>
struct Equal;
template<typename _Fract1, typename _Fract2>
struct Not_Equal;
template<typename _Fract1, typename _Fract2>
struct Less;
template<typename _Fract1, typename _Fract2>
struct Less_Equal;
template<typename _Fract1, typename _Fract2>
struct Greater;
template<typename _Fract1, typename _Fract2>
struct Greater_Equal;

template<typename _Fract1, typename _Fract2, typename ... _Rest>
struct Max;
template<typename _Fract1, typename _Fract2, typename ... _Rest>
struct Min;

// -----------------------------------------
//
//   Definition
//
// -----------------------------------------

// Basic struct representing a fraction
template<int64 _Num, int64 _DeNum>
struct Fraction {
    static_assert(_DeNum != 0, "Denum cannot be 0");

    // The constexpr_abs and the multiplication by -1
    // makes that the negative sign is always in num
    static constexpr int64 num      = 
        _Num 
        / ::constexpr_abs(::GCD<_Num, _DeNum>::value) 
        * (_DeNum < 0 ? -1 : 1);
    static constexpr int64 denum    = 
        _DeNum 
        / ::constexpr_abs(::GCD<_Num, _DeNum>::value)
        * (_DeNum < 0 ? -1 : 1);
    static constexpr double value   = static_cast<double>(_Num) / _DeNum;
};

// Negate a fraction
template<typename _Fract>
struct Negative {
    static constexpr int64  num = -_Fract::num;
    static constexpr int64  denum = _Fract::denum;
    static constexpr double value = static_cast<double>(num) / denum;
};

// Inverse a fraction
template<typename _Fract>
struct Inverse {
    static constexpr int64  num     = _Fract::denum;
    static constexpr int64  denum   = _Fract::num;
    static constexpr double value   = static_cast<double>(num) / denum;
};

// Add two fractions together
template<typename _Fract1, typename _Fract2>
struct FractionAdd<_Fract1, _Fract2> {
    static constexpr int64 num      = Fraction<
        _Fract1::num* _Fract2::denum + _Fract1::denum * _Fract2::num,
        _Fract1::denum* _Fract2::denum
    >::num;
    static constexpr int64 denum    = Fraction<
        _Fract1::num* _Fract2::denum + _Fract1::denum * _Fract2::num,
        _Fract1::denum* _Fract2::denum
    >::denum;
    static constexpr double value   = static_cast<double>(num) / denum;
};

// Add multiple fractions together
template<typename _Fract1 = Fraction<0, 1>, typename _Fract2 = Fraction<0, 1>, typename ... _Rest>
struct FractionAdd {
    static constexpr int64 num      = FractionAdd<FractionAdd<_Fract1, _Fract2>, FractionAdd<_Rest...> >::num;
    static constexpr int64 denum    = FractionAdd<FractionAdd<_Fract1, _Fract2>, FractionAdd<_Rest...> >::denum;
    static constexpr double value   = static_cast<double>(num) / denum;
};

// Add a scalar to a fraction
template<typename _Fract1, int64 scalar>
struct FractionAddScalar {
    static constexpr int64  num     = FractionAdd<Fraction<scalar, scalar> >::num;
    static constexpr int64  denum   = FractionAdd<Fraction<scalar, scalar> >::denum;
    static constexpr double value   = static_cast<double>(num) / denum;
};

// Subtract two fractions
template<typename _Fract1, typename _Fract2>
struct FractionSub<_Fract1, _Fract2> {
    static constexpr int64  num     = FractionAdd<_Fract1, FractionMulScalar<_Fract2, -1> >::num;
    static constexpr int64  denum   = FractionAdd<_Fract1, FractionMulScalar<_Fract2, -1> >::denum;
    static constexpr double value   = static_cast<double>(num) / denum;
};

// Subtract multiple fractions
template<typename _Fract1 = Fraction<0, 1>, typename _Fract2 = Fraction<0, 1>, typename ... _Rest>
struct FractionSub {
    static constexpr int64  num     = FractionSub<FractionSub<_Fract1, _Fract2>, FractionAdd<_Rest...> >::num;
    static constexpr int64  denum   = FractionSub<FractionSub<_Fract1, _Fract2>, FractionAdd<_Rest...> >::denum;
    static constexpr double value   = static_cast<double>(num) / denum;
};

// Subtract a scalar from a fraction 
template<typename _Fract1, int64 scalar>
struct FractionSubScalar {
    static constexpr int64  num     = FractionAddScalar<_Fract1, Fraction<-scalar, scalar> >::num;
    static constexpr int64  denum   = FractionAddScalar<_Fract1, Fraction<-scalar, scalar> >::denum;
    static constexpr double value   = static_cast<double>(num) / denum;
};

// Multiply two fractions
template<typename _Fract1, typename _Fract2>
struct FractionMul<_Fract1, _Fract2> {
    static constexpr int64  num     = Fraction< (_Fract1::num * _Fract2::num), (_Fract1::denum * _Fract2::denum) >::num;
    static constexpr int64  denum   = Fraction< (_Fract1::num * _Fract2::num), (_Fract1::denum * _Fract2::denum) >::denum;
    static constexpr double value   = static_cast<double>(num) / denum;
};

// Multiply multiple fractions
template<typename _Fract1 = Fraction<1, 1>, typename _Fract2 = Fraction<1, 1>, typename ... _Rest>
struct FractionMul {
    static constexpr int64  num     = FractionMul<FractionMul<_Fract1, _Fract2>, FractionMul<_Rest...> >::num;
    static constexpr int64  denum   = FractionMul<FractionMul<_Fract1, _Fract2>, FractionMul<_Rest...> >::denum;
    static constexpr double value   = static_cast<double>(num) / denum;
};

// Multiply a scalar to a fraction
template<typename _Fract1, int64 scalar>
struct FractionMulScalar {
    static constexpr int64  num     = Fraction< (_Fract1::num* scalar), (_Fract1::denum) >::num;
    static constexpr int64  denum   = Fraction< (_Fract1::num* scalar), (_Fract1::denum) >::denum;
    static constexpr double value   = static_cast<double>(num) / denum;
};

// Divide two fractions
template<typename _Fract1, typename _Fract2>
struct FractionDiv {
    static constexpr int64  num     = FractionMul<_Fract1, Inverse<_Fract2> >::num;
    static constexpr int64  denum   = FractionMul<_Fract1, Inverse<_Fract2> >::denum;
    static constexpr double value   = static_cast<double>(num) / denum;
};

// Divide a fraction from a scalar
template<typename _Fract1, int64 scalar>
struct FractionDivScalar {
    static constexpr int64  num     = FractionMul<_Fract1, Fraction<1, scalar> >::num;
    static constexpr int64  denum   = FractionMul<_Fract1, Fraction<1, scalar> >::denum;
    static constexpr double value   = static_cast<double>(num) / denum;
};

template<typename _Fract>
struct Power<_Fract, 0> {
    static constexpr int64  num     = 1;
    static constexpr int64  denum   = 1;
    static constexpr double value   = static_cast<double>(num) / denum;
};

template<typename _Fract>
struct Power<_Fract, 1> {
    static constexpr int64  num     = _Fract::num;
    static constexpr int64  denum   = _Fract::denum;
    static constexpr double value   = static_cast<double>(num) / denum;
};

// Raise a fraction to the power of n
template<typename _Fract, int64 n>
struct Power {
    static_assert(n >= 0, "n need to be a number bigger or equal than 0");
    static constexpr int64  num     = FractionMul<Power<_Fract, n / 2>, Power<_Fract, n - n / 2> >::num;
    static constexpr int64  denum   = FractionMul<Power<_Fract, n / 2>, Power<_Fract, n - n / 2> >::denum;
    static constexpr double value   = static_cast<double>(num) / denum;
};

template<typename _Fract1, typename _Fract2>
struct Equal {
    static constexpr bool value = _Fract1::denum == _Fract2::denum && _Fract1::num == _Fract2::num;
};

template<typename _Fract1, typename _Fract2>
struct Not_Equal {
    static constexpr bool value = _Fract1::denum != _Fract2::denum || _Fract1::num != _Fract2::num;
};

template<typename _Fract1, typename _Fract2>
struct Less {
    static constexpr bool value = _Fract1::num * _Fract2::denum < _Fract2::num* _Fract1::denum;
};
template<typename _Fract1, typename _Fract2>
struct Less_Equal {
    static constexpr bool value = _Fract1::num * _Fract2::denum <= _Fract2::num* _Fract1::denum;
};
template<typename _Fract1, typename _Fract2>
struct Greater {
    static constexpr bool value = _Fract1::num * _Fract2::denum > _Fract2::num* _Fract1::denum;
};
template<typename _Fract1, typename _Fract2>
struct Greater_Equal {
    static constexpr bool value = _Fract1::num * _Fract2::denum >= _Fract2::num* _Fract1::denum;
};

// Get the max of two fractions
template<typename _Fract1, typename _Fract2>
struct Max<_Fract1, _Fract2> {
    static constexpr int64 num = _Fract1::value > _Fract2::value ? _Fract1::num : _Fract2::num;
    static constexpr int64 denum = _Fract1::value > _Fract2::value ? _Fract1::denum : _Fract2::denum;
    static constexpr double value = static_cast<double>(num) / denum;
};
// Get the max of muiltiple fractions
template<typename _Fract1 = Fraction<std::numeric_limits<int64>::min(), 1>, typename _Fract2 = Fraction<std::numeric_limits<int64>::min(), 1>, typename ... _Rest>
struct Max {
    static constexpr int64 num = Max<Max<_Fract1, _Fract2>, Max<_Rest...> >::num;
    static constexpr int64 denum = Max<Max<_Fract1, _Fract2>, Max<_Rest...> >::denum;
    static constexpr double value = static_cast<double>(num) / denum;
};

// Get the min of two fractions
template<typename _Fract1, typename _Fract2>
struct Min<_Fract1, _Fract2> {
    static constexpr int64 num = _Fract1::value < _Fract2::value ? _Fract1::num : _Fract2::num;
    static constexpr int64 denum = _Fract1::value < _Fract2::value ? _Fract1::denum : _Fract2::denum;
    static constexpr double value = static_cast<double>(num) / denum;
};
// Get the min of multiple fractions
template<typename _Fract1 = Fraction<std::numeric_limits<int64>::max(), 1>, typename _Fract2 = Fraction<std::numeric_limits<int64>::max(), 1>, typename ... _Rest>
struct Min {
    static constexpr int64 num = Min<Min<_Fract1, _Fract2>, Min<_Rest...> >::num;
    static constexpr int64 denum = Min<Min<_Fract1, _Fract2>, Min<_Rest...> >::denum;
    static constexpr double value = static_cast<double>(num) / denum;
};

#endif