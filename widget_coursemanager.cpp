#include "widget_coursemanager.h"
#include "ui_widget_coursemanager.h"

widget_coursemanager::widget_coursemanager(QSqlDatabase db,int week,QStandardItemModel *mmodel,QSqlTableModel *sqlmodel,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget_coursemanager)
{
    ui->setupUi(this);
    sqlmodel3=sqlmodel;
    this->mmodel=mmodel;
    this->db=db;
    currentweek=week;
    //qDebug() << "当前对象数：" << QApplication::allWidgets().size();
    opentable();

}

widget_coursemanager::~widget_coursemanager()
{
    emit wantsetenable();//让按钮恢复可用
    delete ui;
}

void widget_coursemanager::opentable()
{   //配置表头
    sqlmodel3->setHeaderData(sqlmodel3->fieldIndex("name"),Qt::Horizontal,"课程名称");
    sqlmodel3->setHeaderData(sqlmodel3->fieldIndex("weekfrom"),Qt::Horizontal,"开课周");
    sqlmodel3->setHeaderData(sqlmodel3->fieldIndex("weekto"),Qt::Horizontal,"结课周");
    sqlmodel3->setHeaderData(sqlmodel3->fieldIndex("workday_on"),Qt::Horizontal,"周内上课日");
    sqlmodel3->setHeaderData(sqlmodel3->fieldIndex("begin_at"),Qt::Horizontal,"上课节");
    sqlmodel3->setHeaderData(sqlmodel3->fieldIndex("end_at"),Qt::Horizontal,"下课节");
    sqlmodel3->setHeaderData(sqlmodel3->fieldIndex("color"),Qt::Horizontal,"颜色");
    sqlmodel3->setHeaderData(sqlmodel3->fieldIndex("text"),Qt::Horizontal,"备注");

    //绑定选择模型
    selection=new QItemSelectionModel(sqlmodel3,this);
    connect(selection,&QItemSelectionModel::currentRowChanged,this,&widget_coursemanager::do_currentRowChanged);
    //绑定模型视图
    ui->tableView->setModel(sqlmodel3);
    ui->tableView->setSelectionModel(selection);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(9,true);
    ui->tableView->setColumnHidden(10,true);
    //绑定模型映射组件
    mapper=new QDataWidgetMapper(this);
    mapper->setModel(sqlmodel3);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    mapper->addMapping(ui->name,sqlmodel3->fieldIndex("name"));
    mapper->addMapping(ui->sbfirstweek,sqlmodel3->fieldIndex("weekfrom"));
    mapper->addMapping(ui->sbendweek,sqlmodel3->fieldIndex("weekto"));
    mapper->addMapping(ui->workday,sqlmodel3->fieldIndex("workday_on"));
    mapper->addMapping(ui->coursebegin,sqlmodel3->fieldIndex("begin_at"));
    mapper->addMapping(ui->courseend,sqlmodel3->fieldIndex("end_at"));
    mapper->addMapping(ui->memo,sqlmodel3->fieldIndex("text"));
    mapper->toFirst();
    if(sqlmodel3->rowCount()>0){//如果模型非空
        selection->setCurrentIndex(sqlmodel3->index(0,1),QItemSelectionModel::Select);//首次进入时，选择第一条
    }
}

void widget_coursemanager::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{//行变换时,首次进入也会触发
    Q_UNUSED(previous);
    ui->bt_delete->setEnabled(true);
    mapper->setCurrentIndex(current.row());
    QSqlRecord rec=sqlmodel3->record(current.row());
    if(rec.value("color").toString()!=""){//如果数据库中有颜色
        QColor c(rec.value("color").toString());
        QPalette p=ui->color->palette();
        p.setColor(QPalette::Window,c);
        ui->color->setAutoFillBackground(true);
        ui->color->setPalette(p);
    }
    else{//如果没有默认用黑色
        QPalette p=ui->color->palette();
        p.setColor(QPalette::Window,Qt::black);
        ui->color->setAutoFillBackground(true);
        ui->color->setPalette(p);
    }
}

void widget_coursemanager::on_btchangecolor_clicked()//修改颜色
{
    QColor c=QColorDialog::getColor(Qt::black,this,"选择课程背景颜色");//打开选颜色的窗口
    if(c.isValid()){
        QPalette p=ui->color->palette();
        p.setColor(QPalette::Window,c);
        ui->color->setPalette(p);
        //以下代码存在问题
        //原因：setrecord不适合单独修改一条记录中仅某一列的值，改用setdata
        // QSqlRecord rec=sqlmodel3->record(selection->currentIndex().row());
        // rec.setValue("color",c.name());
        // rec.setGenerated("color",false);
        // sqlmodel3->setRecord(selection->currentIndex().row(),rec);

        QModelIndex index=sqlmodel3->index(selection->currentIndex().row(),7);
        sqlmodel3->setData(index,c.name());
    }
}


void widget_coursemanager::on_bt_save_clicked()//点击保存按钮
{
    QModelIndex index=selection->currentIndex();//当前索引
    sqlmodel3->submitAll();
    selection->setCurrentIndex(index,QItemSelectionModel::Select);//
    mmodel->clear();

    emit wantloadtable();
    emit wantloadcourse(currentweek);//更新课表显示

}


void widget_coursemanager::on_bt_new_clicked()//点击新建课程按钮
{
    QSqlRecord rec=sqlmodel3->record();
    //配置默认值
    rec.setValue("no",sqlmodel3->record(sqlmodel3->rowCount()-1).value("no").toInt()+1);//最后一条记录的no值+1，永不重复
    rec.setValue("name","新课程");
    rec.setValue("weekfrom",1);
    rec.setValue("weekto",1);
    rec.setValue("workday_on","周一");
    rec.setValue("begin_at",1);
    rec.setValue("end_at",1);
    rec.setValue("color","#000001");
    rec.setValue("notice_before_min",0);

    sqlmodel3->insertRecord(sqlmodel3->rowCount(),rec);
    sqlmodel3->submitAll();

    QModelIndex index=sqlmodel3->index(sqlmodel3->rowCount()-1,1);
    selection->setCurrentIndex(index,QItemSelectionModel::Select);//选择新建的条目

}


void widget_coursemanager::on_bt_delete_clicked()//点击删除当前课程
{
    if(selection->hasSelection()){
        QModelIndex curindex=selection->currentIndex();//当前要删除的课程

        int row=selection->currentIndex().row();//当前索引的行号
        QString name=sqlmodel3->index(row,1).data().toString();
        sqlmodel3->removeRow(curindex.row());//删除当前行
        sqlmodel3->submitAll();
        //如果删除的是最后一节课，把同门课的所有课件记录也删除
        QSqlQueryModel *qrymodel=new QSqlQueryModel(this);
        qrymodel->setQuery("SELECT name from course where name = \""+name+"\"",db);
        if(qrymodel->rowCount()==0){
            QSqlQuery q;
            q.prepare("DELETE from coursefile where name =?");
            q.bindValue(0,name);
            q.exec();
        }
        if(sqlmodel3->rowCount()>0){//如果模型非空，就把选择光标移到上一行条目
            selection->setCurrentIndex(sqlmodel3->index(row-1,1),QItemSelectionModel::Select);//选择其之上一个项
        }

    }
    mmodel->clear();
    emit wantloadtable();
    emit wantloadcourse(currentweek);//更新课表显示
}


void widget_coursemanager::on_name_editingFinished()
{
    if(ui->name->text()=="其他"){
        ui->name->setText("新课程");
        QMessageBox::critical(this,"课程名称无效","不可使用 \"其他\" 作为课程名称");
    }
}

