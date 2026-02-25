#ifndef WIDGET_COURSEMANAGER_H
#define WIDGET_COURSEMANAGER_H

#include <QWidget>
#include <QSqlTableModel>//课程表格模型
#include <QItemSelectionModel>//选择模型
#include <QStandardItemModel>
#include <QSqlRecord>//sql记录读取写入
#include <QColorDialog>//颜色对话框
#include <QDataWidgetMapper>//数据映射组件
#include "widget_course.h"
namespace Ui {
class widget_coursemanager;
}

class widget_coursemanager : public QWidget
{
    Q_OBJECT

public:
    explicit widget_coursemanager(courseform *m,QStandardItemModel *mmodel,QSqlTableModel *sqlmodel,QWidget *parent = nullptr);
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
    QStandardItemModel *mmodel;
    QItemSelectionModel *selection;
    QDataWidgetMapper *mapper;
    courseform *w;
    int currentweek;
};

#endif // WIDGET_COURSEMANAGER_H
