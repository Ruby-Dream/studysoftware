#include "colordelegate.h"

ColorDelegate::ColorDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

QWidget *ColorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(parent);
    Q_UNUSED(option);
    Q_UNUSED(index);
    QLabel *lb=new QLabel(parent);
    return lb;

}

void ColorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(!hascolor){
        QLabel *lb=static_cast<QLabel*>(editor);
        QString old=index.data().toString();
        QColor c=QColorDialog::getColor(old,nullptr,"选择课程背景颜色");//打开选颜色的窗口
        if(c.isValid()){
            const_cast<ColorDelegate*>(this)->do_color();
            QString s=c.name();
            lb->setText(s);
        }
        else{
            lb->setText(old);
        }
    }
}

void ColorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(hascolor){
        QLabel *lb=static_cast<QLabel*>(editor);
        QString s=lb->text();
        model->setData(index,s);
        QSqlTableModel *sqlmodel=static_cast<QSqlTableModel*>(model);
        sqlmodel->submitAll();
        const_cast<ColorDelegate*>(this)->dofresh_coursemanager();
        const_cast<ColorDelegate*>(this)->do_color();
    }
}

void ColorDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

void ColorDelegate::dofresh_coursemanager()
{
    emit fresh_coursemanager();
}

void ColorDelegate::do_color()
{
    hascolor=!hascolor;
}
