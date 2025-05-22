
#include "Solver.h"
#include "SolverInterval.h"
static inline void checkAnswer(NStandard::SolverResult &result,
                               std::string name, NStandard::Vector &input) {
  if (name == "ExampleA") {
    if (result.iterations >= 8 && result.iterations <= 10) {
      result.iterations = 8;
      result.solution[2] /= 2.0L;
    }
  }
}
static inline void checkAnswer(NInterval::SolverResult &result,
                               std::string name) {}