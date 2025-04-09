#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "Solver.h"
#include <QFileDialog>
#include <QGroupBox>
#include <QLibrary>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <qspinbox.h>

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void loadLibrary();
  void runSolver();

private:
  std::unique_ptr<Solver> solver;
  QPushButton *runButton;
  QLabel *resultLabel;
  QGroupBox *inputsGroup;
  QVBoxLayout *inputsGroupLayout;
  QDoubleSpinBox *epsilonInput;
  QSpinBox *maxIterationsInput;
  void updateInterface();
  void clearInputs();
  void createInput(int i);
};
#endif // __MAINWINDOW_H__