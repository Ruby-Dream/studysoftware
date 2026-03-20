#include "comboboxdelegate.h"

ComboboxDelegate::ComboboxDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
    this->parent=parent;
}

QWidget *ComboboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);
    QComboBox *cb=new QComboBox(parent);
    QStringList list;
    list<<"周一"<<"周二"<<"周三"<<"周四"<<"周五"<<"周六 "<<"周日";
    cb->addItems(list);
    return cb;
}

void ComboboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString workday=index.model()->data(index).toString();
    QComboBox *cb=static_cast<QComboBox*>(editor);
    cb->setCurrentText(workday);
}

void ComboboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb=static_cast<QComboBox*>(editor);
    QString workday=cb->currentText();
    model->setData(index,workday);
    QSqlTableModel *sqlmodel=static_cast<QSqlTableModel*>(model);
    sqlmodel->submitAll();
    if(parent->findChild<QTableView*>("tv",Qt::FindDirectChildrenOnly)!=nullptr){
        const_cast<ComboboxDelegate*>(this)->dofresh_coursemanager();
    }
}

void ComboboxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

void ComboboxDelegate::dofresh_coursemanager()
{
    emit fresh_coursemanager();
}
