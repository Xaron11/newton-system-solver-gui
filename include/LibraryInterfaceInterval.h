#ifndef __LIBRARYINTERFACE_INTERVAL_H__
#define __LIBRARYINTERFACE_INTERVAL_H__

#include "./NewtonSystemInterval.h"

using namespace NInterval;

// Define export macros for cross-platform compatibility
#if defined(_WIN32) || defined(_WIN64)
#define FUNCTION_EXPORT __declspec(dllexport)
#else
#define FUNCTION_EXPORT __attribute__((visibility("default")))
#endif

// Required function signatures to be implemented by the user
extern "C" {
// Function to evaluate the i-th equation of the system
FUNCTION_EXPORT ValInterval evaluateFunction(int i, int n,
                                             const ValInterval *x);

// Function to evaluate the derivatives of the i-th equation
FUNCTION_EXPORT void evaluateDerivatives(int i, int n, const ValInterval *x,
                                         ValInterval *dfatx);

// Get a human-readable name for this system (optional)
FUNCTION_EXPORT const char *getName();

// Get the recommended number of equations for this system
FUNCTION_EXPORT int getNumberOfEquations();

// // Get recommended initial values (optional)
// FUNCTION_EXPORT void getRecommendedInitialGuess(double *initialGuess);
}
#endif  // __LIBRARYINTERFACE_INTERVAL_H__