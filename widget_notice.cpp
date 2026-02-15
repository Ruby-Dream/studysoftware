#include "widget_notice.h"
#include "ui_widget_notice.h"

widget_notice::widget_notice(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget_notice)
{
    ui->setupUi(this);
}

widget_notice::~widget_notice()
{
    delete ui;
}
