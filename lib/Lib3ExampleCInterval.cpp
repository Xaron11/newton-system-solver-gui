
#include "../include/LibraryInterfaceInterval.h"
#include "../include/NewtonSystemInterval.h"

// Implementation for a system of 2 equations (Book example c.)

extern "C" {
FUNCTION_EXPORT ValInterval evaluateFunction(int i, int n,
                                             const ValInterval *x) {
  if (i == 1) {
    int st = 0;
    return ISqr(x[1], st) + 8.0L * x[2] - ValInterval(16, 16);
  }
  if (i == 2) return x[1] - IExp(x[2]);
  return ValInterval(0, 0);
}

FUNCTION_EXPORT void evaluateDerivatives(int i, int n, const ValInterval *x,
                                         ValInterval *dfatx) {
  if (i == 1) {
    dfatx[1] = 2.0L * x[1];
    dfatx[2] = ValInterval(8, 8);
  } else if (i == 2) {
    dfatx[1] = ValInterval(1, 1);
    dfatx[2] = IExp(x[2]).Opposite();
  }
}

FUNCTION_EXPORT const char *getName() { return "ExampleC (Interval)"; }

FUNCTION_EXPORT int getNumberOfEquations() { return 2; }
}