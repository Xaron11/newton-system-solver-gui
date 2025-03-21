#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QFileDialog>
#include <QLibrary>
#include <QMainWindow>
#include <QMessageBox>

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);

private slots:
  void loadPlugin();

private:
  void callPlugin(const QString &path);
};
#endif // __MAINWINDOW_H__