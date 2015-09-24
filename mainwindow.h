#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "landscapewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LandscapeWidget* landscape;
};

#endif // MAINWINDOW_H
