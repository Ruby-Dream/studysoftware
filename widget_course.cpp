#include "widget_course.h"
#include "dialog_timesetting.h"
#include "ui_widget_course.h"
#include "dialog_tablesetting.h"
#include "widget_coursemanager.h"

courseform::courseform(QSqlDatabase db,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::courseform)
{
    ui->setupUi(this);
    this->db=db;
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

    QSqlRecord rec=sqlmodel->record(0);
    column=rec.value("workdays").toInt();
    row=rec.value("classes").toInt();
    day=rec.value("firstday").toDate();
    mmodel=new QStandardItemModel(row,column,this);
    ui->tableView->setModel(mmodel);//课程表模型-视图

    //第一次启动时获取时间
    //day=sqlmodel->record(0).value("firstday").toDate();
    //QDate now=QDate::currentDate();
    // qint64 d=date.daysTo(now);
    // int week=d/7+1;
    // ui->label_2->setText(QString::asprintf("这是第%d周星期%d",week,now.dayOfWeek()));
    // //ui->label_2->setText(date.toString("yyyy-MM-dd"));

    //model->clear();
    loadtable();//加载课表结构框架
    QDate now=QDate::currentDate();
    week=day.daysTo(now)/7+1;
    selected_week=week;
    loadcourse(selected_week);//加载数据库中的课程
}

courseform::~courseform()
{
    delete ui;
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
    }
    else if(column==6){
        strlist<<"周一"<<"周二"<<"周三"<<"周四"<<"周五"<<"周六";
    }
    else if(column==7){
        strlist<<"周一"<<"周二"<<"周三"<<"周四"<<"周五"<<"周六"<<"周日";
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

void courseform::loadcourse(int weekof)//加载课程
{
    ui->labelweek->setText(QString::asprintf("第%d周",weekof));
    for(int i=0;i<sqlmodel3->rowCount();i++){
        QSqlRecord rec=sqlmodel3->record(i);
        if(rec.value("weekfrom").toInt()>weekof ||rec.value("weekto").toInt()<weekof){//如果当前不在开课周和结课周之间，跳过
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
        if(rec.value("color").toString()=="") c=Qt::black;//如果数据库没有存颜色，默认用黑色
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
            m->setBackground(c);
            mmodel->setItem(class_begin_at,daynumber,m);
        }
    }
}

void courseform::on_bt_upweek_clicked()//点击上一周
{
    mmodel->clear();
    loadtable();
    loadcourse(selected_week-=1);
    if(selected_week==1) ui->bt_upweek->setHidden(true);//如果是从第二周切换到第一周，隐藏 上一周 按钮

}


void courseform::on_bt_downweek_clicked()//下一周
{
    mmodel->clear();
    loadtable();
    loadcourse(selected_week+=1);
    if(ui->bt_upweek->isHidden()) ui->bt_upweek->setHidden(false);//如果是从第一周切换到第二周，显示 上一周 按钮
}


void courseform::on_bt_tablesetting_clicked()//点击课表设置，以及处理返回结果
{
    Dialog_tablesetting *settable=new Dialog_tablesetting(sqlmodel,this);
    settable->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    //settable->setAttribute(Qt::WA_DeleteOnClose);
    settable->setcuroption(row,column,day);
    int ret=settable->exec();
    if(ret==QDialog::Accepted){//设置新的课表长和宽，以及第一周
        row=settable->getcuroptionrow();
        column=settable->getcuroptioncolumn();
        day=settable->getcuroptiondate();

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
        loadcourse(week);//更新本周内课程

    }
    {//无论返回结果如何
        settable->close();//关闭窗口
        delete settable;//释放内存
        settable=nullptr;//避免野指针
    }
}


void courseform::on_bt_coursemanager_clicked()//点击课程管理按钮
{
    widget_coursemanager *course=new widget_coursemanager(this,mmodel,sqlmodel3,nullptr);
    course->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    course->setAttribute(Qt::WA_DeleteOnClose);//关闭窗口时自动释放内存，避免内存占用无限上涨
    course->show();
}


void courseform::on_bt_timesetting_clicked()//点击时间管理按钮
{
    Dialog_timesetting *settime=new Dialog_timesetting(sqlmodel2,this);
    settime->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    settime->setAttribute(Qt::WA_DeleteOnClose);
    QSqlRecord rec=sqlmodel2->record(0);
    settime->inittime(rec);
    settime->exec();
}

