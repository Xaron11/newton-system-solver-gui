
#include "../include/LibraryInterfaceInterval.h"
#include "../include/NewtonSystemInterval.h"

// Implementation for a system of 2 equations

extern "C" {
FUNCTION_EXPORT ValInterval evaluateFunction(int i, int n,
                                             const ValInterval *x) {
  if (i == 1) return x[1] * x[1] + 8.0L * x[2] - ValInterval(16, 16);
  if (i == 2) return x[1] - IAbs(x[2]);
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

FUNCTION_EXPORT const char *getName() { return "Example2 System (Interval)"; }

FUNCTION_EXPORT int getNumberOfEquations() { return 2; }
}