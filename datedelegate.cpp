#include "datedelegate.h"

DateDelegate::DateDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
    this->parent=parent;
}

QWidget *DateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QDateEdit *dateedit=new QDateEdit(parent);
    return dateedit;
}

void DateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString date=index.model()->data(index).toString();
    QDateEdit *dateedit=static_cast<QDateEdit*>(editor);
    QDate d=QDate::fromString(date,"yyyy-MM-dd");
    dateedit->setDate(d);
}

void DateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateEdit *dateedit=static_cast<QDateEdit*>(editor);
    QString date=dateedit->date().toString("yyyy-MM-dd");
    model->setData(index,date);

    QSqlTableModel *sqlmodel=static_cast<QSqlTableModel*>(model);
    sqlmodel->submitAll();
    if(parent->findChild<QTableView*>("tv_person",Qt::FindDirectChildrenOnly)!=nullptr){
        const_cast<DateDelegate*>(this)->dofresh();
    }
}

void DateDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

void DateDelegate::dofresh()
{
    emit fresh();
}
