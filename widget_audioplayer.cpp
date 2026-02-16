#include "widget_audioplayer.h"
#include "ui_widget_audioplayer.h"
#include "QStyle"

widget_audioplayer::widget_audioplayer(QString audiofile,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget_audioplayer)
{
    ui->setupUi(this);
    filename=audiofile;
    player=new QMediaPlayer(this);//配置媒介
    output=new QAudioOutput(this);//配置输出设备

    connect(player,&QMediaPlayer::playbackStateChanged,this,&widget_audioplayer::do_statechanged);//播放状态改变时的槽函数
    connect(player,&QMediaPlayer::positionChanged,this,&widget_audioplayer::do_positionchanged);//播放位置自动改变时的槽函数
    connect(player,&QMediaPlayer::durationChanged,this,&widget_audioplayer::do_durationchanged);//首次加载媒介时配置进度条的槽函数
    output->setVolume(volume);//初始音量
    player->setAudioOutput(output);
    QUrl s=QUrl::fromLocalFile(filename);
    player->setSource(s);
    player->play();

}

widget_audioplayer::~widget_audioplayer()
{
    delete ui;
    player->stop();
    this->close();
}

void widget_audioplayer::on_bt_control_clicked()//点击播放控制按钮时
{
    if(player->playbackState()==QMediaPlayer::PlayingState){//如果是播放时按下按钮
        player->pause();//暂停
        ui->bt_control->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));

    }
    else{//如果是暂停或停止时按下按钮
        player->play();//播放
        ui->bt_control->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPause));
    }
}

void widget_audioplayer::do_statechanged(QMediaPlayer::PlaybackState state)
{
    if(loop && (state==QMediaPlayer::StoppedState)){//如果开了单曲循环
        player->setSource(filename);
        player->play();
    }
    else if(state==QMediaPlayer::StoppedState){//非循环播放结束时，改变按钮图标
        ui->bt_control->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
    }
}


void widget_audioplayer::on_bt_isloop_clicked(bool checked)
{
    loop=checked;
}


void widget_audioplayer::on_volumeslider_valueChanged(int value)//设置音量
{
    volume=float(value)/100;
    output->setVolume(volume);
    ui->label_volume->setText(QString::asprintf("%d",ui->volumeslider->value()));
}


// void widget_audioplayer::on_playerslider_valueChanged(int value)不能这么写，会由于自动播放音频而高频触发
// {
//     //player->setPosition(value);
// }

void widget_audioplayer::do_positionchanged(qint64 position)//音乐播放时，动态移动播放进度条，参数单位是毫秒
{
    if(ui->playerslider->isSliderDown()) return;//如果这时鼠标正在按住滑块，停止自动移动
    ui->playerslider->setSliderPosition(position);

    // int sec=position/1000;
    // int min=sec/60;
    // sec=sec%60;
}

void widget_audioplayer::do_durationchanged(qint64 duration)//设置播放进度条最大值
{
    ui->playerslider->setMaximum(duration);
}


void widget_audioplayer::on_playerslider_sliderReleased()//只有鼠标松开滑块时才改变音频播放位置，不能用valuechanged信号
{
    qint64 now=ui->playerslider->value();
    player->setPosition(now);
}




void widget_audioplayer::on_bt_volume_up_clicked()//音量加，然后触发on_volumeslider_valueChanged
{
    ui->volumeslider->setValue(ui->volumeslider->value()+5);
}


void widget_audioplayer::on_bt_volume_down_clicked()//音量减，然后触发on_volumeslider_valueChanged
{
    ui->volumeslider->setValue(ui->volumeslider->value()-5);
}

