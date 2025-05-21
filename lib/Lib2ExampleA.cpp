#include <math.h>

#include "../include/LibraryInterface.h"

// Implementation for a system of 3 equations (Book example a.)

extern "C" {
FUNCTION_EXPORT long double evaluateFunction(int i, int n,
                                             const long double *x) {
  if (i == 1) return 3.0L * x[1] - std::cosl(x[2] * x[3]) - 0.5L;
  if (i == 2)
    return x[1] * x[1] - 81.0L * ((x[2] + 0.1L) * (x[2] + 0.1L)) +
           std::sinl(x[3]) + 1.06L;
  if (i == 3)
    return expl(-x[1] * x[2]) + 20.0L * x[3] + (10.0L * M_PIl - 3.0L) / 3.0L;
  return 0.0L;
}

FUNCTION_EXPORT void evaluateDerivatives(int i, int n, const long double *x,
                                         long double *dfatx) {
  if (i == 1) {
    dfatx[1] = 3.0L;
    dfatx[2] = x[3] * std::sinl(x[2] * x[3]);
    dfatx[3] = x[2] * std::sinl(x[2] * x[3]);
  } else if (i == 2) {
    dfatx[1] = 2.0L * x[1];
    dfatx[2] = -162.0L * (x[2] + 0.1L);
    dfatx[3] = std::cosl(x[3]);
  } else if (i == 3) {
    dfatx[1] = -x[2] * expl(-x[1] * x[2]);
    dfatx[2] = -x[1] * expl(-x[1] * x[2]);
    dfatx[3] = 20.0L;
  }
}

FUNCTION_EXPORT const char *getName() { return "ExampleA"; }

FUNCTION_EXPORT int getNumberOfEquations() { return 3; }
}