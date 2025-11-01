#ifndef PAINTCANVAS_H
#define PAINTCANVAS_H

#include <QWidget>

class PaintCanvas : public QWidget
{
    Q_OBJECT
public:
    enum ToolType
    {
        Pen,
        Rect,
        Ellipse,
        Eraser,
    };
    explicit PaintCanvas(QWidget *parent = nullptr);

    ToolType tool() const;
    void setTool(ToolType newTool);

    bool fill() const;
    void setFill(bool newFill);

    int penWidth() const;
    void setPenWidth(int newPenWidth);

    QColor fillColor() const;
    void setFillColor(const QColor &newFillColor);

    QColor penColor() const;
    void setPenColor(const QColor &newPenColor);

signals:

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void resizeImage(QImage *image, const QSize &newSize);

    void drawLineTo(const QPoint& endPoint);
    void drawRectTo(const QPoint& endPoint, bool ellipse = false);
    void eraseUnder(const QPoint& topleft);

private:
    ToolType m_tool{};
    bool m_fill{};
    bool m_drawing{};
    int m_penWidth{};

    QColor m_fillColor{};
    QColor m_penColor{};
    QPoint m_lastPoint{};
    QRectF m_lastRect{};
    QRectF m_lastEraserRect{};
    QImage m_image{};
};

#endif // PAINTCANVAS_H
