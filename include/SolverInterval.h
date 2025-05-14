#ifndef SOLVER_INTERVAL_H
#define SOLVER_INTERVAL_H

#include <string>

#include "./NewtonSystemInterval.h"
#include "SolverStatus.h"

namespace NInterval {

using GetNameFunc = const char *(*)();
using GetNumberOfEquationsFunc = int (*)();

struct SolverResult {
  SolverStatus status;
  int iterations;
  Vector solution;
  std::string errorMessage;
};

class Solver {
 public:
  Solver();

  // Load user-defined functions from a shared library
  bool loadLibrary(std::string libraryPath);

  // Solve the system with the loaded functions
  SolverResult solve(Vector &x, int maxIterations, ValInterval epsilon);

  // Get the last error message
  std::string getLastError() const;

  // Check if functions are loaded and ready to use
  bool isReady() const;

  std::string getLibraryName() const;

  int getEquationsCount() const;

 private:
  FunctionTypeC evaluateFunction;
  DerivativeTypeC evaluateDerivatives;
  GetNameFunc getName;
  GetNumberOfEquationsFunc getNumberOfEquations;
  bool functionsLoaded;
  std::string lastError;
};
}  // namespace NInterval
#endif  // SOLVER_INTERVAL_H