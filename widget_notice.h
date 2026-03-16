#ifndef WIDGET_NOTICE_H
#define WIDGET_NOTICE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QMessageBox>
#include "spindelegate.h"
#include "readonlydelegate.h"
#include "lineeditdelegate.h"
#include "datedelegate.h"
#include "timedelegate.h"
#include <QSqlRecord>
#include <QSqlQuery>

namespace Ui {
class widget_notice;
}

class widget_notice : public QWidget
{
    Q_OBJECT

public:
    explicit widget_notice(QSqlDatabase db,QWidget *parent = nullptr);
    ~widget_notice();

private slots:

    void on_bt_new_clicked();

    void on_pushButton_2_clicked();

    void on_tv_person_clicked(const QModelIndex &index);

    void on_bt_refresh_clicked();

private:
    Ui::widget_notice *ui;
    QSqlDatabase db;
    QSqlTableModel *sqlmodel,*sqlmodel2;
    SpinDelegate *spin_delegate;
    ReadonlyDelegate *readonly_delegate;
    LineeditDelegate *lineedit_delegate1,*lineedit_delegate2;
    DateDelegate *date_delegate;
    TimeDelegate *time_delegate;
    void loadnotice_personal();
signals:
    void refresh();
};

#endif // WIDGET_NOTICE_H
