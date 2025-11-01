#include "stareditor.h"
#include <QMouseEvent>
#include <QPainter>

StarEditor::StarEditor(QWidget *parent)
    : QWidget{parent}
{
    // If mouse tracking is enabled, the widget receives mouse move events even if no buttons are pressed.
    // 如果启用该功能, 则该 widget在没有按下鼠标也可以接受鼠标事件
    setMouseTracking(true);

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

int StarEditor::starRating() const
{
    return m_starRating;
}

void StarEditor::setStarRating(int newStarRating)
{
    m_starRating = newStarRating;
}

/**
 * @brief StarEditor::sizeHint 返回建议的该 Editor widget 显示的初始大小
 * @return QSize Editor Widget的尺寸
 */
QSize StarEditor::sizeHint() const
{
    return QSize(100, 50);
}

/**
 * @brief StarEditor::mouseReleaseEvent
 * 处理鼠标释放事件: 当鼠标松开时,认为用户已经确认评分选择
 * @param event 鼠标释放事件对象
 */
void StarEditor::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit editingFinished();
}

/**
 * @brief StarEditor::mouseMoveEvent
 * 处理鼠标移动事件: 当鼠标再控件内移动时, 根据鼠标的横坐标动态计算星级评分.
 * @param event 鼠标移动事件对象
 */
void StarEditor::mouseMoveEvent(QMouseEvent *event)
{
    int rating = static_cast<int>(event->position().x() / 20);

    // 更新评分并重新绘制(星星数量)
    if((rating != m_starRating) && (rating < 6))
    {
        m_starRating = rating;
        update();
    }
}

/**
 * @brief StarEditor::paintEvent 绘制星级评分widget
 * @param event 绘制事件对象(未使用)
 */
void StarEditor::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::green);

    // background rectangle green
    painter.setBrush(Qt::green);
    painter.drawRect(rect());

    // stars are yellow
    painter.setBrush(Qt::yellow);

    painter.translate(rect().x(), rect().y() + 5);
    painter.scale(0.1, 0.1);

    for(int i = 0; i < m_starRating; ++i)
    {
        painter.drawPolygon(m_poly);
        painter.translate(220, 0);   // next star
    }
}
