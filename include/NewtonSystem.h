#ifndef __NEWTONSYSTEM_H__
#define __NEWTONSYSTEM_H__

#include <vector>

using Vector = std::vector<long double>;
using FunctionType = long double (*)(int i, int n, const Vector &x);
using DerivativeType = void (*)(int i, int n, const Vector &x, Vector &dfatx);
using FunctionTypeC = long double (*)(int i, int n, const long double *x);
using DerivativeTypeC = void (*)(int i, int n, const long double *x,
                                 long double *dfatx);

void NewtonSystem(int n, Vector &x, FunctionTypeC f, DerivativeTypeC df,
                  int mit, long double eps, int &it, int &st);

#endif  // __NEWTONSYSTEM_H__