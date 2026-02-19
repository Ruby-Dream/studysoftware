#ifndef DIALOG_TIMESETTING_H
#define DIALOG_TIMESETTING_H

#include <QDialog>
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTime>
namespace Ui {
class Dialog_timesetting;
}

class Dialog_timesetting : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_timesetting(QSqlTableModel *model,QWidget *parent = nullptr);
    void inittime(QSqlRecord rec);
    ~Dialog_timesetting();

private slots:
    void on_btsavetime_clicked();

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::Dialog_timesetting *ui;
    QSqlTableModel *sqlmodel2;
};

#endif // DIALOG_TIMESETTING_H
