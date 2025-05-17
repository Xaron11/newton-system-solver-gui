#include "../include/MainWindow.h"

#include <qmessagebox.h>

#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

#include "../include/Solver.h"
#include "../include/SolverInterval.h"

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

  auto modeGroup = new QGroupBox("Arithmetic Mode", this);
  QVBoxLayout *modeLayout = new QVBoxLayout();
  modeGroup->setLayout(modeLayout);
  mainLayout->addWidget(modeGroup);

  QRadioButton *radio1 = new QRadioButton("Standard");
  QRadioButton *radio2 = new QRadioButton("Interval (with Standard input)");
  QRadioButton *radio3 = new QRadioButton("Interval (with Interval input)");

  radio1->setChecked(true);

  modeLayout->addWidget(radio1);
  modeLayout->addWidget(radio2);
  modeLayout->addWidget(radio3);
  connect(radio1, &QRadioButton::toggled, [=](bool checked) {
    if (radio1->isChecked()) {
      arithmeticMode = ArithmeticMode::STANDARD;
      updateInterface();
    }
  });
  connect(radio2, &QRadioButton::toggled, [=](bool checked) {
    if (radio2->isChecked()) {
      arithmeticMode = ArithmeticMode::INTERVAL;
      updateInterface();
    }
  });
  connect(radio3, &QRadioButton::toggled, [=](bool checked) {
    if (radio3->isChecked()) {
      arithmeticMode = ArithmeticMode::INTERVAL_INPUT;
      updateInterface();
    }
  });

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

  standardSolver = std::make_unique<NStandard::Solver>();
  intervalSolver = std::make_unique<NInterval::Solver>();
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
  switch (arithmeticMode) {
    case ArithmeticMode::STANDARD:
      if (!standardSolver->loadLibrary(filePath.toStdString())) {
        QMessageBox::critical(
            this, "Error",
            QString("Failed to load library: %1")
                .arg(QString::fromStdString(standardSolver->getLastError())));
        return;
      }
      QMessageBox::information(
          this, "Library Loaded",
          QString("Name: %1")
              .arg(QString::fromStdString(standardSolver->getLibraryName())));
      break;
    case ArithmeticMode::INTERVAL:
    case ArithmeticMode::INTERVAL_INPUT:
      if (!intervalSolver->loadLibrary(filePath.toStdString())) {
        QMessageBox::critical(
            this, "Error",
            QString("Failed to load library: %1")
                .arg(QString::fromStdString(intervalSolver->getLastError())));
        return;
      }
      QMessageBox::information(
          this, "Library Loaded",
          QString("Name: %1")
              .arg(QString::fromStdString(intervalSolver->getLibraryName())));
      break;
  }

  updateInterface();
}

void MainWindow::updateInterface() {
  clearInputs();
  resultLabel->clear();
  switch (arithmeticMode) {
    case ArithmeticMode::STANDARD:
      if (standardSolver->isReady()) {
        runButton->setEnabled(true);
        int n = standardSolver->getEquationsCount();
        for (int i = 1; i <= n; ++i) {
          createInput(i);
        }
      } else {
        runButton->setEnabled(false);
      }
      break;
    case ArithmeticMode::INTERVAL:
    case ArithmeticMode::INTERVAL_INPUT:
      if (intervalSolver->isReady()) {
        runButton->setEnabled(true);
        int n = intervalSolver->getEquationsCount();
        for (int i = 1; i <= n; ++i) {
          createInput(i);
        }
      } else {
        runButton->setEnabled(false);
      }
      break;
  }
}

