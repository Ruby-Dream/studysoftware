#ifndef MYVIDEOWIDGET_H
#define MYVIDEOWIDGET_H

#include <QObject>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QKeyEvent>

class Myvideowidget : public QVideoWidget
{
    Q_OBJECT

private:
    QMediaPlayer  *player;
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
public:
    Myvideowidget(QWidget *parent =nullptr);
    void setMediaPlayer(QMediaPlayer *player);
signals:
    void statechange();
};

#endif // MYVIDEOWIDGET_H
