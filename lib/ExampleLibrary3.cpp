#include <cmath>
#include <cstring>

#include "../include/LibraryInterface.h"

// Implementation for a system of 2 equations

extern "C" {
FUNCTION_EXPORT long double evaluateFunction(int i, int n,
                                             const long double *x) {
  if (i == 1) return x[1] * x[1] + 8.0L * x[2] - 16.0L;
  if (i == 2) return x[1] - std::exp(x[2]);
  return 0.0L;
}

FUNCTION_EXPORT void evaluateDerivatives(int i, int n, const long double *x,
                                         long double *dfatx) {
  if (i == 1) {
    dfatx[1] = 2.0L * x[1];
    dfatx[2] = 8.0L;
  } else if (i == 2) {
    dfatx[1] = 1.0L;
    dfatx[2] = -std::exp(x[2]);
  }
}

FUNCTION_EXPORT const char *getName() { return "Example 3-Equation System"; }

FUNCTION_EXPORT int getNumberOfEquations() { return 2; }
}