#include "paintcanvas.h"
#include <QPainter>
#include <QMouseEvent>

PaintCanvas::PaintCanvas(QWidget *parent)
    : QWidget{parent}
    ,m_tool(Pen)
    ,m_fill(false)
    ,m_drawing(false)
    ,m_penWidth(2)
    ,m_fillColor(Qt::red)
    ,m_penColor(Qt::green)
    ,m_lastEraserRect(QRectF(0,0,0,0))
    ,m_lastRect(QRectF(0,0,0,0))
{}

void PaintCanvas::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_lastPoint = event->pos();
        m_drawing = true;
    }
}

void PaintCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && m_drawing)
    {
        if(m_tool == Pen)
        {
            drawLineTo(event->pos());
        }

        if(m_tool == Rect)
        {
            drawRectTo(event->pos());
        }

        if(m_tool == Ellipse)
        {
            drawRectTo(event->pos(), true);
        }

        if(m_tool == Eraser)
        {
            eraseUnder(event->pos());
        }
    }

}

void PaintCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && m_drawing)
    {
        m_drawing = false;

        if(m_tool == Pen)
        {
            drawLineTo(event->pos());
        }

        if(m_tool == Rect)
        {
            drawRectTo(event->pos());
        }

        if(m_tool == Ellipse)
        {
            drawRectTo(event->pos(), true);
        }

        if(m_tool == Eraser)
        {
            eraseUnder(event->pos());
        }

        //reset the last rect
        m_lastRect = QRect(0,0,0,0);
    }
}

void PaintCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rectToDraw = event->rect();
    painter.drawImage(rectToDraw, m_image, rectToDraw);
}

void PaintCanvas::resizeEvent(QResizeEvent *event)
{
    if(width() > m_image.width() || height() > m_image.height())
    {
        int newWidth = qMax(width()+128 , m_image.width());
        int newHeight = qMax(height() + 128, m_image.height());
        resizeImage(&m_image, QSize(newWidth,newHeight));
        update();
    }

    QWidget::resizeEvent(event);
}

void PaintCanvas::resizeImage(QImage *image, const QSize &newSize)
{
    if(image->size() == newSize)
        return;

    QImage newImage(newSize,QImage::Format_RGB32);
    newImage.fill(qRgb(255,255,255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0,0), *image);
    *image = newImage;
}

void PaintCanvas::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&m_image);
    painter.setPen(QPen(m_penColor, m_penWidth,Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.drawLine(m_lastPoint, endPoint);
    int adjustment = m_penWidth + 2;

    // QImage
    update(QRect(m_lastPoint, endPoint).normalized().
           adjusted(-adjustment, -adjustment, +adjustment, +adjustment));
    m_lastPoint = endPoint;
}

void PaintCanvas::drawRectTo(const QPoint &endPoint, bool ellipse)
{
    QPainter painter(&m_image);
    painter.setPen(QPen(m_penColor, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    if(m_fill)
    {
        painter.setBrush(m_fillColor);
    }
    else
    {
        painter.setBrush(Qt::NoBrush);
    }

    if(!ellipse)
    {
        painter.drawRect(QRect(m_lastPoint, endPoint));
    }
    else
    {
        painter.drawEllipse(QRect(m_lastPoint, endPoint));
    }

    // 使用白色覆盖临时矩形(通过m_drawing 判断)
    if(m_drawing)
    {
        painter.setPen(QPen(Qt::white, m_penWidth + 2, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));

        if(m_fill)
        {
            painter.setBrush(Qt::white);
        }
        else
        {
            painter.setBrush(Qt::NoBrush);
        }

        if(!ellipse)
        {
            painter.drawRect(m_lastRect);
        }
        else
        {
            painter.drawEllipse(m_lastRect);
        }

        //reset the pen and brush
        painter.setPen(QPen(m_penColor, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.setBrush(m_fillColor);
    }

    // 这是绘制的矩形, 从上一记录点 到 结束点
    m_lastRect = QRectF(m_lastPoint, endPoint);
    update();
}

void PaintCanvas::eraseUnder(const QPoint &topLeft)
{
    QPainter painter(&m_image);

    //eraser last eraser rect
    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);
    painter.drawRect(m_lastEraserRect);

    // erase the content under current rect, bursh and pen still white
    QRect currentRect(topLeft, QSize(100, 100));
    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);
    painter.drawRect(currentRect);

    // draw current eraser rect
    painter.setBrush(Qt::black);
    painter.setPen(Qt::black);
    painter.drawRect(currentRect);

    m_lastEraserRect = currentRect;

    if(m_drawing)
    {
        painter.setBrush(Qt::white);
        painter.setPen(Qt::white);
        painter.drawRect(m_lastEraserRect);
        m_lastEraserRect = QRect(0,0,0,0);
    }

    update();
}

QColor PaintCanvas::penColor() const
{
    return m_penColor;
}

void PaintCanvas::setPenColor(const QColor &newPenColor)
{
    m_penColor = newPenColor;
}

QColor PaintCanvas::fillColor() const
{
    return m_fillColor;
}

void PaintCanvas::setFillColor(const QColor &newFillColor)
{
    m_fillColor = newFillColor;
}

int PaintCanvas::penWidth() const
{
    return m_penWidth;
}

void PaintCanvas::setPenWidth(int newPenWidth)
{
    m_penWidth = newPenWidth;
}


bool PaintCanvas::fill() const
{
    return m_fill;
}

void PaintCanvas::setFill(bool newFill)
{
    m_fill = newFill;
}

PaintCanvas::ToolType PaintCanvas::tool() const
{
    return m_tool;
}

void PaintCanvas::setTool(ToolType newTool)
{
    m_tool = newTool;
}
