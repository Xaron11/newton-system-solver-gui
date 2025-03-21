#include "../include/MainWindow.h"
#include "../include/LibraryAPI.h"
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Plugin Loader");

  auto *loadAction = new QAction("Load Plugin", this);
  connect(loadAction, &QAction::triggered, this, &MainWindow::loadPlugin);

  QMenu *fileMenu = menuBar()->addMenu("File");
  fileMenu->addAction(loadAction);
}

void MainWindow::loadPlugin() {
  QString filePath = QFileDialog::getOpenFileName(this, "Select plugin DLL", "",
                                                  "Shared Libraries (*.so)");
  if (!filePath.isEmpty()) {
    callPlugin(filePath);
  }
}

void MainWindow::callPlugin(const QString &path) {
  QLibrary lib(path);
  if (!lib.load()) {
    QMessageBox::critical(this, "Error",
                          "Failed to load library: " + lib.errorString());
    return;
  }

  auto infoFunc = (GetLibraryInfoFunc)lib.resolve("GetLibraryInfo");
  auto execFunc = (EquationFunc)lib.resolve("Equation");

  if (infoFunc && execFunc) {
    LibraryInfo info = infoFunc();
    QMessageBox::information(this, "Library Loaded",
                             QString("Name: %1").arg(info.name));
    double v = execFunc(2.0);
    QMessageBox::information(this, "Result", QString("Result: %1").arg(v));

  } else {
    QMessageBox::critical(this, "Error",
                          "Missing functions in library. " + lib.errorString());
  }

  lib.unload();
}