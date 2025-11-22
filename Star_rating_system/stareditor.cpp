#include "stareditor.h"
#include <QPainter>
#include <QMouseEvent>

StarEditor::StarEditor(QWidget *parent)
    : QWidget{parent}
{
    m_poly << QPoint(0, 85) << QPoint(75, 75)
    << QPoint(100, 10) << QPoint(125, 75)
    << QPoint(200, 85) << QPoint(150, 125)
    << QPoint(160, 190) << QPoint(100, 150)
    << QPoint(40, 190) << QPoint(50, 125)
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
 * @brief StarEditor::sizeHint 返回控件推荐的尺寸
 * @return QSize
 */
QSize StarEditor::sizeHint() const
{
    return QSize(100, 50);
}

/**
 * @brief StarEditor::mouseReleaseEvent 鼠标施放事件处理函数
 * 当鼠标释放时, 发出信号,表示用户完成评分操作
 * @param event 鼠标释放事件
 */
void StarEditor::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit editingFinished();
}

/**
 * @brief StarEditor::mouseMoveEvent 鼠标移动事件处理
 * 根据鼠标在 Widget 中的横向位置动态计算星级评分
 * 鼠标横向滑动实时更新评分(1~5)等级,并重新绘制 widget
 * @param event 鼠标移动事件
 */
void StarEditor::mouseMoveEvent(QMouseEvent *event)
{
    int rating = event->position().x() / 20;

    if((m_starRating != rating) && (rating < 6))
    {
        m_starRating = rating;
        update(); // emit -> paint();
    }
}

/**
 * @brief StarEditor::paintEvent 绘制事件处理函数
 * - 绘制绿色背景
 * - 绘制黄色星星
 * - 根据当前 starRating 绘制对应数目的 星星
 * @param event 绘制事件
 */
void StarEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.save();

    painter.setRenderHint(QPainter::Antialiasing, true);

    // green background
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(Qt::green)));
    painter.drawRect(rect());

    // yellow stars
    painter.setBrush(QBrush(QColor(Qt::yellow)));
    // move painter down a bit
    painter.translate(rect().x(), rect().y());
    painter.scale(0.1, 0.1);

    for(int i = 0 ; i < starRating(); ++i)
    {
        painter.drawPolygon(m_poly);
        painter.translate(220, 0);
    }
    painter.restore();
}
