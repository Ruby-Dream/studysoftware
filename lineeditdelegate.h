#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QSqlTableModel>
#include <QTableView>

class LineeditDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit LineeditDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)const; //创建代理
    void setEditorData(QWidget *editor, const QModelIndex &index)const; //设置代理数据
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)const; //更新模型数据
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index)const;
    void dofresh_personal();
    void dofresh_course();
    void dofresh_coursemanager();
private:
    QObject *parent;
signals:
    void fresh_personal();
    void fresh_course();
    void fresh_coursemanager();
};

#endif // LINEEDITDELEGATE_H
