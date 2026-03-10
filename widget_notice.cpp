#include "widget_notice.h"

#include "ui_widget_notice.h"

widget_notice::widget_notice(QSqlDatabase db,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget_notice)
{
    ui->setupUi(this);
    this->db=db;
    sqlmodel=new QSqlTableModel(nullptr,db);
    sqlmodel->setTable("course");
    sqlmodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sqlmodel->setSort(sqlmodel->fieldIndex("rowid"),Qt::AscendingOrder);
    if(!sqlmodel->select()){
        QMessageBox::critical(this,"bad","e");
    }

    sqlmodel->setHeaderData(sqlmodel->fieldIndex("name"),Qt::Horizontal,"课程名称");
    sqlmodel->setHeaderData(sqlmodel->fieldIndex("weekfrom"),Qt::Horizontal,"开课周");
    sqlmodel->setHeaderData(sqlmodel->fieldIndex("weekto"),Qt::Horizontal,"结课周");
    sqlmodel->setHeaderData(sqlmodel->fieldIndex("workday_on"),Qt::Horizontal,"周内上课日");
    sqlmodel->setHeaderData(sqlmodel->fieldIndex("begin_at"),Qt::Horizontal,"上课节");
    sqlmodel->setHeaderData(sqlmodel->fieldIndex("notice_before_min"),Qt::Horizontal,"提前提醒(分钟)");
    sqlmodel->setHeaderData(sqlmodel->fieldIndex("notice_text"),Qt::Horizontal,"提醒备注");

    ui->tv_course->setModel(sqlmodel);

    ui->tv_course->setColumnHidden(0,true);
    ui->tv_course->setColumnHidden(6,true);
    ui->tv_course->setColumnHidden(7,true);
    ui->tv_course->setColumnHidden(8,true);
    spin_delegate=new SpinDelegate(this);
    readonly_delegate=new ReadonlyDelegate(this);
    lineedit_delegate=new LineeditDelegate(this);

    ui->tv_course->setItemDelegateForColumn(9,spin_delegate);
    ui->tv_course->setItemDelegateForColumn(10,lineedit_delegate);
    ui->tv_course->setItemDelegateForColumn(0,readonly_delegate);
    ui->tv_course->setItemDelegateForColumn(1,readonly_delegate);
    ui->tv_course->setItemDelegateForColumn(2,readonly_delegate);
    ui->tv_course->setItemDelegateForColumn(3,readonly_delegate);
    ui->tv_course->setItemDelegateForColumn(4,readonly_delegate);
    ui->tv_course->setItemDelegateForColumn(5,readonly_delegate);
    ui->tv_course->setItemDelegateForColumn(6,readonly_delegate);
    ui->tv_course->setItemDelegateForColumn(7,readonly_delegate);
    ui->tv_course->setItemDelegateForColumn(8,readonly_delegate);

    //以上为课程通知
    //以下为事务通知
    sqlmodel2=new QSqlTableModel(nullptr,db);
    sqlmodel2->setTable("notice_personal");
    sqlmodel2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sqlmodel2->setSort(sqlmodel->fieldIndex("rowid"),Qt::AscendingOrder);
    if(!sqlmodel2->select()){
        QMessageBox::critical(this,"bad","e");
    }
    sqlmodel2->setHeaderData(sqlmodel2->fieldIndex("date"),Qt::Horizontal,"提醒日期");
    sqlmodel2->setHeaderData(sqlmodel2->fieldIndex("time"),Qt::Horizontal,"提醒时间");
    sqlmodel2->setHeaderData(sqlmodel2->fieldIndex("text"),Qt::Horizontal,"备注");
    ui->tv_person->setModel(sqlmodel2);
    ui->tv_person->setColumnHidden(0,true);

}

widget_notice::~widget_notice()
{
    delete ui;
}



void widget_notice::on_pushButton_clicked()
{

}

