#ifndef WIDGET_COURSEMANAGER_H
#define WIDGET_COURSEMANAGER_H

#include <QWidget>
#include<QSqlTableModel>
#include <QItemSelectionModel>
#include <QMainWindow>
#include <QSqlRecord>
#include <QColorDialog>
#include <QDataWidgetMapper>
#include "courseform.h"
namespace Ui {
class widget_coursemanager;
}

class widget_coursemanager : public QWidget
{
    Q_OBJECT

public:
    explicit widget_coursemanager(courseform *m,int week,QSqlTableModel *sqlmodel,QWidget *parent = nullptr);
    void opentable();
    ~widget_coursemanager();
private slots:
    void do_currentRowChanged(const QModelIndex &current,const QModelIndex &previous);
    void on_btchangecolor_clicked();

    void on_bt_save_clicked();

    void on_bt_new_clicked();

    void on_bt_delete_clicked();

private:
    Ui::widget_coursemanager *ui;
    QSqlTableModel *sqlmodel3;
    QItemSelectionModel *selection;
    QDataWidgetMapper *mapper;
    courseform *w;
    int currentweek;
};

#endif // WIDGET_COURSEMANAGER_H
