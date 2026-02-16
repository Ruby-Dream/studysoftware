#ifndef WIDGET_AUDIOPLAYER_H
#define WIDGET_AUDIOPLAYER_H

#include <QWidget>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
namespace Ui {
class widget_audioplayer;
}

class widget_audioplayer : public QWidget
{
    Q_OBJECT

public:
    explicit widget_audioplayer(QString audiofile,QWidget *parent = nullptr);
    ~widget_audioplayer();

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

private:
    Ui::widget_audioplayer *ui;
    bool loop=false;
    QMediaPlayer *player;
    QAudioOutput *output;
    float volume=0.5;
    QString filename;
};

#endif // WIDGET_AUDIOPLAYER_H
