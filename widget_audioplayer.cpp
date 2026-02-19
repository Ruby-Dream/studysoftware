#include "widget_audioplayer.h"
#include "ui_widget_audioplayer.h"
#include "QStyle"

widget_audioplayer::widget_audioplayer(QString audiofile,QSqlDatabase db,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget_audioplayer)
{
    ui->setupUi(this);
    filename=audiofile;//传入的文件名，绝对路径
    this->db=db;
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
    listmodel=new QStringListModel();
    qrymodel3=new QSqlQueryModel(this);//初始化时间节点查询模型

    ui->timeview->setModel(listmodel);
    loadtimestamp();
}

widget_audioplayer::~widget_audioplayer()
{
    delete ui;
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


void widget_audioplayer::on_bt_isloop_clicked(bool checked)//改变单曲循环
{
    loop=checked;
}


void widget_audioplayer::on_volumeslider_valueChanged(int value)//拖动音量滑块条以设置音量
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
    QString playedtime=gettime(position);
    ui->playedtime->setText(playedtime);
}

void widget_audioplayer::do_durationchanged(qint64 duration)//设置播放进度条最大值
{
    QString totaltime=gettime(duration);
    ui->playerslider->setMaximum(duration);
    ui->totaltime->setText(totaltime);
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


void widget_audioplayer::on_playerslider_sliderMoved(int position)//拖动进度条时动态变化时间戳
{
    QString playedtime=gettime(position);
    ui->playedtime->setText(playedtime);
}


void widget_audioplayer::on_timeview_doubleClicked(const QModelIndex &index)//双击时间戳时跳转播放位置
{
    QString s=listmodel->data(index).toString();
    int position=s.mid(0,2).toInt()*60000+s.mid(3,2).toInt()*1000;
    player->setPosition(position);
}

QString widget_audioplayer::gettime(int position)//将毫秒转换为mm:ss格式的字符串
{
    int sec=position/1000;
    int min=sec/60;
    sec=sec%60;
    QString time=QString::asprintf("%.2d:%.2d",min,sec);
    return time;
}

void widget_audioplayer::loadtimestamp()//加载该文件对应的时间节点和备注，根据时间升序排序
{
    qrymodel3->setQuery("SELECT timestamp,text from media_time where media = \""+filename+"\" ORDER BY timestamp ASC" ,db);//读取所有该音频的时间节点和节点备注
    QStringList strlist;
    for(int i=0;i<qrymodel3->rowCount();i++){
        QSqlRecord rec=qrymodel3->record(i);
        int position=rec.value("timestamp").toInt();
        QString playedtime=gettime(position);
        strlist<<playedtime;
    }
    listmodel->setStringList(strlist);

}


void widget_audioplayer::on_bt_newtime_clicked()//新建时间节点
{
    int time=player->position();//当前播放位置
    QSqlQueryModel *q=new QSqlQueryModel();
    q->setQuery("SELECT media from media_time",db);
    int row=q->rowCount();
    QSqlQuery query(db);
    query.prepare("INSERT into media_time (no,media,timestamp) values(?,?,?)");//节点条目插入数据库
    query.bindValue(0,row+1);
    query.bindValue(1,filename);
    query.bindValue(2,time);
    query.exec();

    loadtimestamp();//刷新时间节点视图
}


void widget_audioplayer::on_bt_deletetime_clicked()
{

}

