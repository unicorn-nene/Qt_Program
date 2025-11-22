#include "stardelegate.h"
#include <QPainter>
#include "stareditor.h"

StarDelegate::StarDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
    //draw star ploy
    m_poly << QPoint(0, 85) << QPoint(75, 75)
           << QPoint(100, 10) << QPoint(125, 75)
           << QPoint(200, 85) << QPoint(150, 125)
           << QPoint(160, 190) << QPoint(100, 150)
           << QPoint(40, 190) << QPoint(50, 125)
           << QPoint(0, 85);
}

/**
 * @brief StarDelegate::paint 自定义绘制处理函数, 绘制评分列
 * - 读取评分 Rating
 * - 根据 Rating 绘制对应数量的 星星
 * @param painter 绘制 widget
 * @param option  cell(单元格) 样式
 * @param index   cell 的 index
 */
void StarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 评分列在 第四列
    if(index.column() == 3)
    {
        int starNumber = index.data().toInt();

        painter->save();

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(QColor(Qt::black)));

        // 计算星星整体区域大小
        QRect rect = option.rect;
        const int starSpacing = 22;  // 星星间距
        const int starSize = 20;     // 星星大小
        const int totalWidth = starNumber * starSize + (starNumber - 1) * 2;
        // const int offsetX = rect.x() + (rect.width() - totalWidth) / 2;
        // const int offsetY = rect.y() + (rect.height() - starSize) / 2;

        // 移动坐标到单元格中心区域
        painter->translate(rect.x(), rect. y());
        painter->scale(0.1, 0.1); // 与 StarEditor 保持一致

        for(int i = 0; i < starNumber; ++i)
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
 * @brief StarDelegate::sizeHint 返回推荐的cell尺寸
 * @param option 样式选项
 * @param index 在 Model 中的 index
 * @return QSize
 */
QSize StarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

/**
 * @brief StarDelegate::createEditor 创建自定义编辑控件(Editor)
 * - 在评分列创建自定义 StarEditor
 * - 同步与 model 的数据(星级数据)
 * @param parent 父对象(父控件)
 * @param option 样式选项
 * @param index 在 Model 中的 index
 * @return 自定义的 Editor
 */
QWidget *StarDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 3)
    {
        int starRating = index.data().toInt();

        StarEditor *editor = new StarEditor(parent);
        editor->setStarRating(starRating);

        connect(editor, &StarEditor::editingFinished, this, &StarDelegate::commitAndCloseEditor);

        return editor;
    }
    else
    {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

/**
 * @brief StarDelegate::setEditorData
 * 将 Model 中的数据传递给 Editor, 确保 Editor 显示的数据正确
 * @param editor 对应的 Editor
 * @param index 在 Model 的 cell index
 */
void StarDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == 3)
    {
        int startRating = index.data().toInt();
        StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
        starEditor->setStarRating(startRating);
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

/**
 * @brief StarDelegate::setModelData 将 Editor 修改后的值写回 Model
 * @param editor 对应的 Editor
 * @param model 要写回的 Model
 * @param index 要写回的 cell index
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

void StarDelegate::commitAndCloseEditor()
{
    StarEditor *editor = qobject_cast<StarEditor *>(sender());

    emit commitData(editor);
    emit closeEditor(editor);
}
