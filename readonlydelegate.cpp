#include "header/readonlydelegate.h"

ReadonlyDelegate::ReadonlyDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

QWidget *ReadonlyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return nullptr;
}
