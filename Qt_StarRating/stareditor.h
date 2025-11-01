#ifndef STAREDITOR_H
#define STAREDITOR_H

#include <QWidget>

class StarEditor : public QWidget
{
    Q_OBJECT
public:
    explicit StarEditor(QWidget *parent = nullptr);

    int starRating() const;
    void setStarRating(int newStarRating);

    // QWidget interface
    QSize sizeHint() const override;

protected:
    // QWidget interface
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void editingFinished();

private:
    int m_starRating{1};
    QPolygon m_poly{};


};

#endif // STAREDITOR_H
