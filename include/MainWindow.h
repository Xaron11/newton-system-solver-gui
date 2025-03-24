#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "Solver.h"
#include <QFileDialog>
#include <QLibrary>
#include <QMainWindow>
#include <QMessageBox>

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() = default;

private slots:
  void loadLibrary();
  void runSolver();

private:
  std::unique_ptr<Solver> solver;
};
#endif // __MAINWINDOW_H__