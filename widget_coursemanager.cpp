#include "widget_coursemanager.h"
#include "ui_widget_coursemanager.h"

widget_coursemanager::widget_coursemanager(QSqlDatabase db,QStandardItemModel *mmodel,QSqlTableModel *sqlmodel,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget_coursemanager)
{
    ui->setupUi(this);
    sqlmodel3=sqlmodel;
    this->mmodel=mmodel;
    this->db=db;
    opentable();

}

widget_coursemanager::~widget_coursemanager()
{
    emit wantsetenable();//让按钮恢复可用
    delete ui;
}

void widget_coursemanager::do_fresh_coursemanager()
{
    mmodel->clear();
    emit wantloadtable();//更新课表结构
    emit wantloadcourse();//更新课表显示
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

    lineedit_delegate1=new LineeditDelegate(ui->gb_tv,true);//改课程用代理
    lineedit_delegate2=new LineeditDelegate(ui->gb_tv,false);//改备注用代理
    combobox_delegate=new ComboboxDelegate(ui->gb_tv);
    spin_delegate=new SpinDelegate(ui->gb_tv);
    color_delegate=new ColorDelegate(ui->gb_tv);
    ui->tv->setItemDelegateForColumn(1,lineedit_delegate1);
    ui->tv->setItemDelegateForColumn(2,spin_delegate);
    ui->tv->setItemDelegateForColumn(3,spin_delegate);
    ui->tv->setItemDelegateForColumn(4,combobox_delegate);
    ui->tv->setItemDelegateForColumn(5,spin_delegate);
    ui->tv->setItemDelegateForColumn(6,spin_delegate);
    ui->tv->setItemDelegateForColumn(7,color_delegate);
    ui->tv->setItemDelegateForColumn(8,lineedit_delegate2);

    connect(color_delegate,&ColorDelegate::fresh_coursemanager,this,&widget_coursemanager::do_fresh_coursemanager);
    connect(spin_delegate,&SpinDelegate::fresh_coursemanager,this,&widget_coursemanager::do_fresh_coursemanager);
    connect(combobox_delegate,&ComboboxDelegate::fresh_coursemanager,this,&widget_coursemanager::do_fresh_coursemanager);
    connect(lineedit_delegate1,&LineeditDelegate::fresh_coursemanager,this,&widget_coursemanager::do_fresh_coursemanager);
    connect(lineedit_delegate2,&LineeditDelegate::fresh_coursemanager,this,&widget_coursemanager::do_fresh_coursemanager);
    connect(lineedit_delegate1,&LineeditDelegate::fresh_sql,this,&widget_coursemanager::sql);
    //绑定选择模型
    selection=new QItemSelectionModel(sqlmodel3,this);
    connect(selection,&QItemSelectionModel::currentRowChanged,this,&widget_coursemanager::do_currentRowChanged);
    //绑定模型视图
    ui->tv->setModel(sqlmodel3);
    ui->tv->setSelectionModel(selection);
    ui->tv->setColumnHidden(0,true);
    ui->tv->setColumnHidden(9,true);
    ui->tv->setColumnHidden(10,true);

}

void widget_coursemanager::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{//行变换时,首次进入也会触发
    Q_UNUSED(previous);
    Q_UNUSED(current);
    ui->bt_delete->setEnabled(true);
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
    rec.setValue("color","#000000");
    rec.setValue("notice_before_min",0);

    sqlmodel3->insertRecord(sqlmodel3->rowCount(),rec);
    sqlmodel3->submitAll();
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
    }
    mmodel->clear();
    emit wantloadtable();
    emit wantloadcourse();//更新课表显示
}

void widget_coursemanager::sql(QString old, QString now)//更新课程名时把课件条目同步更新
{
    QSqlQuery query;
    query.prepare("UPDATE coursefile set name =? where name = ?");
    query.bindValue(0,now);
    query.bindValue(1,old);
    query.exec();
}

