#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widget_course.h"
#include "widget_coursefile.h"
#include <QSystemTrayIcon>
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

private:
    Ui::MainWindow *ui;
    courseform *_courseform=nullptr;
    widget_coursefile *_widget_coursefile=nullptr;
};
#endif // MAINWINDOW_H
