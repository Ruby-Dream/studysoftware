#include "mainwindow.h"


#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);

    db=QSqlDatabase::addDatabase("QSQLITE");//所有数据库的连接，用哪个构造函数传哪个
    db.setDatabaseName("table.db");
    db.open();


    sqlmodel=new QSqlTableModel(nullptr,db);
    sqlmodel->setTable("classtableoption");
    sqlmodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sqlmodel->setSort(sqlmodel->fieldIndex("rowid"),Qt::AscendingOrder);
    if(!sqlmodel->select()){
        QMessageBox::critical(this,"bad","e");
    }

    sqlmodel2=new QSqlTableModel(nullptr,db);
    sqlmodel2->setTable("begin_and_end_at");
    sqlmodel2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sqlmodel2->setSort(sqlmodel2->fieldIndex("no"),Qt::AscendingOrder);
    if(!sqlmodel2->select()){
        QMessageBox::critical(this,"bad","e");
    }

    sqlmodel3=new QSqlTableModel(nullptr,db);
    sqlmodel3->setTable("course");
    sqlmodel3->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sqlmodel3->setSort(sqlmodel3->fieldIndex("no"),Qt::AscendingOrder);
    if(!sqlmodel3->select()){
        QMessageBox::critical(this,"bad","e");
    }


    _courseform=new courseform(sqlmodel,sqlmodel2,sqlmodel3,this);
    ui->stackedWidget->addWidget(_courseform);
    ui->stackedWidget->setCurrentWidget(_courseform);
    ui->bt_coursefile->setEnabled(true);
    ui->bt_course->setEnabled(false);
    ui->bt_notice->setEnabled(true);
    currentwidget=1;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_bt_course_clicked()//切换到课表窗口
{
    delete_old_widget();
    currentwidget=1;
    _courseform=new courseform(sqlmodel,sqlmodel2,sqlmodel3,this);
    ui->stackedWidget->addWidget(_courseform);
    ui->stackedWidget->setCurrentWidget(_courseform);
    ui->bt_coursefile->setEnabled(true);
    ui->bt_course->setEnabled(false);
    ui->bt_notice->setEnabled(true);
}


void MainWindow::on_bt_coursefile_clicked()//切换到课件窗口
{
    delete_old_widget();
    currentwidget=2;
    _widget_coursefile=new widget_coursefile(db,this);
    ui->stackedWidget->addWidget( _widget_coursefile);
    ui->stackedWidget->setCurrentWidget( _widget_coursefile);
    ui->bt_coursefile->setEnabled(false);
    ui->bt_course->setEnabled(true);
    ui->bt_notice->setEnabled(true);
}


void MainWindow::on_bt_notice_clicked()//切换到通知窗口
{
    delete_old_widget();
    currentwidget=3;
    _widget_notice=new widget_notice(this);
    ui->stackedWidget->addWidget(_widget_notice);
    ui->stackedWidget->setCurrentWidget(_widget_notice);
    ui->bt_coursefile->setEnabled(true);
    ui->bt_course->setEnabled(true);
    ui->bt_notice->setEnabled(false);
}

void MainWindow::delete_old_widget()//释放上一个页面的内存
{
    if(currentwidget==1){
        ui->stackedWidget->removeWidget(_courseform);
        delete _courseform;
        _courseform=nullptr;
    }
    else if(currentwidget==2){
        ui->stackedWidget->removeWidget(_widget_coursefile);
        delete _widget_coursefile;
        _widget_coursefile=nullptr;
    }
    else if(currentwidget==3){
        ui->stackedWidget->removeWidget(_widget_notice);
        delete _widget_notice;
        _widget_notice=nullptr;
    }
}

