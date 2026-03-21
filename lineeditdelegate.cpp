#include "lineeditdelegate.h"

LineeditDelegate::LineeditDelegate(QObject *parent,bool isname)
    : QStyledItemDelegate{parent}
{
    this->parent=parent;
    this->isname=isname;
}

QWidget *LineeditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);
    QLineEdit *lineedit=new QLineEdit(parent);
    return lineedit;
}

void LineeditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str=index.model()->data(index).toString();
    old=index.model()->data(index).toString();
    QLineEdit *lineedit=static_cast<QLineEdit*>(editor);
    lineedit->setText(str);
}

void LineeditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineedit=static_cast<QLineEdit*>(editor);
    QString str=lineedit->text();
    if(isname && str=="其他"){
        QMessageBox::critical(nullptr,"课程名称无效","不可使用 \"其他\" 作为课程名称");
        str="新课程";
    }
    if(isname){
        const_cast<LineeditDelegate*>(this)->dofresh_sql(old,str);
    }
    model->setData(index,str);
    QSqlTableModel *sqlmodel=static_cast<QSqlTableModel*>(model);
    sqlmodel->submitAll();
    if(this->parent->findChild<QTableView*>("tv_person",Qt::FindDirectChildrenOnly)!=nullptr){
        const_cast<LineeditDelegate*>(this)->dofresh_personal();
    }
    else if(this->parent->findChild<QTableView*>("tv_course",Qt::FindDirectChildrenOnly)!=nullptr){
        const_cast<LineeditDelegate*>(this)->dofresh_course();
    }
    else if(this->parent->findChild<QTableView*>("tv",Qt::FindDirectChildrenOnly)!=nullptr){
        const_cast<LineeditDelegate*>(this)->dofresh_coursemanager();
    }
}

void LineeditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

void LineeditDelegate::dofresh_personal()
{
    emit fresh_personal();
}

void LineeditDelegate::dofresh_course()
{
    emit fresh_course();
}

void LineeditDelegate::dofresh_coursemanager()
{
    emit fresh_coursemanager();
}

void LineeditDelegate::dofresh_sql(QString old, QString now)
{
    emit fresh_sql(old,now);
}
