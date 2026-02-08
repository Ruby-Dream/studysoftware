#ifndef COURSEFORM_H
#define COURSEFORM_H
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
    explicit courseform(QWidget *parent = nullptr);
    ~courseform();
    void loadcourse(int weekof);

private slots:
    void on_bt_change_clicked(bool checked);

    void on_bt_upweek_clicked();

    void on_bt_downweek_clicked();

    void on_bt_tablesetting_clicked();

    void on_pushButton_clicked();

private:
    Ui::courseform *ui;
    QStandardItemModel *model;

    int row;//一天的课程节数
    int column;//一周的天数
    QDate day;//一学期开学第一周的周一
    int week;//当前周是第几周


    void loadtable();
    QSqlTableModel *sqlmodel,*sqlmodel2,*sqlmodel3,*sqlmodel4;
    QSqlDatabase db,db2,db3,db4;
    QSqlQueryModel *qrymodel;
};

#endif // COURSEFORM_H