void MainWindow::createInput(int i) {
  auto *label = new QLabel("x[" + QString::number(i) + "] = ", this);

  auto *hLayout = new QHBoxLayout();
  hLayout->addWidget(label);
  switch (arithmeticMode) {
    case ArithmeticMode::STANDARD:
    case ArithmeticMode::INTERVAL: {
      auto *spinBox = new QDoubleSpinBox(this);
      spinBox->setRange(-1E5, 1E5);
      spinBox->setDecimals(5);
      spinBox->setSingleStep(0.1);
      spinBox->setValue(0.0);
      hLayout->addWidget(spinBox);
      break;
    }
    case ArithmeticMode::INTERVAL_INPUT: {
      auto *spinBoxStart = new QDoubleSpinBox(this);
      spinBoxStart->setRange(-1E5, 1E5);
      spinBoxStart->setDecimals(5);
      spinBoxStart->setSingleStep(0.1);
      spinBoxStart->setValue(0.0);
      hLayout->addWidget(spinBoxStart);
      auto *spinBoxEnd = new QDoubleSpinBox(this);
      spinBoxEnd->setRange(-1E5, 1E5);
      spinBoxEnd->setDecimals(5);
      spinBoxEnd->setSingleStep(0.1);
      spinBoxEnd->setValue(0.0);
      hLayout->addWidget(spinBoxEnd);
      break;
    }
  }

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

void MainWindow::showResult(NStandard::SolverResult &result) {
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

void MainWindow::showResult(NInterval::SolverResult &result) {
  QString resultText = "Result:\n";
  std::ostringstream resultStr;
  for (size_t i = 1; i < result.solution.size(); ++i) {
    resultStr.str("");
    resultStr.clear();
    std::string left, right;
    result.solution[i].IEndsToStrings(left, right);
    //   std::cout <<  << std::endl;
    resultStr << std::scientific
              << std::uppercase
              // << std::setprecision(18);
              << std::setprecision(std::numeric_limits<long double>::digits10 +
                                   1)
              << "x[" << i << "] = " << "[" << left << ";" << right << "]"
              << "\n";
    resultText += QString::fromStdString(resultStr.str());
  }
  resultStr.str("");
  resultStr.clear();
  resultStr << std::scientific
            << std::uppercase
            // << std::setprecision(18);
            << std::setprecision(std::numeric_limits<long double>::digits10 + 1)
            << "Width: " << result.solution[0].GetWidth() << "\n";

  resultText += QString("Iterations: %1\n").arg(result.iterations);
  resultText += QString::fromStdString(resultStr.str());
  resultLabel->setText(resultText);
}

void MainWindow::checkResultStatus(SolverStatus status) {
  switch (status) {
    case SolverStatus::SUCCESS:
      QMessageBox::information(this, "Success",
                               "Equations solved successfully");
      break;
    case SolverStatus::INVALID_INPUT:
      QMessageBox::critical(this, "Error", "Invalid input");
      break;
    case SolverStatus::SINGULAR_MATRIX:
      QMessageBox::critical(this, "Error", "Singular matrix");
      break;
    case SolverStatus::MAX_ITERATIONS_EXCEEDED:
      QMessageBox::warning(this, "Warning", "Max iterations exceeded");
      break;
    case SolverStatus::LIBRARY_ERROR:
      QMessageBox::critical(this, "Error", "Library error");
    case SolverStatus::FUNCTION_NOT_LOADED:
      QMessageBox::critical(this, "Error", "Functions not loaded");
      break;
    default:
      break;
  }
}

void MainWindow::runStandardSolver() {
  if (!standardSolver->isReady()) {
    QMessageBox::critical(this, "Error", "Library not loaded");
    return;
  }

  NStandard::Vector initialGuess(standardSolver->getEquationsCount() + 1, 0.0);
  for (int i = 0; i < standardSolver->getEquationsCount(); ++i) {
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox *>(
        inputsGroupLayout->itemAt(i)->layout()->itemAt(1)->widget());
    if (spinBox) {
      initialGuess[i + 1] = spinBox->value();
    }
  }
  int maxIterations = maxIterationsInput->value();
  NStandard::Val epsilon = epsilonInput->value();
  NStandard::SolverResult result =
      standardSolver->solve(initialGuess, maxIterations, epsilon);
  checkResultStatus(result.status);
  showResult(result);
}

void MainWindow::runIntervalSolver() {
  if (!intervalSolver->isReady()) {
    QMessageBox::critical(this, "Error", "Library not loaded");
    return;
  }

  NInterval::Vector initialGuess(intervalSolver->getEquationsCount() + 1,
                                 NInterval::ValInterval(0.0, 0.0));
  for (int i = 0; i < intervalSolver->getEquationsCount(); ++i) {
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox *>(
        inputsGroupLayout->itemAt(i)->layout()->itemAt(1)->widget());
    if (spinBox) {
      initialGuess[i + 1] =
          NInterval::ValInterval(spinBox->value(), spinBox->value());
    }
  }
  int maxIterations = maxIterationsInput->value();
  NInterval::ValInterval epsilon =
      NInterval::ValInterval(epsilonInput->value(), epsilonInput->value());
  NInterval::SolverResult result =
      intervalSolver->solve(initialGuess, maxIterations, epsilon);
  checkResultStatus(result.status);
  showResult(result);
}

void MainWindow::runIntervalInputSolver() {
  if (!intervalSolver->isReady()) {
    QMessageBox::critical(this, "Error", "Library not loaded");
    return;
  }

  if (!intervalSolver->isReady()) {
    QMessageBox::critical(this, "Error", "Library not loaded");
    return;
  }

  NInterval::Vector initialGuess(intervalSolver->getEquationsCount() + 1,
                                 NInterval::ValInterval(0.0, 0.0));
  for (int i = 0; i < intervalSolver->getEquationsCount(); ++i) {
    QDoubleSpinBox *spinBoxStart = qobject_cast<QDoubleSpinBox *>(
        inputsGroupLayout->itemAt(i)->layout()->itemAt(1)->widget());
    QDoubleSpinBox *spinBoxEnd = qobject_cast<QDoubleSpinBox *>(
        inputsGroupLayout->itemAt(i)->layout()->itemAt(2)->widget());
    if (spinBoxStart && spinBoxEnd) {
      initialGuess[i + 1] =
          NInterval::ValInterval(spinBoxStart->value(), spinBoxEnd->value());
    }
  }
  int maxIterations = maxIterationsInput->value();
  NInterval::ValInterval epsilon =
      NInterval::ValInterval(epsilonInput->value(), epsilonInput->value());
  NInterval::SolverResult result =
      intervalSolver->solve(initialGuess, maxIterations, epsilon);
  checkResultStatus(result.status);
  showResult(result);
}

void MainWindow::runSolver() {
  // Example initial guess and parameters for quadratic system
  //   Vector initialGuess = {0.0, 1.0, 0.0};
  //   int maxIterations = 100;
  //   double epsilon = 1E-6;
  // Example initial guess and parameters for 3-equation system
  //   Vector initialGuess = {0.0, 0.1, 0.1, -0.1};
  //   int maxIterations = 100;
  //   double epsilon = 1E-16;

  switch (arithmeticMode) {
    case ArithmeticMode::STANDARD:
      runStandardSolver();
      break;
    case ArithmeticMode::INTERVAL:
      runIntervalSolver();
      break;
    case ArithmeticMode::INTERVAL_INPUT:
      runIntervalInputSolver();
      break;
  }
}
