#include <QApplication>
#include <iostream>
#include <string>

#include "../include/Interval.h"
#include "../include/MainWindow.h"
#include "../include/NewtonSystem.h"

int main(int argc, char *argv[]) {
  //   interval_arithmetic::Interval<double> x(1, 2);
  //   interval_arithmetic::Interval<double> y(3, 4);
  //   interval_arithmetic::Interval<double> z = x + y;
  //   std::string left, right;
  //   z.IEndsToStrings(left, right);
  //   std::cout << "[" << left << ";" << right << "]" << std::endl;
  // NewtonSystem(10, Vector &x, FunctionType f, DerivativeType df, int mit,
  // double eps, int &it, int &st)

  //   std::cout << "Size of long double: " << sizeof(long double) << "
  //   bytes\n"; std::cout << "Significant digits: "
  //             << std::numeric_limits<long double>::digits10 << "\n";
  //   std::cout << "Max value: " << std::numeric_limits<long double>::max() <<
  //   "\n";

  QApplication app(argc, argv);
  MainWindow window;
  window.show();
  return app.exec();
}

// int main() {
//     // Example usage of NewtonSystem
//     // NewtonSystem(10, x, evaluateFunction, evaluateDerivatives, 100, 1e-9L,
//     iterations, status); return 0;
// }