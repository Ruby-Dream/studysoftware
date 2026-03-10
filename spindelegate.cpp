#include "spindelegate.h"

SpinDelegate::SpinDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

QWidget *SpinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);
    QSpinBox *spinbox=new QSpinBox(parent);
    spinbox->setMinimum(0);
    spinbox->setMaximum(60);
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
}

void SpinDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
