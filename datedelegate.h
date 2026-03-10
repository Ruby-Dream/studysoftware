#ifndef DATEDELEGATE_H
#define DATEDELEGATE_H

#include <QStyledItemDelegate>
#include <QDateEdit>
#include <QSqlTableModel>
class DateDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DateDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)const; //创建代理
    void setEditorData(QWidget *editor, const QModelIndex &index)const; //设置代理数据
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)const; //更新模型数据
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index)const;
};

#endif // DATEDELEGATE_H
