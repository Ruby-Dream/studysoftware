#ifndef WIDGET_NOTICE_H
#define WIDGET_NOTICE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QMessageBox>
#include "spindelegate.h"
#include "readonlydelegate.h"
#include "lineeditdelegate.h"

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
    QSqlTableModel *sqlmodel;
    SpinDelegate *spin_delegate;
    ReadonlyDelegate *readonly_delegate;
    LineeditDelegate *lineedit_delegate;
};

#endif // WIDGET_NOTICE_H
