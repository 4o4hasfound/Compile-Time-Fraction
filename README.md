# Compile Time Fraction

## Description

This is a simple compile time fraction header I wrote. It supports:

1. Addition         (with either fraction or scalar)
2. Subtraction      (with either fraction or scalar)
3. Multiplication   (with either fraction or scalar)
4. Devision         (with either fraction or scalar)
5. Exponentiate
6. Min/Max
7. Inversion
8. Negate

All the fractions will be reduced to the simplest term
Ex: 5/10 will be reduced to 1/2

## Basic Usage

This is what a basic fraction looks like

```c++
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

```

Inorder to create a fraction, simply do

```c++
using fract = Fraction<5, 10>;
```

And there you have a fraction that represents 1/2!

## Fraction Operations

Every operation itself is another fraction, so you can chain your operations!

```c++
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
```

To chain your operations, simply do:

```c++
using fract1 = Fraction<1, 2>;
using fract2 = Fraction<3, 2>;
using fract3 = Fraction<5, 2>;
using newFract = FractionAdd<Inverse<fract1>, fract2, Negative<fract3>, ...>;
```

