#include "widget_course.h"
#include "ui_widget_course.h"


courseform::courseform(QSqlDatabase db,QSqlTableModel *sqlmodel,QSqlTableModel *sqlmodel2,QSqlTableModel *sqlmodel3,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::courseform)
{
    ui->setupUi(this);
    this->db=db;
    this->sqlmodel=sqlmodel;
    this->sqlmodel2=sqlmodel2;
    this->sqlmodel3=sqlmodel3;

    QSqlRecord rec=sqlmodel->record(0);
    column=rec.value("workdays").toInt();
    row=rec.value("classes").toInt();
    day=rec.value("firstday").toDate();
    mmodel=new QStandardItemModel(row,column,this);
    ui->tableView->setModel(mmodel);//课程表模型-视图
    loadtable();//加载课表结构框架

    QDate now=QDate::currentDate();
    today_week=day.daysTo(now)/7+1;//这个是启动软件当天时是第几周
    watching_week=today_week;//这个是正在看第几周的课表
    if(watching_week==1) ui->bt_upweek->setHidden(true);
    loadcourse();//加载数据库中的课程

    ui->bt_change->installEventFilter(this);
    ui->bt_tablesetting->installEventFilter(this);
    ui->bt_timesetting->installEventFilter(this);
    ui->bt_coursemanager->installEventFilter(this);
}

courseform::~courseform()
{
    delete ui;
}

bool courseform::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->bt_change && event->type()==QEvent::Enter){
        emit status("点击切换课表按时间/节显示",1);
    }
    else if(watched==ui->bt_tablesetting && event->type()==QEvent::Enter){
        emit status("设置开课第一天、一周显示几天、一天上几节课",1);
    }
    else if(watched==ui->bt_timesetting && event->type()==QEvent::Enter){
        emit status("设置每节课的上下课时间",1);
    }
    else if(watched==ui->bt_coursemanager && event->type()==QEvent::Enter){
        emit status("管理课程信息",1);
    }
    else if(event->type()==QEvent::Leave){
        emit status(" ",1);
    }
    return QWidget::eventFilter(watched,event);
}
void courseform::on_bt_change_clicked(bool checked)//切换课表显示为时间/节数
{
    if(checked){//变成时间
        QStringList classlist;
        for(int i=0;i<sqlmodel->record(0).value("classes").toInt();i++){
            QSqlRecord rec=sqlmodel2->record(i);
            classlist<<rec.value("begin_at").toString().mid(0,5)+'-'+rec.value("end_at").toString().mid(0,5);
        }
        mmodel->setVerticalHeaderLabels(classlist);
        ui->bt_change->setText("切换为节数");
    }
    else{//变成节数
        QStringList classlist;
        for(int i=0;i<sqlmodel->record(0).value("classes").toInt();i++){
            QSqlRecord rec=sqlmodel2->record(i);
            classlist<<QString::asprintf("第%d节\n",i+1);
        }
        mmodel->setVerticalHeaderLabels(classlist);
        ui->bt_change->setText("切换为时间");
    }
}

void courseform::loadtable()//设置课表横竖表头，以及长宽
{
    QStringList strlist;

    if(column==5){
        strlist<<"周一"<<"周二"<<"周三"<<"周四"<<"周五";
        ui->tableView->horizontalHeader()->setDefaultSectionSize(168);
    }
    else if(column==6){
        strlist<<"周一"<<"周二"<<"周三"<<"周四"<<"周五"<<"周六";
        ui->tableView->horizontalHeader()->setDefaultSectionSize(140);
    }
    else if(column==7){
        strlist<<"周一"<<"周二"<<"周三"<<"周四"<<"周五"<<"周六"<<"周日";
        ui->tableView->horizontalHeader()->setDefaultSectionSize(120);
    }
    mmodel->setHorizontalHeaderLabels(strlist);

    if(ui->bt_change->isChecked()){//变成时间
        QStringList classlist;
        for(int i=0;i<sqlmodel->record(0).value("classes").toInt();i++){
            QSqlRecord rec=sqlmodel2->record(i);
            classlist<<rec.value("begin_at").toString().mid(0,5)+'-'+rec.value("end_at").toString().mid(0,5);
        }
        mmodel->setVerticalHeaderLabels(classlist);
    }
    else{//变成节数
        QStringList classlist;
        for(int i=0;i<sqlmodel->record(0).value("classes").toInt();i++){
            QSqlRecord rec=sqlmodel2->record(i);
            classlist<<QString::asprintf("第%d节\n",i+1);
        }
        mmodel->setVerticalHeaderLabels(classlist);
    }
}

