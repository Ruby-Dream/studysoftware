#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widget_course.h"
#include "widget_coursefile.h"
#include "widget_notice.h"
#include <QSystemTrayIcon>//右下角程序托盘图标
#include <QSqlDatabase>//数据库连接
#include <QDate>
#include <QTime>
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
    void do_status(QString s,int LorR);
    void on_bt_course_clicked();

    void on_bt_coursefile_clicked();

    void on_bt_notice_clicked();

    void do_singleshot_timeout_personal();

    void do_singleshot_timeout_course();

    void update_personal();
    //更新软件运行这时候的日期（与用户无关）
    //什么时候需要更新：第一次启动时，过午夜12点时,事务通知发生修改时
    void update_course();
    //更新软件运行这时候是星期几（与用户有关）
    //什么时候需要更新：第一次启动时，过午夜12点时，修改学期第一天时，修改课程开始节时，修改课程前多少分钟提醒时
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *t;
    courseform *_courseform=nullptr;
    widget_coursefile *_widget_coursefile=nullptr;
    widget_notice *_widget_notice=nullptr;
    QSqlDatabase db;
    QSqlTableModel *sqlmodel;//数据库中课表配置
    QSqlTableModel *sqlmodel2;//时间表，上课时间下课时间对应的节
    QSqlTableModel *sqlmodel3;//课程
    int currentwidget;//切换前是哪个窗口，需要释放内存
    void delete_old_widget();//切换时释放上一个窗口内存
    QSqlQueryModel *qrymodel,*qrymodel2;//用来查下一个需要定时的事务
    QString date;//今天是哪年哪月哪日
    int currentweek;//这是学期的第几周
    QString workday;//今天是星期几
    void setsingleshot_personal();
    void setsingleshot_course();
    QString personal_notice,course_notice;//事务提醒备注和课程备注
    QTimer *personal_timer,*course_timer;//事务计时器和课程计时器
    QLabel *left,*right;

};
#endif // MAINWINDOW_H
