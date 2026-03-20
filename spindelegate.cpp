#include "spindelegate.h"

SpinDelegate::SpinDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
    this->parent=parent;
}

QWidget *SpinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);
    QSpinBox *spinbox=new QSpinBox(parent);
    if(parent->findChild<QTableView*>("tv_course",Qt::FindDirectChildrenOnly)!=nullptr){
        spinbox->setMinimum(-1);
        spinbox->setMaximum(60);
    }
    else if(parent->findChild<QTableView*>("tv",Qt::FindDirectChildrenOnly)!=nullptr){
        spinbox->setMinimum(1);
        spinbox->setMaximum(50);
    }
    return spinbox;
}

void SpinDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int data=index.model()->data(index).toInt();
    QSpinBox *spinbox=static_cast<QSpinBox*>(editor);
    spinbox->setValue(data);
}

void SpinDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *spinbox=static_cast<QSpinBox*>(editor);
    int val=spinbox->value();
    model->setData(index,val);
    QSqlTableModel *sqlmodel=static_cast<QSqlTableModel*>(model);
    sqlmodel->submitAll();
    if(parent->findChild<QTableView*>("tv_course",Qt::FindDirectChildrenOnly)!=nullptr){
        const_cast<SpinDelegate*>(this)->dofresh();
    }
    else if(parent->findChild<QTableView*>("tv",Qt::FindDirectChildrenOnly)!=nullptr){
        const_cast<SpinDelegate*>(this)->dofresh_coursemanager();
    }
}

void SpinDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

void SpinDelegate::dofresh()
{
    emit fresh();
}

void SpinDelegate::dofresh_coursemanager()
{
    emit fresh_coursemanager();
}
