#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widget_course.h"
#include "widget_coursefile.h"
#include "widget_notice.h"
#include <QSystemTrayIcon>//右下角程序托盘图标
#include <QSqlDatabase>//数据库连接
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_bt_course_clicked();

    void on_bt_coursefile_clicked();

    void on_bt_notice_clicked();

private:
    Ui::MainWindow *ui;
    courseform *_courseform=nullptr;
    widget_coursefile *_widget_coursefile=nullptr;
    widget_notice *_widget_notice=nullptr;
    QSqlDatabase db;
    QSqlTableModel *sqlmodel;//数据库中课表配置
    QSqlTableModel *sqlmodel2;//时间表，上课时间下课时间对应的节
    QSqlTableModel *sqlmodel3;//课程
    int currentwidget;//切换前是哪个窗口，需要释放内存
    void delete_old_widget();
};
#endif // MAINWINDOW_H
