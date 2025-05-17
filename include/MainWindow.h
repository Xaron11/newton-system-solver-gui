#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <qspinbox.h>

#include <QFileDialog>
#include <QGroupBox>
#include <QLibrary>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "Solver.h"
#include "SolverInterval.h"
#include "SolverStatus.h"

enum class ArithmeticMode {
  STANDARD = 0,
  INTERVAL = 1,
  INTERVAL_INPUT = 2,
};

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void loadLibrary();
  void runSolver();

 private:
  std::unique_ptr<NStandard::Solver> standardSolver;
  std::unique_ptr<NInterval::Solver> intervalSolver;
  ArithmeticMode arithmeticMode = ArithmeticMode::STANDARD;
  QPushButton *runButton;
  QLabel *resultLabel;
  QGroupBox *inputsGroup;
  QVBoxLayout *inputsGroupLayout;
  QDoubleSpinBox *epsilonInput;
  QSpinBox *maxIterationsInput;
  void updateInterface();
  void clearInputs();
  void createInput(int i);
  void showResult(NStandard::SolverResult &result);
  void showResult(NInterval::SolverResult &result);
  void checkResultStatus(SolverStatus status);
  void runStandardSolver();
  void runIntervalSolver();
  void runIntervalInputSolver();
};
#endif  // __MAINWINDOW_H__