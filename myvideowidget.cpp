#include "myvideowidget.h"

Myvideowidget::Myvideowidget(QWidget *parent):QVideoWidget(parent)  {
}

void Myvideowidget::keyPressEvent(QKeyEvent *event)//处理escape事件
{
    if(event->key()==Qt::Key_Escape && this->isFullScreen()){
        this->setFullScreen(false);//取消全屏
        this->setGeometry(40,50,640,360);//重新设置布局
        event->accept();
        QVideoWidget::keyPressEvent(event);
    }
}

void Myvideowidget::mousePressEvent(QMouseEvent *event)//处理点击事件
{
    if(event->button()==Qt::LeftButton){
        emit statechange();//发射信号，相当于点击了控制按钮
    }
    QVideoWidget::mousePressEvent(event);
}

void Myvideowidget::setMediaPlayer(QMediaPlayer *player)
{
    this->player=player;
}
