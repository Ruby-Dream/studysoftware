#include "widget_notice.h"
#include "ui_widget_notice.h"

widget_notice::widget_notice(QSqlDatabase db,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget_notice)
{
    ui->setupUi(this);
    this->db=db;

}

widget_notice::~widget_notice()
{
    delete ui;
}
