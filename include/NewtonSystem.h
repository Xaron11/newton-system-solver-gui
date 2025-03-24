#ifndef __NEWTONSYSTEM_H__
#define __NEWTONSYSTEM_H__

#include <vector>

using Vector = std::vector<double>;
using FunctionType = double (*)(int i, int n, const Vector &x);
using DerivativeType = void (*)(int i, int n, const Vector &x, Vector &dfatx);
using FunctionTypeC = double (*)(int i, int n, const double *x);
using DerivativeTypeC = void (*)(int i, int n, const double *x, double *dfatx);

void NewtonSystem(int n, Vector &x, FunctionTypeC f, DerivativeTypeC df,
                  int mit, double eps, int &it, int &st);

#endif // __NEWTONSYSTEM_H__