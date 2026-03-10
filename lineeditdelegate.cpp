#include "lineeditdelegate.h"

LineeditDelegate::LineeditDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

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
    QLineEdit *lineedit=static_cast<QLineEdit*>(editor);
    lineedit->setText(str);
}

void LineeditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineedit=static_cast<QLineEdit*>(editor);
    QString str=lineedit->text();
    model->setData(index,str);
    QSqlTableModel *sqlmodel=static_cast<QSqlTableModel*>(model);
    sqlmodel->submitAll();
}

void LineeditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
