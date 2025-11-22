#include "persondelegate.h"
#include "personmodel.h"
#include <QComboBox>
#include <QPainter>

PersonDelegate::PersonDelegate(QObject *parent)
    :QStyledItemDelegate{parent}
{

}

/**
 * @brief PersonDelegate::paint 自定义单元格的绘图逻辑,
 * 将 editor 绘制成 当前 QComboBox 显示的颜色, 尺寸为当前当前单元格稍小一些
 * @param painter   QPainter 对象指针
 * @param option    样式选项
 * @param index     要绘制单元格(cell)的索引(index)
 */
void PersonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 2)
    {
        // hightlight selected cell
        // 如果当前 cell 被选中了，就执行高亮绘制
        if(option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
        }

        // get favite color
        QString favColor = index.data(PersonModel::FavoriteColorRole).toString();

        painter->save();

        painter->setBrush(QBrush(QColor(favColor)));

        // Draw color rect
        painter->drawRect(option.rect.adjusted(3, 3, -3, -3));
        // Text size
        QSize textSize = option.fontMetrics.size(Qt::TextSingleLine, favColor);
        painter->setBrush(QBrush(QColor(Qt::white)));

        // Adjustments for inner white rectangle
        int widthAdjust = (option.rect.width() - textSize.width()) / 2;
        int heightAdjust = (option.rect.height() - textSize.height()) / 2;
        painter->drawRect(option.rect.adjusted(widthAdjust, heightAdjust, -widthAdjust, -heightAdjust));

        //draw text
        painter->drawText(option.rect, favColor, Qt::AlignHCenter | Qt::AlignVCenter);

        painter->restore();
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

/**
 * @brief PersonDelegate::sizeHint 返回单元格推荐的大小
 * @param option 样式选项
 * @param index  单元格 index
 * @return QSize 推荐的 QSize 尺寸
 */
QSize PersonDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 2)
    {
        QStringList colorNames = QColor::colorNames();

        int maxLength = 0;
        for(const QString &color : colorNames)
        {
            int width = option.fontMetrics.horizontalAdvance(color);
            if(width > maxLength)
                maxLength = width;
        }
        maxLength += 40;

        int height = option.fontMetrics.height() + 10;

        return QSize(maxLength, height);
    }

    return QStyledItemDelegate::sizeHint(option, index).expandedTo(QSize(64, option.fontMetrics.height() + 10));
}

/**
 * @brief PersonDelegate::createEditor 创建用于编辑单元格内容的控件(widget)
 * @param parent 父控件
 * @param option 样式选项
 * @param index  单元格 index
 * @return QWidget 创建的编辑控件
 */
QWidget *PersonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 使用 QComboBox 绘制 favoriteColor 列
    if(index.column() == 2)
    {
        QComboBox *editor = new QComboBox(parent);

        foreach(QString color, QColor::colorNames())
        {
            QPixmap mPix(50, 50);
            mPix.fill(QColor(color));
            editor->addItem(QIcon(mPix), color);
        }

        return editor;
    }
    else
    {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

/**
 * @brief PersonDelegate::setEditorData 设置 Editor 的数据
 * @param editor 要设置的 Editor
 * @param index  源数据 model 的单元格索引
 */
void PersonDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // 根据 favoriteColor列 的数据 设置 QComboBox(editor) 的显示数据
    if(index.column() == 2)
    {
        QComboBox *combo = static_cast<QComboBox *>(editor);

        int idx = QColor::colorNames().indexOf(index.data(Qt::DisplayRole).toString());
        combo->setCurrentIndex(idx);
    }
}

/**
 * @brief PersonDelegate::setModelData 将 Editor(编辑器)中的数据写回 Model
 * @param editor 当前 Editor
 * @param model  要写回的 model
 * @param index  写回的 model 的 单元格索引
 */
void PersonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == 2)
    {
        QComboBox *combo = static_cast<QComboBox *>(editor);
        model->setData(index, combo->currentText(), PersonModel::FavoriteColorRole);
        // model->setData(index, editor->currentIndex(), Qt::EditRole); same using
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

/**
 * @brief PersonDelegate::updateEditorGeometry 控制 Editor在单元格中的位置和大小
 * @param editor 当前 Editor
 * @param option 样式选择
 * @param index 当前编辑的单元格(cell)的索引
 */
void PersonDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);

    // 把 editor 设置成 单元格(item)大小
    editor->setGeometry(option.rect);
}
