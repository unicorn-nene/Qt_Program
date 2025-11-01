#include "persondelegate.h"
#include "personmodel.h"
#include <QComboBox>
#include <QPainter>

PersonDelegate::PersonDelegate(QObject *parent)
    :QStyledItemDelegate{parent}
{}


/**
 * @brief PersonDelegate::createEditor 创建用于编辑单元格数据的 Widget
 * @param parent 编辑 widget 的 parent widget
 * @param option 单元格的样式
 * @param index 当前编辑的 Model 的索引 (可根据列不同,提供不同的Editor)
 * @return QWidget* 返回创建的自定义编辑 widget (Editor)
 */
QWidget *PersonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 2)
    {
        // set a custom editor widget
        QComboBox *editor = new QComboBox(parent);

        foreach(QString colorString, QColor::colorNames())
        {
            QPixmap pix(50, 50);
            pix.fill(QColor(colorString));
            editor->addItem(QIcon(pix), colorString);
        }

        return editor;
    }
    else
    {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}


/**
 * @brief PersonDelegate::setEditorData 从 Model 中提取数据并设置到 Editor 中
 * @param editor 用于编辑(颜色)的 widget
 * @param index 需要设置数据的 model 的索引, 用于获取对应的单元格
 */
void PersonDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // read the right data from the model and display that in the editor widget
    if(index.column() == 2)
    {
        QComboBox *combo = qobject_cast<QComboBox *> (editor);
        auto colorList = QColor::colorNames();
        auto currentColorString = index.data(Qt::DisplayRole).toString();
        auto idx = colorList.indexOf(currentColorString);
        combo->setCurrentIndex(idx);
    }
    else
    {
        return QStyledItemDelegate::setEditorData(editor, index);
    }
}

/**
 * @brief PersonDelegate::setModelData 将修改的 Editor 数据保存到 Model中
 * @param editor 当前用于编辑(颜色)的 Editor (widget)
 * @param model 要保存到的 Model
 * @param index 需要保存的数据的 model 索引, 用于获取对应的单元格
 */
void PersonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // read the data from the editor and write that back into the model
    if(index.column() == 2)
    {
        QComboBox *combo = qobject_cast<QComboBox *> (editor);
        model->setData(index, combo->currentText(), Qt::EditRole);
        // model->setData(index, combo->currentText(), PersonModel::FavoriteColorRole);
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

/**
 * @brief PersonDelegate::updateEditorGeometry 设置 Editor 在 view 中显示的位置与大小
 * @param editor 要设置的 Editor (widget)
 * @param option 单元格样式选项, 提供位置与大小信息
 * @param index 当前编辑操作的 model 索引, 用于获取对应的单元格
 */
void PersonDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // make sure that the editor widget is property sized and styled to fit blend in with the view
    editor->setGeometry(option.rect);
}

/**
 * @brief PersonDelegate::paint 自定义绘制单元格内容
 * @param painter 画家对象,用于绘制
 * @param option 绘制选项,包含当前单元格绘制的上下文信息
 * @param index 当前绘制操作的 Model 索引, 用于获取对应的单元格.
 */
void PersonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 2)
    {
        painter->save();

        // 先绘制选中背景
        if(option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());

        // 绘制颜色矩形
        QString favColor = index.data(PersonModel::FavoriteColorRole).toString();
        painter->setBrush(QBrush(QColor(favColor)));
        painter->setPen(Qt::NoPen);
        painter->drawRect(option.rect.adjusted(3, 3, -3, -3));

        // 文本居中
        QSize textSize = option.fontMetrics.size(Qt::TextSingleLine, favColor);
        int widthAdjust = (option.rect.width() - textSize.width()) / 2;
        int heightAdjust = (option.rect.height() - textSize.height()) / 2;

        painter->setBrush(Qt::white);
        painter->setPen(Qt::black);
        painter->drawRect(option.rect.adjusted(widthAdjust, heightAdjust, -widthAdjust, -heightAdjust));
        painter->drawText(option.rect, favColor, Qt::AlignHCenter | Qt::AlignVCenter);

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
QSize PersonDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index).expandedTo(QSize(64, option.fontMetrics.height() + 10));
}
