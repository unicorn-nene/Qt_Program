#include "stardelegate.h"
#include "stareditor.h"
#include  <QPainter>

StarDelegate::StarDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
    m_poly << QPoint(0, 85)
    << QPoint(75, 75)
    << QPoint(100, 10)
    << QPoint(125, 75)
    << QPoint(200, 85)
    << QPoint(150, 125)
    << QPoint(160, 190)
    << QPoint(100, 150)
    << QPoint(40, 190)
    << QPoint(50, 125)
    << QPoint(0, 85);
}


/**
 * @brief StarDelegate::paint 绘制自定义的评分星级逻辑
 * @param painter  用于绘制单元格内容的 painter 对象
 * @param option 单元格的样式和绘制区域信息
 * @param index 当前绘制的 model 索引, 表示对应的 model 索引
 */
void StarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 3)
    {
        QRect rect = option.rect.adjusted(10, 10, -10, -10);
        int starNumber = index.data(Qt::DisplayRole).toInt();

        painter->save();

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);

        painter->translate(rect.x(), rect.y());
        painter->scale(0.1, 0.1);

        for(int i = 0 ; i < starNumber; ++i)
        {
            painter->drawPolygon(m_poly);
            painter->translate(220, 0);
        }

        painter->restore();
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}


/**
 * @brief PersonDelegate::sizeHint 提供建议的单元格大小, 使得表格能够合理布局
 * @param option 样式选项, 提供相关显示信息
 * @param index 需要设置的 Model 索引, 用于获取对应的单元格.
 * @return QSize 返回的推荐的单元格尺寸(size).
 */
QSize StarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  return  QStyledItemDelegate::sizeHint(option, index);
}

/**
 * @brief StarDelegate::createEditor 创建用于绘制星级图形的 Editor widget
 * @param parent perent widget
 * @param option 样式选项
 * @param index 要设置的 model 索引, 单元格所在的位置
 * @return QWidget* 自定义的 StarEditor(widget)
 */
QWidget *StarDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 3)
    {
        int starRating = index.data(Qt::DisplayRole).toInt();
        StarEditor *editor = new StarEditor(parent);
        editor->setStarRating(starRating);

        // Make the connection to respond when editing is finished

        return editor;
    }
    else
    {
      return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

/**
 * @brief StarDelegate::setEditorData 将 model 中的数据设置到编辑 Widget(StarEditor) 中
 * @param editor 要设置数据的 (Star)Editor
 * @param index 源数据的 model 索引, 单元格所在的位置
 */
void StarDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == 3)
    {
        int starRating = index.data().toInt();
        StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
        starEditor->setStarRating(starRating);
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

/**
 * @brief StarDelegate::setModelData 将 editor 中的数据更新回 Model
 * @param editor 源数据的 editor
 * @param model 目标更新的 model
 * @param index 要更新的 model 的索引, 对应的单元格
 */
void StarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == 3)
    {
        StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
        model->setData(index, QVariant::fromValue(starEditor->starRating()), Qt::EditRole);
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

/**
 * @brief StarDelegate::commitAndCloseEditor
 */
void StarDelegate::commitAndCloseEditor()
{
    StarEditor *editor = qobject_cast<StarEditor *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
