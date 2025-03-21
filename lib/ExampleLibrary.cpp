#include "../include/LibraryAPI.h"

extern "C" {

LibraryInfo GetLibraryInfo() { return {"ExampleLibrary"}; }

double Equation(double value) { return value * value; }
}