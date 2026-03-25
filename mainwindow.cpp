#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    t=new QSystemTrayIcon(this);
    t->setIcon(QIcon("D:/desktop/图片/好康的/4.jpg"));
    t->setToolTip("h");
    QMenu *m = new QMenu;
    m->addAction("显示", this, &QWidget::showNormal);
    m->addSeparator();
    m->addAction("退出", qApp, &QCoreApplication::quit);
    t->setContextMenu(m);
    t->show();


    ui->setupUi(this);
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    right=new QLabel(nullptr);
    left=new QLabel(nullptr);
    ui->statusbar->addPermanentWidget(right);
    ui->statusbar->addWidget(left);

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
    connect(_courseform,&courseform::status,this,&MainWindow::do_status);
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


    course_timer=new QTimer(nullptr);
    course_timer->setSingleShot(true);
    course_timer->setTimerType(Qt::PreciseTimer);
    course_timer->stop();

    qrymodel=new QSqlQueryModel(nullptr);
    qrymodel2=new QSqlQueryModel(nullptr);
    connect(personal_timer,&QTimer::timeout,this,&MainWindow::do_singleshot_timeout_personal);
    connect(course_timer,&QTimer::timeout,this,&MainWindow::do_singleshot_timeout_course);
    update_personal();//首次启动时运行
    update_course();

    ui->bt_course->installEventFilter(this);
    ui->bt_coursefile->installEventFilter(this);
    ui->bt_notice->installEventFilter(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_status(QString s,int LorR)//接收其他窗口的槽函数，修改左右下方状态栏小字显示，-1为左，1为右
{
    if(LorR==-1){
        left->setText(s);
    }
    else right->setText(s);
}


void MainWindow::on_bt_course_clicked()//切换到课表窗口
{
    left->clear();
    delete_old_widget();
    currentwidget=1;
    _courseform=new courseform(db,sqlmodel,sqlmodel2,sqlmodel3,this);
    ui->stackedWidget->addWidget(_courseform);
    ui->stackedWidget->setCurrentWidget(_courseform);
    ui->bt_coursefile->setEnabled(true);
    ui->bt_course->setEnabled(false);
    ui->bt_notice->setEnabled(true);
    connect(_courseform,&courseform::status,this,&MainWindow::do_status);
}


void MainWindow::on_bt_coursefile_clicked()//切换到课件窗口
{
    left->clear();
    delete_old_widget();
    currentwidget=2;
    _widget_coursefile=new widget_coursefile(db,this);
    ui->stackedWidget->addWidget( _widget_coursefile);
    ui->stackedWidget->setCurrentWidget( _widget_coursefile);
    ui->bt_coursefile->setEnabled(false);
    ui->bt_course->setEnabled(true);
    ui->bt_notice->setEnabled(true);
    connect(_widget_coursefile,&widget_coursefile::status,this,&MainWindow::do_status);
}


void MainWindow::on_bt_notice_clicked()//切换到通知窗口
{
    left->clear();
    delete_old_widget();
    currentwidget=3;
    _widget_notice=new widget_notice(db,this);
    ui->stackedWidget->addWidget(_widget_notice);
    ui->stackedWidget->setCurrentWidget(_widget_notice);
    ui->bt_coursefile->setEnabled(true);
    ui->bt_course->setEnabled(true);
    ui->bt_notice->setEnabled(false);

    connect(_widget_notice->lineedit_delegate1,&LineeditDelegate::fresh_course,this,&MainWindow::setsingleshot_course);
    connect(_widget_notice->spin_delegate,&SpinDelegate::fresh,this,&MainWindow::setsingleshot_course);
    connect(_widget_notice->time_delegate,&TimeDelegate::fresh,this,&MainWindow::setsingleshot_personal);
    connect(_widget_notice->date_delegate,&DateDelegate::fresh,this,&MainWindow::setsingleshot_personal);
    connect(_widget_notice->lineedit_delegate2,&LineeditDelegate::fresh_personal,this,&MainWindow::setsingleshot_personal);
}

void MainWindow::do_singleshot_timeout_personal()
{
    t->showMessage("来自软件的通知",personal_notice);
    setsingleshot_personal();
}

void MainWindow::do_singleshot_timeout_course()
{
    t->showMessage("即将上课",course_notice);
    setsingleshot_course();
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

void MainWindow::setsingleshot_course()//设定下一次课程提醒
{
    course_timer->stop();
    //找出那些周几和今天一样，且上课周范围包含这周的条目
    QString query="SELECT * from course where workday_on = \"" + workday+"\""
                    +" and weekfrom <="+ QString::asprintf("%d",currentweek)+" and weekto >="+QString::asprintf("%d",currentweek)
                    +" ORDER BY begin_at";
    qrymodel2->setQuery(query,db);
    if(qrymodel2->rowCount()==0) return;
    for(int i=0;i<qrymodel2->rowCount();i++){
        if(qrymodel2->record(i).value("notice_before_min").toInt()==-1) continue;//如果是-1，不提醒这节课
        int begin=qrymodel2->record(i).value("begin_at").toInt();//上课节
        QTime t=sqlmodel2->record(begin-1).value("begin_at").toTime().addSecs(-60*(qrymodel2->record(i).value("notice_before_min").toInt()));//应该提醒的时间
        QTime now=QTime::currentTime();//当前时间
        if(now.msecsTo(t)>0){
            course_notice="现在是"+QTime::currentTime().toString("HH:mm")+'\n'
                            +qrymodel2->record(i).value("name").toString()+"即将上课\n"
                            +"上课时间："+qrymodel2->record(i).value("notice_before_min").toString()+"分钟后\n"
                            +"备注:"+qrymodel2->record(i).value("notice_text").toString();
            //int n=1000+now.msecsTo(t);
            course_timer->setInterval(1000+now.msecsTo(t));
            course_timer->start();
            break;
        }
    }
}
//更新软件运行这时候的日期（与用户无关）
//什么时候需要更新：第一次启动时√，过午夜12点时√,事务通知发生修改时√
void MainWindow::update_personal()
{
    date=QDate::currentDate().toString("yyyy-MM-dd");//更新当天日期
    QTimer::singleShot(86401000-QTime::currentTime().msec(),this,&MainWindow::update_personal);//下一次更新日期是在过午夜12点的时候
    setsingleshot_personal();//每天要执行一次检查是否今天有提醒事项
}
//更新软件运行这时候是星期几（与用户有关）
//什么时候需要更新：第一次启动时√，过午夜12点时√，修改学期第一天时√，修改课程开始节时，修改课程前多少分钟提醒时，修改提醒备注时√√
void MainWindow::update_course()
{   switch (QDate::currentDate().dayOfWeek()){//更新当天是周几
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
    QSqlRecord rec=sqlmodel->record(0);
    currentweek=rec.value("firstday").toDate().daysTo(QDate::currentDate())/7+1;//更新软件当天时是第几周
    QTimer::singleShot(86401000-QTime::currentTime().msec(),this,&MainWindow::update_course);//下一次更新日期是在过午夜12点的时候
    setsingleshot_course();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->bt_course && event->type()==QEvent::Enter){
        right->setText("查看并管理课程信息");
    }
    else if(watched==ui->bt_coursefile && event->type()==QEvent::Enter){
        right->setText("查看并管理本地课件");
    }
    else if(watched==ui->bt_notice && event->type()==QEvent::Enter){
        right->setText("查看并管理课程提醒及事务通知");
    }
    else if(event->type()==QEvent::Leave){
        right->clear();
    }
    return QWidget::eventFilter(watched,event);
}

