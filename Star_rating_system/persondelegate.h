#ifndef PERSONDELEGATE_H
#define PERSONDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

/**
 * @brief The PersonDelegate class
 * 自定义委托类，用于控制表格（QTableView）或列表（QListView）中单元格的绘制与编辑行为
 */
class PersonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    PersonDelegate(QObject *parent = nullptr);

    // QAbstractItemDelegate interface
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
#endif // PERSONDELEGATE_H
