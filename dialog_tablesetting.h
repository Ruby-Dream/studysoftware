#ifndef DIALOG_TABLESETTING_H
#define DIALOG_TABLESETTING_H

#include <QDialog>
#include <QDialog>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlRecord>

namespace Ui {
class Dialog_tablesetting;
}

class Dialog_tablesetting : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_tablesetting(QSqlTableModel *m,QWidget *parent = nullptr);
    void setcuroption(int row,int column,QDate d);
    int getcuroptionrow();
    int getcuroptioncolumn();
    QDate getcuroptiondate();
    ~Dialog_tablesetting();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog_tablesetting *ui;
    QSqlTableModel *sqlmodel;
};

#endif // DIALOG_TABLESETTING_H
