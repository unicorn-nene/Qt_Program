#ifndef PILLARITEM_H
#define PILLARITEM_H

#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class PillarItem :public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged FINAL)
public:
    explicit PillarItem();
    ~PillarItem();

    qreal x() const;

    void freezeInPlace(); // 暂停柱子动画

public slots:
    void setX(qreal newX);

signals:

    void xChanged();

    void collideFail();

private:
    bool collidesWithBird();

private:
    // 图形元素
    QGraphicsPixmapItem * m_topPillar{};        // 上方柱子的图形对象
    QGraphicsPixmapItem * m_bottomPillar{};     // 下方柱子的图形对象
    QPropertyAnimation * m_xAnimation{};        // 控制整组柱子沿着 x 方向运动的动画

    // 动画相关
    int m_yPos{};   // 柱子组的垂直偏移量
    qreal m_x{};    // 当前柱子组的x坐标, 用于动画绑定属性

    // 游戏逻辑
    bool m_pastBird{}; // 标记柱子是否已经"经过"小鸟,避免重复加分
};

#endif // PILLARITEM_H
