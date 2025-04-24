#include <cmath>
#include <cstring>

#include "../include/LibraryInterface.h"

// Implementation for a system of 3 equations

extern "C" {
FUNCTION_EXPORT long double evaluateFunction(int i, int n,
                                             const long double *x) {
  if (i == 1) return 3.0L * x[1] - std::cos(x[2] * x[3]) - 0.5L;
  if (i == 2)
    return x[1] * x[1] - 81.0L * (x[2] + 0.1L) * (x[2] + 0.1L) +
           std::sin(x[3]) + 1.06L;
  if (i == 3)
    return exp(-x[1] * x[2]) + 20.0L * x[3] + (10.0L * M_PIl - 3.0L) / 3.0L;
  return 0.0L;
}

FUNCTION_EXPORT void evaluateDerivatives(int i, int n, const long double *x,
                                         long double *dfatx) {
  if (i == 1) {
    dfatx[1] = 3.0L;
    dfatx[2] = x[3] * std::sin(x[2] * x[3]);
    dfatx[3] = x[2] * std::sin(x[2] * x[3]);
  } else if (i == 2) {
    dfatx[1] = 2.0L * x[1];
    dfatx[2] = -162.0L * (x[2] + 0.1L);
    dfatx[3] = std::cos(x[3]);
  } else if (i == 3) {
    dfatx[1] = -x[2] * exp(-x[1] * x[2]);
    dfatx[2] = -x[1] * exp(-x[1] * x[2]);
    dfatx[3] = 20.0L;
  }
}

FUNCTION_EXPORT const char *getName() { return "Example 3-Equation System"; }

FUNCTION_EXPORT int getNumberOfEquations() { return 3; }
}