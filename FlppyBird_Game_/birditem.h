#ifndef BIRDITEM_H
#define BIRDITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class BirdItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged FINAL)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged FINAL)

public:
    explicit BirdItem(QPixmap pixmap);

    qreal rotation() const;
    qreal y() const;

    void shootUp();
    void startFlying();

    void freezeInPlace();

signals:
    void rotationChanged();
    void yChanged();

public slots:
      void setRotation(qreal newRotation);
      void setY(qreal newY);

      void rotateTo(const qreal &end, const int& duration, const QEasingCurve & curve);

      void fallToGroundIfNecessary();
private:
    void updatePixmap();

private:
    enum WingPosition
    {
        Up,
        Middle,
        Down,
    };

    WingPosition m_wingPosition{};                  // 表示当前 bird 翅膀的状态
    bool m_wingDirection{};                         // 表示翅膀运动的方向 0: down, 1: up

    qreal m_rotation{};                             // 当前bird的选择角度, 动画捆绑属性
    qreal m_y{};                                    // 当前bird在 scene 中的y坐标, 动画捆绑属性

    QPropertyAnimation * m_yAnimation{};            // 控制bird垂直位置的动画
    QPropertyAnimation * m_rotationAnimation{};     // 控制bird选择角度的动画(飞行姿态)

    qreal m_groundPosition{};                       // 地面 y 坐标, 用于判断地面位置
};

#endif // BIRDITEM_H
