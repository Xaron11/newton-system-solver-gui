#ifndef __NEWTONSYSTEM_H__
#define __NEWTONSYSTEM_H__

#include <vector>

namespace NStandard {

using Val = long double;
using Vector = std::vector<Val>;
using FunctionType = Val (*)(int i, int n, const Vector &x);
using DerivativeType = void (*)(int i, int n, const Vector &x, Vector &dfatx);
using FunctionTypeC = Val (*)(int i, int n, const Val *x);
using DerivativeTypeC = void (*)(int i, int n, const Val *x, Val *dfatx);

void NewtonSystem(int n, Vector &x, FunctionTypeC f, DerivativeTypeC df,
                  int mit, Val eps, int &it, int &st);
}  // namespace NStandard
#endif  // __NEWTONSYSTEM_H__