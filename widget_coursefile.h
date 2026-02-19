#ifndef WIDGET_COURSEFILE_H
#define WIDGET_COURSEFILE_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QStringListModel>
#include <QMainWindow>
#include <QSqlTableModel>
#include <QItemSelectionModel>

#include <QSqlRecord>
#include <QSqlQuery>
#include <QDesktopServices>
#include <QStringListModel>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include "widget_audioplayer.h"
namespace Ui {
class widget_coursefile;
}

class widget_coursefile : public QWidget
{
    Q_OBJECT

public:
    explicit widget_coursefile(QSqlDatabase db,QWidget *parent = nullptr);
    ~widget_coursefile();

private slots:
    void on_bt_include_clicked();

    void on_listView_course_clicked(const QModelIndex &index);

    void on_listView_file_clicked(const QModelIndex &index);

    void on_listView_file_doubleClicked(const QModelIndex &index);

    void on_bt_delete_clicked();

    void on_bt_save_clicked();

    void on_plainTextEdit_filetext_textChanged();

private:
    Ui::widget_coursefile *ui;
    QSqlQueryModel *qrymodel,*qrymodel2;
    QSqlDatabase db;
    QStringListModel *listmodel1,*listmodel2;
    QItemSelectionModel *selection1,*selection2;
    QSqlTableModel *sqlmodel4;
    void loadfile();
};

#endif // WIDGET_COURSEFILE_H
