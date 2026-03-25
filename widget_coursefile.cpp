#include "widget_coursefile.h"
#include "ui_widget_coursefile.h"

widget_coursefile::widget_coursefile(QSqlDatabase db,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget_coursefile)
{
    ui->setupUi(this);
    this->db=db;
    ui->bt_include->installEventFilter(this);
    ui->bt_delete->installEventFilter(this);
    ui->bt_save->installEventFilter(this);

    sqlmodel4=new QSqlTableModel(nullptr,db);
    sqlmodel4->setTable("coursefile");
    sqlmodel4->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sqlmodel4->setSort(sqlmodel4->fieldIndex("no"),Qt::AscendingOrder);
    if(!sqlmodel4->select()){
        QMessageBox::critical(this,"bad","e");
    }
    qrymodel=new QSqlQueryModel(this);//sql语句查询所有课程名
    qrymodel->setQuery("SELECT name FROM course group by name",db);

    listmodel1=new QStringListModel(this);//记录课程条目的列表模型
    QStringList strlist;
    strlist<<"全部";
    for(int i=0;i<qrymodel->rowCount();i++){
        QSqlRecord rec=qrymodel->record(i);
        QString s=rec.value("name").toString();
        strlist<<s;
    }
    strlist<<"其他";
    listmodel1->setStringList(strlist);//列表项第一是 全部 ，最后是 其他 ，课程夹在中间
    ui->listView_course->setModel(listmodel1);//绑定模型视图

    ui->listView_course->setCurrentIndex(listmodel1->index(0));//
    qrymodel2=new QSqlQueryModel();
    qrymodel2->setQuery("SELECT file from coursefile",db);//查询全部课件
    QStringList strlist2;
    listmodel2=new QStringListModel(this);
    for(int j=0;j<qrymodel2->rowCount();j++){
        QSqlRecord rec=qrymodel2->record(j);
        QString s=rec.value("file").toString();
        strlist2<<s;
    }
    listmodel2->setStringList(strlist2);
    ui->listView_file->setModel(listmodel2);
    ui->bt_delete->setEnabled(false);
    emit status("全部",-1);
}

widget_coursefile::~widget_coursefile()
{
    delete ui;
}

bool widget_coursefile::eventFilter(QObject *watched, QEvent *event)//事件过滤器
{
    if(watched==ui->bt_include && event->type()==QEvent::Enter){
        emit status("导入一个或多个课件记录，重复的课件将被忽略",1);
    }
    else if(watched==ui->bt_delete && event->type()==QEvent::Enter){
        emit status("删除选中的课件记录及备注，若是媒体则其节点将一并删除",1);
    }
    else if(watched==ui->bt_save && event->type()==QEvent::Enter){
        emit status("保存该课件的备注，也可留空",1);
    }
    else if(event->type()==QEvent::Leave){
        emit status(" ",1);
    }
    return QWidget::eventFilter(watched,event);
}

void widget_coursefile::loadfile()//加载视图，显示对应科目下的文件
{
    QString course=ui->listView_course->currentIndex().data().toString();//当前课程名
    if(course!="全部")
    {
        qrymodel2->setQuery("SELECT file from coursefile where name = \""+course+"\"",db);
        ui->bt_include->setEnabled(true);
    }
    else{
        qrymodel2->setQuery("SELECT file from coursefile",db);
        ui->bt_include->setEnabled(false);
    }
    QStringList strlist2;//当前课程的课件列表
    for(int j=0;j<qrymodel2->rowCount();j++){
        QSqlRecord rec=qrymodel2->record(j);
        QString s=rec.value("file").toString();
        strlist2<<s;
    }
    listmodel2->setStringList(strlist2);
}

void widget_coursefile::on_bt_include_clicked()//导入课件
{
    QString curpath=QDir::currentPath();
    QString title="导入一个或多个文件（重复课件将被忽略）";
    QString filter="所有文件(*.*)";
    QStringList filelist=QFileDialog::getOpenFileNames(this,title,curpath,filter);//打开选择文件对话框
    if(!filelist.isEmpty()){
        QSqlQueryModel *model=new QSqlQueryModel();
        QString course=ui->listView_course->currentIndex().data().toString();//课程名
        for(int i=0;i<filelist.size();i++){//对于每一个待导入的课件
            QString filename=filelist.at(i);
            model->setQuery("SELECT file from coursefile where file =\""+filename+"\"",db);
            if(model->rowCount()>0) {
                continue;//如果已经有这个文件了，跳过
            }
            QSqlRecord rec=sqlmodel4->record();
            rec.setValue("no",sqlmodel4->record(sqlmodel4->rowCount()-1).value("no").toInt()+1);//no主键为最后一条记录的+1值，防止重复
            rec.setValue("name",course);
            rec.setValue("file",filelist.at(i));
            sqlmodel4->insertRecord(sqlmodel4->rowCount(),rec);
        }
        sqlmodel4->submitAll();//更改到数据库
        loadfile();
    }
}


