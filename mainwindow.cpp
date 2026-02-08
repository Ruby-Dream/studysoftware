#include "mainwindow.h"
#include "courseform.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    courseform *_courseform=new courseform(this);
    ui->stackedWidget->addWidget(_courseform);
    ui->stackedWidget->setCurrentWidget(_courseform);
    //ui->stackedWidget->setCurrentIndex(2);

}

MainWindow::~MainWindow()
{
    delete ui;
}
