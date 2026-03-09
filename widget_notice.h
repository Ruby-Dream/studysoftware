#ifndef WIDGET_NOTICE_H
#define WIDGET_NOTICE_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class widget_notice;
}

class widget_notice : public QWidget
{
    Q_OBJECT

public:
    explicit widget_notice(QSqlDatabase db,QWidget *parent = nullptr);
    ~widget_notice();

private:
    Ui::widget_notice *ui;
    QSqlDatabase db;
};

#endif // WIDGET_NOTICE_H
