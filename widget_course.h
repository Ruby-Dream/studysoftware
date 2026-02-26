#ifndef WIDGET_COURSE_H
#define WIDGET_COURSE_H

#include <QStandardItemModel>//课表表格模型
#include <QSqlTableModel>//sql表格模型
#include <QSqlDatabase>//数据库连接
#include <QMessageBox>//消息框
#include <QFileDialog>//打开文件操作
#include <QSqlRecord>//sql记录读取写入
#include <QSqlQueryModel>//sql查询模型
#include <QWidget>

namespace Ui {
class courseform;
}

class courseform : public QWidget
{
    Q_OBJECT

public:
    explicit courseform(QSqlTableModel *sqlmodel,QSqlTableModel *sqlmodel2,QSqlTableModel *sqlmodel3,QWidget *parent = nullptr);
    ~courseform();
    void loadtable();
    void enable();
    void loadcourse(int weekof);
    int selected_week;//当前课表显示的是第几周

private slots:
    void on_bt_change_clicked(bool checked);

    void on_bt_upweek_clicked();

    void on_bt_downweek_clicked();

    void on_bt_tablesetting_clicked();

    void on_bt_coursemanager_clicked();

    void on_bt_timesetting_clicked();


private:
    Ui::courseform *ui;
    QStandardItemModel *mmodel;
    //QWidget p;

    int row;//一天的课程节数
    int column;//一周的天数
    QDate day;//一学期开学第一周的周一
    int week;//当前周是第几周




    QSqlTableModel *sqlmodel,*sqlmodel2,*sqlmodel3;
    QSqlDatabase db;
    //QSqlQueryModel *qrymodel;
};

#endif // WIDGET_COURSE_H
