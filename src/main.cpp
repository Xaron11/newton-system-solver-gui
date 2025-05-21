#include <QApplication>

// #include "../include/Interval.h"
#include "../include/MainWindow.h"
// #include "../include/NewtonSystem.h"

// using Extended = long double;

// Extended mainf(Extended x) {
//   Extended s = sin(x);
//   return s * (s + 0.5) - 0.5;
// }
// Extended maindf(Extended x) { return sin(2 * x) + 0.5 * cos(x); }

int main(int argc, char *argv[]) {
  // interval_arithmetic::Interval<double> x(1, 2);
  //   interval_arithmetic::Interval<double> y(3, 4);
  //   interval_arithmetic::Interval<double> z = x + y;
  //   std::string left, right;
  //   z.IEndsToStrings(left, right);
  //   std::cout << "[" << left << ";" << right << "]" << std::endl;
  // NewtonSystem(10, Vector &x, FunctionType f, DerivativeType df, int mit,
  // double eps, int &it, int &st)

  //   std::cout << "Size of long double: " << sizeof(long double) << "bytes\n
  //   "; std::cout << " Significant digits : "
  //             << std::numeric_limits<long double>::digits10 << "\n";
  //   std::cout << "Max value: " << std::numeric_limits<long double>::max() <<
  //   "\n";

  //   Extended x = 0.6L;
  //   std::cout << std::setprecision(24) << "x = " << x << std::endl;
  //   std::cout << mainf(x) << std::endl;
  //   std::cout << maindf(x) << std::endl;

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