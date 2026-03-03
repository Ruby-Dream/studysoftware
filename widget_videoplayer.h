#ifndef WIDGET_VIDEOPLAYER_H
#define WIDGET_VIDEOPLAYER_H

#include <QWidget>
#include <QWidget>
#include <QtMultimedia>//媒体
#include <QtMultimediaWidgets>//媒体视图
#include <QSqlQueryModel>//sql查询模型
#include <QSqlRecord>//sql记录读取写入
#include <QSqlTableModel>//sqltable模型
#include <QStringListModel>//qstringlist模型
#include <QSqlQuery>//执行sql语句
#include "QStyle"//用于播放/暂停图标变化吧
namespace Ui {
class widget_videoplayer;
}

class widget_videoplayer : public QWidget
{
    Q_OBJECT

public:
    explicit widget_videoplayer(QString videofile,QSqlDatabase db,QWidget *parent = nullptr);
    ~widget_videoplayer();
private slots:
    void on_bt_control_clicked();

    void on_bt_isloop_clicked(bool checked);

    void do_statechanged(QMediaPlayer::PlaybackState state);

    void on_volumeslider_valueChanged(int value);

    void do_positionchanged(qint64 position);

    void do_durationchanged(qint64 duration);

    void on_playerslider_sliderReleased();

    void on_bt_volume_up_clicked();

    void on_bt_volume_down_clicked();

    void on_playerslider_sliderMoved(int position);

    void on_timeview_doubleClicked(const QModelIndex &index);

    void on_bt_newtime_clicked();

    void on_bt_deletetime_clicked();

    void on_timeview_clicked(const QModelIndex &index);

    void on_bt_save_clicked();

    void on_bt_fullscreen_clicked();

private:
    Ui::widget_videoplayer *ui;
    QSqlDatabase db;
    bool loop=false;
    QMediaPlayer *player;
    QAudioOutput *output;
    float volume=0.5;
    QString filename;
    QSqlQueryModel *qrymodel3;
    QStringListModel *listmodel;

    QString getstringtime(int position);
    int getinttime(QString time);
    void loadtimestamp();
};

#endif // WIDGET_VIDEOPLAYER_H
