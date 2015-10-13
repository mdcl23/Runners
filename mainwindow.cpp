#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QBoxLayout>
#include <QGraphicsWidget>
#include "hexscene.h"

#include "gamescene.h"
#include "tetrixwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(0)//new Ui::MainWindow)
{
    //QGraphicsView* gw = new QGraphicsView();
    //gw->setScene(new HexScene(10));
    //gw->setMouseTracking(true);
    //this->setCentralWidget(gw);
    this->landscape = new LandscapeWidget();
    this->setCentralWidget(landscape);

    /*ui->setupUi(this);
    this->landscape = new LandscapeWidget();
    landscape->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    ui->centralWidget->layout()->addWidget(landscape);
    ui->centralWidget->layout()->setMargin(0);
    //ui->verticalLayout->setDirection();
    */

    connect(landscape->scene, SIGNAL(playerCollidesWithPortal()), this, SLOT(startTetrix()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startTetrix()
{
    this->setCentralWidget(new TetrixWindow());
}
