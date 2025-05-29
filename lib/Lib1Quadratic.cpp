
#include "../include/LibraryInterface.h"

// Implementation for a quadratic system:
// f1(x1, x2) = x1^2 + x2^2 - 4 = 0
// f2(x1, x2) = x1 - x2 - 1 = 0

extern "C" {
FUNCTION_EXPORT long double evaluateFunction(int i, int n,
                                             const long double *x) {
  // Expect x[1] and x[2] contain the values
  if (i == 1) return x[1] * x[1] + x[2] * x[2] - 4.0L;
  if (i == 2) return x[1] - x[2] - 1.0L;
  return 0.0L;
}

FUNCTION_EXPORT void evaluateDerivatives(int i, int n, const long double *x,
                                         long double *dfatx) {
  // Derivatives are stored starting at index 1
  if (i == 1) {
    dfatx[1] = 2.0L * x[1];
    dfatx[2] = 2.0L * x[2];
  } else if (i == 2) {
    dfatx[1] = 1.0L;
    dfatx[2] = -1.0L;
  }
}

FUNCTION_EXPORT const char *getName() { return "Quadratic System"; }

FUNCTION_EXPORT int getNumberOfEquations() { return 2; }
}