void courseform::enable()//恢复按钮可用性
{
    ui->bt_coursemanager->setChecked(false);
    ui->bt_coursemanager->setText("课程管理");
}

void courseform::loadcourse()//加载课程
{
    ui->labelweek->setText(QString::asprintf("第%d周",watching_week));
    for(int i=0;i<sqlmodel3->rowCount();i++){
        QSqlRecord rec=sqlmodel3->record(i);
        if(rec.value("weekfrom").toInt()>watching_week ||rec.value("weekto").toInt()<watching_week){//如果当前不在开课周和结课周之间，跳过
            continue;
        }

        int daynumber=-1;//课在周几
        QString day=rec.value("workday_on").toString();
        if(day=="周一") daynumber=0;
        else if(day=="周二") daynumber=1;
        else if(day=="周三") daynumber=2;
        else if(day=="周四") daynumber=3;
        else if(day=="周五") daynumber=4;
        else if(day=="周六") daynumber=5;
        else if(day=="周日") daynumber=6;
        //下面这句非常重要
        if(daynumber+1>column) continue;//比如课在周日但是课表只显示周一到周五的，就跳过

        int class_begin_at=rec.value("begin_at").toInt()-1;//从哪节开始上课
        int class_end_at=rec.value("end_at").toInt();//到哪节下课

        QColor c;
        if(rec.value("color").toString()=="") c=Qt::black;//如果数据库没有存颜色，默认用黑色，但一般这句不会触发
        else c=rec.value("color").toString();

        for(;class_begin_at<class_end_at;class_begin_at++){
            if(class_begin_at>row-1) break;//如果课程节位置超过一天范围，直接结束
            QStandardItem *m=new QStandardItem(rec.value("name").toString());
            m->setTextAlignment(Qt::AlignHCenter |Qt::AlignVCenter);
            if(rec.value("text").toString()!=""){//如果有备注
                m->setToolTip(rec.value("text").toString());//填写备注
                QFont font=m->font();
                font.setBold(true);
                m->setFont(font);//将其加粗
            }
            m->setBackground(c);//为单元格背景设置颜色
            mmodel->setItem(class_begin_at,daynumber,m);
        }
    }
}

void courseform::on_bt_upweek_clicked()//点击上一周
{
    mmodel->clear();
    loadtable();
    watching_week-=1;
    loadcourse();
    if(watching_week==1) ui->bt_upweek->setHidden(true);//如果是从第二周切换到第一周，隐藏 上一周 按钮
}

void courseform::on_bt_downweek_clicked()//下一周
{
    mmodel->clear();
    loadtable();
    watching_week+=1;
    loadcourse();
    if(ui->bt_upweek->isHidden()) ui->bt_upweek->setHidden(false);//如果是从第一周切换到第二周，显示 上一周 按钮
}


