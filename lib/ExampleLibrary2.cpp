#include "../include/LibraryInterface.h"
#include <cmath>
#include <cstring>

// Implementation for a system of 3 equations

extern "C" {
FUNCTION_EXPORT double evaluateFunction(int i, int n, const double *x) {
  if (i == 1)
    return 3 * x[1] - std::cos(x[2] * x[3]) - 0.5;
  if (i == 2)
    return x[1] * x[1] - 81 * (x[2] + 0.1) * (x[2] + 0.1) + std::sin(x[3]) +
           1.06;
  if (i == 3)
    return exp(-x[1] * x[2]) + 20 * x[3] + (10 * M_PI - 3) / 3;
  return 0;
}

FUNCTION_EXPORT void evaluateDerivatives(int i, int n, const double *x,
                                         double *dfatx) {
  if (i == 1) {
    dfatx[1] = 3;
    dfatx[2] = x[3] * std::sin(x[2] * x[3]);
    dfatx[3] = x[2] * std::sin(x[2] * x[3]);
  } else if (i == 2) {
    dfatx[1] = 2 * x[1];
    dfatx[2] = -162 * (x[2] + 0.1);
    dfatx[3] = std::cos(x[3]);
  } else if (i == 3) {
    dfatx[1] = -x[2] * exp(-x[1] * x[2]);
    dfatx[2] = -x[1] * exp(-x[1] * x[2]);
    dfatx[3] = 20;
  }
}

FUNCTION_EXPORT const char *getName() { return "Example 3-Equation System"; }

FUNCTION_EXPORT int getNumberOfEquations() { return 3; }
}