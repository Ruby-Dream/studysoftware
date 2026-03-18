#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    t=new QSystemTrayIcon(this);
    t->setIcon(QIcon("D:/desktop/图片/好康的/4.jpg"));
    t->setToolTip("h");
    t->show();

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


    _courseform=new courseform(db,sqlmodel,sqlmodel2,sqlmodel3,this);
    ui->stackedWidget->addWidget(_courseform);
    ui->stackedWidget->setCurrentWidget(_courseform);
    ui->bt_coursefile->setEnabled(true);
    ui->bt_course->setEnabled(false);
    ui->bt_notice->setEnabled(true);
    currentwidget=1;

    personal_timer=new QTimer(nullptr);
    personal_timer->setSingleShot(true);
    personal_timer->setTimerType(Qt::PreciseTimer);
    personal_timer->stop();
    connect(personal_timer,&QTimer::timeout,this,&MainWindow::do_singleshot_timeout_personal);

    course_timer=new QTimer(nullptr);
    course_timer->setSingleShot(true);
    course_timer->setTimerType(Qt::PreciseTimer);
    course_timer->stop();
    // QDate now=QDate::currentDate();
    // today_week=day.daysTo(now)/7+1;//这个是启动软件当天时是第几周

    qrymodel=new QSqlQueryModel(nullptr);
    update_personal();//首次启动时运行

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_bt_course_clicked()//切换到课表窗口
{
    delete_old_widget();
    currentwidget=1;
    _courseform=new courseform(db,sqlmodel,sqlmodel2,sqlmodel3,this);
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
    _widget_notice=new widget_notice(db,this);
    ui->stackedWidget->addWidget(_widget_notice);
    ui->stackedWidget->setCurrentWidget(_widget_notice);
    ui->bt_coursefile->setEnabled(true);
    ui->bt_course->setEnabled(true);
    ui->bt_notice->setEnabled(false);
    //connect(_widget_notice,&widget_notice::refresh,this,&MainWindow::setsingleshot_personal);
    connect(_widget_notice->time_delegate,&TimeDelegate::fresh,this,&MainWindow::setsingleshot_personal);
    connect(_widget_notice->date_delegate,&DateDelegate::fresh,this,&MainWindow::setsingleshot_personal);
    connect(_widget_notice->lineedit_delegate2,&LineeditDelegate::fresh,this,&MainWindow::setsingleshot_personal);
}

void MainWindow::do_singleshot_timeout_personal()
{
    t->showMessage("来自软件的通知",personal_notice);
    update();
    setsingleshot_personal();
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

void MainWindow::setsingleshot_personal()//设定下一次事务通知
{
    personal_timer->stop();//停止上一次的
    QString query="SELECT * from notice_personal where date = \""+date+"\" ORDER BY time" ;
    qrymodel->setQuery(query,db);
    if(qrymodel->rowCount()==0) return;//如果今天没有事项 今天就不再运行

    for(int i=0;i<qrymodel->rowCount();i++){
        QTime t=qrymodel->record(i).value("time").toTime();
        QTime now=QTime::currentTime();
        if(now.msecsTo(t)>0){//找到那个最近的时间
            personal_notice=qrymodel->record(i).value("text").toString();
            personal_timer->setInterval(1000+now.msecsTo(t));
            personal_timer->start();
            break;
        }
    }
}

void MainWindow::setsingleshot_course()
{
    course_timer->stop();
    QString query="SELECT * from course where workday_on = \"" + workday+ "\""  ;
}

void MainWindow::update_personal()//每天首次启动或午夜十二点时更新此时日期
{
    date=QDate::currentDate().toString("yyyy-MM-dd");
    switch (QDate::currentDate().dayOfWeek()){
    case 1: workday="周一";
        break;
    case 2: workday="周二";
        break;
    case 3: workday="周三";
        break;
    case 4: workday="周四";
        break;
    case 5: workday="周五";
        break;
    case 6: workday="周六";
        break;
    case 7: workday="周日";
        break;
    }
    QTimer::singleShot(86401000-QTime::currentTime().msec(),this,&MainWindow::update_personal);//下一次更新日期是在过午夜12点的时候
    setsingleshot_personal();//每天要执行一次检查是否今天有提醒事项
}

void MainWindow::update_course()
{

}

