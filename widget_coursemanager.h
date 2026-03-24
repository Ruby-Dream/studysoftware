#ifndef WIDGET_COURSEMANAGER_H
#define WIDGET_COURSEMANAGER_H

#include <QWidget>
#include <QSqlTableModel>//课程表格模型
#include <QItemSelectionModel>//选择模型
#include <QStandardItemModel>
#include <QSqlRecord>//sql记录读取写入
#include <QSqlDatabase>
#include <QSqlQuery>

#include "lineeditdelegate.h"
#include "comboboxdelegate.h"
#include "spindelegate.h"
#include "colordelegate.h"
namespace Ui {
class widget_coursemanager;
}

class widget_coursemanager : public QWidget
{
    Q_OBJECT

public:
    explicit widget_coursemanager(QSqlDatabase db,QStandardItemModel *mmodel,QSqlTableModel *sqlmodel,QWidget *parent = nullptr);

    ~widget_coursemanager();
private slots:
    void do_fresh_coursemanager();
    void do_currentRowChanged(const QModelIndex &current,const QModelIndex &previous);

    void opentable();

    void on_bt_new_clicked();

    void on_bt_delete_clicked();

    void sql(QString old,QString now);

private:
    Ui::widget_coursemanager *ui;
    QSqlTableModel *sqlmodel3;
    QStandardItemModel *mmodel;
    QItemSelectionModel *selection;
    QSqlDatabase db;

    LineeditDelegate *lineedit_delegate1,*lineedit_delegate2;
    ComboboxDelegate *combobox_delegate;
    SpinDelegate *spin_delegate;
    ColorDelegate *color_delegate;
signals:
    void wantloadtable();
    void wantloadcourse();
    void wantsetenable();
};

#endif // WIDGET_COURSEMANAGER_H
