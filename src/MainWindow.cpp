#include "../include/MainWindow.h"
#include <QFileDialog>
#include <QMenuBar>
#include <QPushButton>

#include "../include/Solver.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Newton Solver");

  auto *loadAction = new QAction("Load Library", this);
  connect(loadAction, &QAction::triggered, this, &MainWindow::loadLibrary);

  QMenu *fileMenu = menuBar()->addMenu("File");
  fileMenu->addAction(loadAction);

  auto *button = new QPushButton("Run Solver", this);
  button->setGeometry(50, 50, 100, 30);
  connect(button, &QPushButton::clicked, this, &MainWindow::runSolver);

  solver = std::make_unique<Solver>();
}

void MainWindow::loadLibrary() {

  QString filePath = QFileDialog::getOpenFileName(
      this, "Select Library DLL", "", "Shared Libraries (*.so)");
  if (!filePath.isEmpty() && !solver->loadLibrary(filePath.toStdString())) {
    QMessageBox::critical(
        this, "Error",
        QString("Failed to load library: %1")
            .arg(QString::fromStdString(solver->getLastError())));
    return;
  }
  QMessageBox::information(
      this, "Library Loaded",
      QString("Name: %1")
          .arg(QString::fromStdString(solver->getLibraryName())));
}

void MainWindow::runSolver() {
  if (!solver->isReady()) {
    QMessageBox::critical(this, "Error", "Library not loaded");
    return;
  }

  Vector initialGuess = {0.0, 1.0, 0.0};
  int maxIterations = 100;
  double epsilon = 1E-6;

  SolverResult result = solver->solve(2, initialGuess, maxIterations, epsilon);
  switch (result.status) {
  case SolverStatus::SUCCESS:
    QMessageBox::information(this, "Result",
                             QString("Result: %1, %2, %3")
                                 .arg(result.solution[0])
                                 .arg(result.solution[1])
                                 .arg(result.solution[2]));
    break;
  case SolverStatus::INVALID_INPUT:
    QMessageBox::critical(this, "Error", "Invalid input");
    break;
  case SolverStatus::SINGULAR_MATRIX:
    QMessageBox::critical(this, "Error", "Singular matrix");
    break;
  case SolverStatus::MAX_ITERATIONS_EXCEEDED:
    QMessageBox::critical(this, "Error", "Max iterations exceeded");
    break;
  case SolverStatus::LIBRARY_ERROR:
    QMessageBox::critical(this, "Error", "Library error");
    break;
  case SolverStatus::FUNCTION_NOT_LOADED:
    QMessageBox::critical(this, "Error", "Functions not loaded");
    break;
  default:
    QMessageBox::critical(this, "Error", solver->getLastError().c_str());
    break;
  }
}
