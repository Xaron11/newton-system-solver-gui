

#include "../include/LibraryInterfaceInterval.h"
#include "../include/NewtonSystemInterval.h"

// Implementation for a system of 3 equations (Book example a.)

extern "C" {
FUNCTION_EXPORT ValInterval evaluateFunction(int i, int n,
                                             const ValInterval *x) {
  if (i == 1) return 3.0L * x[1] - ICos(x[2] * x[3]) - ValInterval(0.5L, 0.5L);
  if (i == 2) {
    int st = 0;
    return ISqr(x[1], st) - 81.0L * ISqr(x[2] + ValInterval(0.1L, 0.1L), st) +
           ISin(x[3]) + ValInterval(1.06L, 1.06L);
  }
  if (i == 3)
    return IExp(x[1].Opposite() * x[2]) + 20.0L * x[3] +
           (10.0L * ValInterval::IPi() - ValInterval(3.0L, 3.0L)) /
               ValInterval(3.0L, 3.0L);
  return ValInterval(0.0L, 0.0L);
}

FUNCTION_EXPORT void evaluateDerivatives(int i, int n, const ValInterval *x,
                                         ValInterval *dfatx) {
  if (i == 1) {
    dfatx[1] = ValInterval(3.0L, 3.0L);
    dfatx[2] = x[3] * ISin(x[2] * x[3]);
    dfatx[3] = x[2] * ISin(x[2] * x[3]);
  } else if (i == 2) {
    dfatx[1] = 2.0L * x[1];
    dfatx[2] = -162.0L * (x[2] + ValInterval(0.1L, 0.1L));
    dfatx[3] = ICos(x[3]);
  } else if (i == 3) {
    dfatx[1] = x[2].Opposite() * IExp(x[1].Opposite() * x[2]);
    dfatx[2] = x[1].Opposite() * IExp(x[1].Opposite() * x[2]);
    dfatx[3] = ValInterval(20.0L, 20.0L);
  }
}

FUNCTION_EXPORT const char *getName() { return "ExampleA (Interval)"; }

FUNCTION_EXPORT int getNumberOfEquations() { return 3; }
}