
#ifndef SOLVER_H
#define SOLVER_H

#include "./NewtonSystem.h"
#include <string>

using GetNameFunc = const char *(*)();
using GetNumberOfEquationsFunc = int (*)();

// Status codes returned by the solver
enum class SolverStatus {
  SUCCESS = 0,
  INVALID_INPUT = 1,
  SINGULAR_MATRIX = 2,
  MAX_ITERATIONS_EXCEEDED = 3,
  LIBRARY_ERROR = 4,
  FUNCTION_NOT_LOADED = 5
};

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
  SolverResult solve(Vector &x, int maxIterations, double epsilon);

  // Get the last error message
  std::string getLastError() const;

  // Check if functions are loaded and ready to use
  bool isReady() const;

  std::string getLibraryName() const;

private:
  FunctionTypeC evaluateFunction;
  DerivativeTypeC evaluateDerivatives;
  GetNameFunc getName;
  GetNumberOfEquationsFunc getNumberOfEquations;
  bool functionsLoaded;
  std::string lastError;
};
#endif // SOLVER_H