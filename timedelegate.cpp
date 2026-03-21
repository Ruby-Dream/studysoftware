#include "timedelegate.h"


TimeDelegate::TimeDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
    this->parent=parent;
}

QWidget *TimeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QTimeEdit *timeedit=new QTimeEdit(parent);
    return timeedit;
}

void TimeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString t=index.model()->data(index).toString();
    QTimeEdit *timeedit=static_cast<QTimeEdit*>(editor);
    QTime time=QTime::fromString(t,"HH:mm");
    timeedit->setTime(time);
}

void TimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTimeEdit *timeedit=static_cast<QTimeEdit*>(editor);
    QString t=timeedit->time().toString("HH:mm");
    model->setData(index,t);
    QSqlTableModel *sqlmodel=static_cast<QSqlTableModel*>(model);
    sqlmodel->submitAll();
    if(this->parent->findChild<QTableView*>("tv_person",Qt::FindDirectChildrenOnly)!=nullptr){
        const_cast<TimeDelegate*>(this)->dofresh();
    }
}

void TimeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

void TimeDelegate::dofresh()
{
    emit fresh();
}
