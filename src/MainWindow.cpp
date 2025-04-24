#include "../include/MainWindow.h"

#include <qmessagebox.h>

#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <ios>

#include "../include/Solver.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  QWidget *central = new QWidget(this);
  QVBoxLayout *mainLayout = new QVBoxLayout(central);
  setWindowTitle("Newton Solver");
  auto *loadAction = new QAction("Load Library", this);
  connect(loadAction, &QAction::triggered, this, &MainWindow::loadLibrary);

  QMenu *fileMenu = menuBar()->addMenu("File");
  fileMenu->addAction(loadAction);

  mainLayout->setContentsMargins(50, 50, 50, 50);
  mainLayout->setSpacing(20);

  inputsGroup = new QGroupBox("Inputs", this);
  inputsGroupLayout = new QVBoxLayout();
  inputsGroup->setLayout(inputsGroupLayout);
  mainLayout->addWidget(inputsGroup);

  QLabel *epsilonLabel = new QLabel("Epsilon: ", this);
  epsilonInput = new QDoubleSpinBox(this);
  epsilonInput->setRange(0.0, 1E20);
  epsilonInput->setDecimals(20);
  epsilonInput->setSingleStep(1E-16);
  epsilonInput->setValue(1E-16);
  QHBoxLayout *epsilonLayout = new QHBoxLayout();
  epsilonLayout->addWidget(epsilonLabel);
  epsilonLayout->addWidget(epsilonInput);
  mainLayout->addLayout(epsilonLayout);

  QLabel *maxIterationsLabel = new QLabel("Max Iterations: ", this);
  maxIterationsInput = new QSpinBox(this);
  maxIterationsInput->setRange(1, 1000);
  maxIterationsInput->setValue(10);
  QHBoxLayout *maxIterationsLayout = new QHBoxLayout();
  maxIterationsLayout->addWidget(maxIterationsLabel);
  maxIterationsLayout->addWidget(maxIterationsInput);
  mainLayout->addLayout(maxIterationsLayout);

  runButton = new QPushButton("Run Solver", this);
  mainLayout->addWidget(runButton);
  connect(runButton, &QPushButton::clicked, this, &MainWindow::runSolver);

  resultLabel = new QLabel("Result: ", this);
  mainLayout->addWidget(resultLabel);

  setCentralWidget(central);

  solver = std::make_unique<Solver>();
  updateInterface();
}

MainWindow::~MainWindow() {
  delete runButton;
  delete resultLabel;
  delete inputsGroupLayout;
  delete inputsGroup;
}

void MainWindow::loadLibrary() {
  QString filePath = QFileDialog::getOpenFileName(
      this, "Select Library DLL", "", "Shared Libraries (*.so)");

  if (filePath.isEmpty()) {
    QMessageBox::critical(this, "Error", QString("No library selected"));
    return;
  }
  if (!solver->loadLibrary(filePath.toStdString())) {
    QMessageBox::critical(
        this, "Error",
        QString("Failed to load library: %1")
            .arg(QString::fromStdString(solver->getLastError())));
    return;
  }

  updateInterface();

  QMessageBox::information(
      this, "Library Loaded",
      QString("Name: %1")
          .arg(QString::fromStdString(solver->getLibraryName())));
}

void MainWindow::updateInterface() {
  clearInputs();
  resultLabel->clear();
  if (solver->isReady()) {
    runButton->setEnabled(true);
    int n = solver->getEquationsCount();
    for (int i = 1; i <= n; ++i) {
      createInput(i);
    }
  } else {
    runButton->setEnabled(false);
  }
}

void MainWindow::createInput(int i) {
  auto *label = new QLabel("x[" + QString::number(i) + "] = ", this);

  auto *spinBox = new QDoubleSpinBox(this);
  spinBox->setRange(-1E5, 1E5);
  spinBox->setDecimals(5);
  spinBox->setSingleStep(0.01);
  spinBox->setValue(0.0);

  auto *hLayout = new QHBoxLayout();
  hLayout->addWidget(label);
  hLayout->addWidget(spinBox);

  inputsGroupLayout->addLayout(hLayout);
}

void MainWindow::clearInputs() {
  QLayoutItem *item;
  while ((item = inputsGroupLayout->takeAt(0)) != nullptr) {
    while (QLayoutItem *childItem = item->layout()->takeAt(0)) {
      delete childItem->widget();
      delete childItem;
    }
  }
}

void MainWindow::showResult(SolverResult &result) {
  QString resultText = "Result:\n";
  for (size_t i = 1; i < result.solution.size(); ++i) {
    std::ostringstream resultStr;
    resultStr << std::scientific
              << std::uppercase
              // << std::setprecision(18);
              << std::setprecision(std::numeric_limits<long double>::digits10 +
                                   1)
              << "x[" << i << "] = " << result.solution[i] << "\n";
    resultText += QString::fromStdString(resultStr.str());
  }
  resultText += QString("Iterations: %1").arg(result.iterations);
  resultLabel->setText(resultText);
}

void MainWindow::runSolver() {
  if (!solver->isReady()) {
    QMessageBox::critical(this, "Error", "Library not loaded");
    return;
  }

  // Example initial guess and parameters for quadratic system
  //   Vector initialGuess = {0.0, 1.0, 0.0};
  //   int maxIterations = 100;
  //   double epsilon = 1E-6;
  // Example initial guess and parameters for 3-equation system
  //   Vector initialGuess = {0.0, 0.1, 0.1, -0.1};
  //   int maxIterations = 100;
  //   double epsilon = 1E-16;
  Vector initialGuess(solver->getEquationsCount() + 1, 0.0);
  for (int i = 0; i < solver->getEquationsCount(); ++i) {
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox *>(
        inputsGroupLayout->itemAt(i)->layout()->itemAt(1)->widget());
    if (spinBox) {
      initialGuess[i + 1] = spinBox->value();
    }
  }
  int maxIterations = maxIterationsInput->value();
  double epsilon = epsilonInput->value();

  SolverResult result = solver->solve(initialGuess, maxIterations, epsilon);
  QString resultText = "Result:\n";
  switch (result.status) {
    case SolverStatus::SUCCESS:
      QMessageBox::information(this, "Success",
                               "Equations solved successfully");
      showResult(result);
      break;
    case SolverStatus::INVALID_INPUT:
      QMessageBox::critical(this, "Error", "Invalid input");
      break;
    case SolverStatus::SINGULAR_MATRIX:
      QMessageBox::critical(this, "Error", "Singular matrix");
      break;
    case SolverStatus::MAX_ITERATIONS_EXCEEDED:
      QMessageBox::warning(this, "Warning", "Max iterations exceeded");
      showResult(result);
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
