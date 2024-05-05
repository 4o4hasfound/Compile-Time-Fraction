#include "Fraction.hpp"

#define PrintFract(fract) std::cout << "Num: " << fract::num << " Denum: " << fract::denum << " Value: " << fract::value<<"\n"
#define PrintEqual(fract1, fract2) std::cout<<Equal<fract1, fract2>::value<<"\n"

int main() {
	std::cout << std::boolalpha;

	using fract1 = Fraction<5, -10>;	// 5/10 = 1/2
	using fract2 = Fraction<3, 10>;	// 3/10
	using fract3 = Fraction<8, 10>;	// 8/10 = 4/5
	using fract2add3 = FractionAdd<fract2, fract3>;	// (3+8)/10 = 11/10
	using fract3sub1 = FractionSub<fract3, fract1>; // (8-5)/10 = 3/10
	using fract1mul2mul3 = FractionMul<fract1, fract2, fract3>;
	// (5*3*8)/1000 = 120/1000 = 3/25
	using fractMax123 = Max<fract1, fract2, fract3>; // 8/10 = 4/5
	using fractMin13 = Min<fract1, fract3>; // 5/10 = 1/2


	// Output
	PrintFract(fract2add3); // Num: 11 Denum: 10 Value: 1.1
	PrintEqual(fract3sub1, fract2); // true
	PrintFract(fract1mul2mul3); // Num: 3 Denum: 25 Value: 0.12
	PrintEqual(fractMax123, fract3); // true
	PrintEqual(fractMin13, fract1); // true
}