void widget_coursefile::on_listView_course_clicked(const QModelIndex &index)//单击课程时，显示该课程下的课件
{
    QString s=listmodel1->data(index).toString();
    if(s!="全部")
    {
        qrymodel2->setQuery("SELECT file from coursefile where name = \""+s+"\"",db);
        ui->bt_include->setEnabled(true);
    }
    else{
        qrymodel2->setQuery("SELECT file from coursefile",db);
        ui->bt_include->setEnabled(false);
    }
    ui->bt_delete->setEnabled(false);
    ui->bt_save->setEnabled(false);
    QStringList strlist2;
    for(int j=0;j<qrymodel2->rowCount();j++){
        QSqlRecord rec=qrymodel2->record(j);
        QString s=rec.value("file").toString();
        strlist2<<s;
    }
    listmodel2->setStringList(strlist2);
    ui->listView_file->setModel(listmodel2);
    ui->plainTextEdit_filetext->clear();//切换课程时清空备注
    ui->plainTextEdit_filetext->setEnabled(false);
    emit status(s,-1);
}


void widget_coursefile::on_listView_file_clicked(const QModelIndex &index)//单击文件时，显示备注
{
    QString s=listmodel2->data(index).toString();
    QSqlQueryModel *q=new QSqlQueryModel();
    q->setQuery("SELECT text from coursefile where file =\""+s+"\"",db);
    QSqlRecord r=q->record(0);
    ui->plainTextEdit_filetext->setPlainText(r.value("text").toString());
    QString ss=ui->listView_course->currentIndex().data().toString();
    ui->bt_delete->setEnabled(true);
    ui->bt_save->setEnabled(true);
    ui->plainTextEdit_filetext->setEnabled(true);
    emit status(ss+","+s,-1);
}


void widget_coursefile::on_listView_file_doubleClicked(const QModelIndex &index)//双击课件条目时打开文件
{
    QString file=listmodel2->data(index).toString();
    if(!QFile::exists(file)){//如果文件路径无效，询问是否删除该条目的对话框
        auto result=QMessageBox::information(this,"文件路径无效","文件可能已被移动或删除\n是否删除该条目?",QMessageBox::Yes|QMessageBox::Cancel,QMessageBox::Cancel);
        if(result==QMessageBox::Yes){
            QSqlQuery query(db);
            query.prepare("DELETE from coursefile where file = ?");
            query.bindValue(0,file);
            query.exec();
            loadfile();
        }
    }
    else{
        QFileInfo fileinfo(file);
        if(fileinfo.suffix().toLower()=="mp3"){
            widget_audioplayer *_audioplayer=new widget_audioplayer(file,db,nullptr);
            _audioplayer->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
            _audioplayer->setAttribute(Qt::WA_DeleteOnClose);
            _audioplayer->show();
        }
        else if(fileinfo.suffix().toLower()=="mp4" ||fileinfo.suffix().toLower()=="wmv" || fileinfo.suffix().toLower()=="mkv"){
            widget_videoplayer *_videoplayer=new widget_videoplayer(file,db,nullptr);
            _videoplayer->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
            _videoplayer->setAttribute(Qt::WA_DeleteOnClose);
            _videoplayer->show();
        }
        else{
            QDesktopServices::openUrl(QUrl::fromLocalFile(file));
        }
    }

}


void widget_coursefile::on_bt_delete_clicked()//点击删除当前课件条目
{
    QString filename=ui->listView_file->currentIndex().data().toString();
    QString coursename=ui->listView_course->currentIndex().data().toString();
    QSqlQuery query(db);
    query.prepare("DELETE from coursefile where file =?");//删除课件条目
    query.bindValue(0,filename);
    query.exec();


    query.prepare("DELETE from media_time where media = ?");//顺带删除课件下的时间节点
    query.bindValue(0,filename);
    query.exec();
    ui->plainTextEdit_filetext->clear();
    ui->plainTextEdit_filetext->setEnabled(false);
    ui->bt_save->setEnabled(false);
    loadfile();
}


void widget_coursefile::on_bt_save_clicked()//点击保存
{
    QSqlQuery query(db);
    query.prepare("UPDATE coursefile set text=? where file =?");
    query.bindValue(0,ui->plainTextEdit_filetext->toPlainText());
    query.bindValue(1,ui->listView_file->currentIndex().data().toString());
    query.exec();
    ui->bt_save->setText("已保存");
}


void widget_coursefile::on_plainTextEdit_filetext_textChanged()//保存课件备注到数据库
{
    ui->bt_save->setText("保存该课件的备注");
}

