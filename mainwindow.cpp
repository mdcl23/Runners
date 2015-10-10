#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(0)//new Ui::MainWindow)
{
    this->landscape = new LandscapeWidget();
    this->setCentralWidget(landscape);

    /*ui->setupUi(this);
    this->landscape = new LandscapeWidget();
    landscape->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    ui->centralWidget->layout()->addWidget(landscape);
    ui->centralWidget->layout()->setMargin(0);
    //ui->verticalLayout->setDirection();
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}
