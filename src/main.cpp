#include "../include/Interval.h"
#include "../include/MainWindow.h"
#include <QApplication>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  interval_arithmetic::Interval<double> x(1, 2);
  interval_arithmetic::Interval<double> y(3, 4);
  interval_arithmetic::Interval<double> z = x + y;
  std::string left, right;
  z.IEndsToStrings(left, right);
  std::cout << "[" << left << ";" << right << "]" << std::endl;
  QApplication app(argc, argv);
  MainWindow window;
  window.show();
  return app.exec();
}