#ifndef WIDGET_COURSE_H
#define WIDGET_COURSE_H
#include <QtSql>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QFileDialog>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QWidget>

namespace Ui {
class courseform;
}

class courseform : public QWidget
{
    Q_OBJECT

public:
    explicit courseform(QSqlDatabase db,QWidget *parent = nullptr);
    ~courseform();
    void loadcourse(int weekof);

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


    void loadtable();
    QSqlTableModel *sqlmodel,*sqlmodel2,*sqlmodel3;
    QSqlDatabase db;
    //QSqlQueryModel *qrymodel;
};

#endif // WIDGET_COURSE_H
