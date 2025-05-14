#ifndef __SOLVERSTATUS_H__
#define __SOLVERSTATUS_H__

// Status codes returned by the solver
enum class SolverStatus {
  SUCCESS = 0,
  INVALID_INPUT = 1,
  SINGULAR_MATRIX = 2,
  MAX_ITERATIONS_EXCEEDED = 3,
  LIBRARY_ERROR = 4,
  FUNCTION_NOT_LOADED = 5
};
#endif  // __SOLVERSTATUS_H__