void courseform::on_bt_tablesetting_clicked()//点击课表设置，以及处理返回结果
{
    Dialog_tablesetting *settable=new Dialog_tablesetting(sqlmodel,this);
    settable->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    settable->setcuroption(row,column,day);
    int ret=settable->exec();
    //在对话框产生返回值前，进程阻塞在这里
    if(ret==QDialog::Accepted){//设置新的课表长和宽，以及第一周
        row=settable->getcuroptionrow();
        column=settable->getcuroptioncolumn();
        if(day!=settable->getcuroptiondate()){//如果开课日期发生了改变
            day=settable->getcuroptiondate();
            QDate now=QDate::currentDate();
            today_week=day.daysTo(now)/7+1;//这个是启动软件时是第几周
            watching_week=today_week;//这个是手动操作想看第几周的课表
            if(watching_week==1) ui->bt_upweek->setHidden(true);
        }

        mmodel->setColumnCount(column);//更新一周天数
        mmodel->setRowCount(row);//更新一天节数

        if(sqlmodel2->rowCount()>row){//如果当前课程节数大于目标值，删除多余的记录
            sqlmodel2->removeRows(row,sqlmodel2->rowCount()-row);
        }
        else if(sqlmodel2->rowCount()<row){//如果当前课程节数小于目标值，新建若干个数据记录
            for(int c=sqlmodel2->rowCount();c<row;c++){
                QSqlRecord newrec=sqlmodel2->record();
                newrec.setValue("begin_at","12:00:00");
                newrec.setValue("end_at","12:00:00");
                newrec.setValue("no",sqlmodel2->rowCount()+1);
                sqlmodel2->insertRecord(c,newrec);
            }
        }
        sqlmodel2->submitAll();
        mmodel->clear();
        loadtable();//更新横竖表头
        loadcourse();//更新本周内课程

    }
    {//无论返回结果如何
        settable->close();//手动关闭窗口
        delete settable;//释放内存
        settable=nullptr;//避免野指针
    }
}

void courseform::on_bt_timesetting_clicked()//点击时间管理按钮
{
    Dialog_timesetting *settime=new Dialog_timesetting(sqlmodel2,this);
    settime->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    settime->setAttribute(Qt::WA_DeleteOnClose);
    QSqlRecord rec=sqlmodel2->record(0);
    settime->inittime(rec);
    connect(settime,&Dialog_timesetting::fresh,this,&courseform::loadtable);
    settime->exec();
}


void courseform::on_tableView_clicked(const QModelIndex &index)
{   switch(index.column()){
        case 0: emit status(QString::asprintf("周一，第%d节 ",index.row()+1)+index.data().toString(),-1); break;
        case 1: emit status(QString::asprintf("周二，第%d节 ",index.row()+1)+index.data().toString(),-1); break;
        case 2: emit status(QString::asprintf("周三，第%d节 ",index.row()+1)+index.data().toString(),-1); break;
        case 3: emit status(QString::asprintf("周四，第%d节 ",index.row()+1)+index.data().toString(),-1); break;
        case 4: emit status(QString::asprintf("周五，第%d节 ",index.row()+1)+index.data().toString(),-1); break;
        case 5: emit status(QString::asprintf("周六，第%d节 ",index.row()+1)+index.data().toString(),-1); break;
        case 6: emit status(QString::asprintf("周日，第%d节 ",index.row()+1)+index.data().toString(),-1); break;
    }
}


void courseform::on_bt_coursemanager_clicked(bool checked)
{
    if(checked){//点击打开窗口
        ui->bt_coursemanager->setText("点击关闭");
        course_manager=new widget_coursemanager(db,mmodel,sqlmodel3,nullptr);
        course_manager->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
        course_manager->setAttribute(Qt::WA_DeleteOnClose);//关闭窗口时自动释放内存，避免内存占用无限上涨
        course_manager->show();
        connect(course_manager,&widget_coursemanager::wantsetenable,this,&courseform::enable);
        connect(course_manager,&widget_coursemanager::wantloadtable,this,&courseform::loadtable);
        connect(course_manager,&widget_coursemanager::wantloadcourse,this,&courseform::loadcourse);
    }
    else{//再点击关闭窗口
        course_manager->close();
        ui->bt_coursemanager->setText("课程管理");
    }
}

