#include "dialog_tablesetting.h"
#include "ui_dialog_tablesetting.h"

Dialog_tablesetting::Dialog_tablesetting(QSqlTableModel *m,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_tablesetting)
{
    ui->setupUi(this);
    sqlmodel=m;
}

Dialog_tablesetting::~Dialog_tablesetting()
{
    delete ui;
}
void Dialog_tablesetting::setcuroption(int row, int column,QDate d)
{
    ui->cbcolumn->setCurrentIndex(column-5);//5就输出第0项，6就输出第1项，7就输出第2项
    ui->sbrow->setValue(row);
    ui->dateEdit->setDate(d);
}

int Dialog_tablesetting::getcuroptionrow()//获取课表一天几节课
{
    return ui->sbrow->value();
}

int Dialog_tablesetting::getcuroptioncolumn()//获取课表一周显示几天
{
    return ui->cbcolumn->currentIndex()+5;
}

QDate Dialog_tablesetting::getcuroptiondate()
{
    return ui->dateEdit->date();
}
void Dialog_tablesetting::on_buttonBox_accepted()
{
    if(ui->dateEdit->date().dayOfWeek()!=1){
        QMessageBox::critical(this,"日期错误","日期不是周一");
    }
    else{
        QSqlRecord rec=sqlmodel->record(0);
        rec.setValue("workdays",ui->cbcolumn->currentIndex()+5);
        rec.setValue("classes",ui->sbrow->value());
        rec.setValue("firstday",ui->dateEdit->date().toString("yyyy-MM-dd"));
        sqlmodel->setRecord(0,rec);
        sqlmodel->submitAll();
        this->accept();//返回accept信号
    }
}

