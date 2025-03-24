#include "../include/Solver.h"
#include "../include/NewtonSystem.h"
#include <QLibrary>
#include <QMessageBox>
#include <iostream>
#include <string>

Solver::Solver() : functionsLoaded(false) {}

bool Solver::loadLibrary(std::string libraryPath) {
  QLibrary lib(libraryPath.c_str());
  if (!lib.load()) {
    lastError = lib.errorString().toStdString();
    return false;
  }

  evaluateFunction = (FunctionTypeC)lib.resolve("evaluateFunction");
  evaluateDerivatives = (DerivativeTypeC)lib.resolve("evaluateDerivatives");
  getName = (GetNameFunc)lib.resolve("getName");
  getNumberOfEquations =
      (GetNumberOfEquationsFunc)lib.resolve("getNumberOfEquations");

  if (evaluateFunction && evaluateDerivatives && getName &&
      getNumberOfEquations) {
    functionsLoaded = true;
    return true;
  }
  lastError = "Missing functions in library";

  return false;
}

SolverResult Solver::solve(int numEquations, Vector &initialGuess,
                           int maxIterations, double epsilon) {
  if (!functionsLoaded) {
    return {SolverStatus::FUNCTION_NOT_LOADED, 0, {}, lastError};
  }

  int iterations = 0;
  int status = 0;

  NewtonSystem(numEquations, initialGuess, evaluateFunction,
               evaluateDerivatives, maxIterations, epsilon, iterations, status);

  switch (status) {
  case 0:
    return {SolverStatus::SUCCESS, iterations, initialGuess, ""};
  case 1:
    return {SolverStatus::INVALID_INPUT, iterations, initialGuess, ""};
  case 2:
    return {SolverStatus::SINGULAR_MATRIX, iterations, initialGuess, ""};
  case 3:
    return {SolverStatus::MAX_ITERATIONS_EXCEEDED, iterations, initialGuess,
            ""};
  default:
    return {SolverStatus::LIBRARY_ERROR, iterations, initialGuess, ""};
  }
}

std::string Solver::getLastError() const { return lastError; }

std::string Solver::getLibraryName() const {
  return functionsLoaded ? getName() : "No library";
}

bool Solver::isReady() const { return functionsLoaded; }
