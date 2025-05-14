#ifndef __NEWTONSYSTEM_INTERVAL_H__
#define __NEWTONSYSTEM_INTERVAL_H__

#include <vector>

#include "./Interval.h"

namespace NInterval {
using ValInterval = interval_arithmetic::Interval<long double>;

using Vector = std::vector<ValInterval>;
using FunctionType = ValInterval (*)(int i, int n, const Vector &x);
using DerivativeType = void (*)(int i, int n, const Vector &x, Vector &dfatx);
using FunctionTypeC = ValInterval (*)(int i, int n, const ValInterval *x);
using DerivativeTypeC = void (*)(int i, int n, const ValInterval *x,
                                 ValInterval *dfatx);

void NewtonSystem(int n, Vector &x, FunctionTypeC f, DerivativeTypeC df,
                  int mit, ValInterval eps, int &it, int &st);

}  // namespace NInterval
#endif  // __NEWTONSYSTEM_INTERVAL_H__