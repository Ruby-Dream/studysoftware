#include "dialog_timesetting.h"
#include "ui_dialog_timesetting.h"

Dialog_timesetting::Dialog_timesetting(QSqlTableModel *model,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_timesetting)
{
    ui->setupUi(this);
    sqlmodel2=model;
    ui->spinBox->setMaximum(sqlmodel2->rowCount());
}
void Dialog_timesetting::inittime(QSqlRecord rec)//显示数据库内已存的时间节点
{
    QString begin=rec.value("begin_at").toString();
    QString end=rec.value("end_at").toString();
    ui->beginhour->setValue(begin.sliced(0,2).toInt());
    ui->beginminute->setValue(begin.sliced(3,2).toInt());
    ui->endhour->setValue(end.sliced(0,2).toInt());
    ui->endminute->setValue(end.sliced(3,2).toInt());

}
Dialog_timesetting::~Dialog_timesetting()
{
    delete ui;
}

void Dialog_timesetting::on_btsavetime_clicked()
{
    QTime begin(ui->beginhour->value(),ui->beginminute->value());
    QTime end(ui->endhour->value(),ui->endminute->value());

    //如果上课时间晚于下课时间
    if(begin.toString().mid(0,2).toInt()>end.toString().sliced(0,2).toInt()){
        QMessageBox::critical(this,"时间设置错误","上课时间不能晚于下课时间");
        return;
    }
    else if(begin.toString().mid(0,2).toInt()==end.toString().sliced(0,2).toInt()
             &&begin.toString().mid(3,2).toInt()>end.toString().sliced(3,2).toInt()){
        QMessageBox::critical(this,"时间设置错误","上课时间不能晚于下课时间");
        return;
    }

    QSqlRecord rec=sqlmodel2->record(ui->spinBox->value()-1);
    rec.setValue("begin_at",begin.toString("HH:mm:ss.sss"));
    rec.setValue("end_at",end.toString("HH:mm:ss.sss"));
    if(!sqlmodel2->setRecord(ui->spinBox->value()-1,rec)){
        QMessageBox::critical(this,"w","t");
    }
    sqlmodel2->submitAll();
    ui->btsavetime->setHidden(true);
    emit fresh();
}


void Dialog_timesetting::on_spinBox_valueChanged(int arg1)
{
    QSqlRecord rec=sqlmodel2->record(arg1-1);
    inittime(rec);
}

