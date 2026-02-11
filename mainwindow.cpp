#include "mainwindow.h"


#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    _courseform=new courseform(this);
    _courseform->setAttribute(Qt::WA_DeleteOnClose);
    ui->stackedWidget->addWidget(_courseform);
    ui->stackedWidget->setCurrentWidget(_courseform);
    ui->bt_coursefile->setEnabled(true);
    ui->bt_course->setEnabled(false);
    ui->bt_notice->setEnabled(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_bt_course_clicked()//切换到课表窗口
{
    if(_widget_coursefile!=nullptr) _widget_coursefile->close();
    _courseform=new courseform(this);
    _courseform->setAttribute(Qt::WA_DeleteOnClose);
    ui->stackedWidget->addWidget(_courseform);
    ui->stackedWidget->setCurrentWidget(_courseform);
    ui->bt_coursefile->setEnabled(true);
    ui->bt_course->setEnabled(false);
    ui->bt_notice->setEnabled(true);
}


void MainWindow::on_bt_coursefile_clicked()//切换到课件窗口
{
    if(_courseform!=nullptr) {
        _courseform->close();
    }
    _widget_coursefile=new widget_coursefile(this);
    _widget_coursefile->setAttribute(Qt::WA_DeleteOnClose);
    ui->stackedWidget->addWidget( _widget_coursefile);
    ui->stackedWidget->setCurrentWidget( _widget_coursefile);
    ui->bt_coursefile->setEnabled(false);
    ui->bt_course->setEnabled(true);
    ui->bt_notice->setEnabled(true);
}

