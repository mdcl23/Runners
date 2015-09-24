#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->landscape = new LandscapeWidget();
    ui->verticalLayout->addWidget(landscape);
}

MainWindow::~MainWindow()
{
    delete ui;
}
