#include "pillaritem.h"
#include "birditem.h"
#include "scene.h"
#include <QRandomGenerator>
#include <QGraphicsScene>

PillarItem::PillarItem()
    :m_topPillar(new QGraphicsPixmapItem(QPixmap(":/images/pillar.png")))
    ,m_bottomPillar(new QGraphicsPixmapItem(QPixmap(":/images/pillar.png")))
    ,m_pastBird(false)
{

    //创建上下两个障碍物柱子
    m_topPillar->setPos(QPointF(0,0) - QPointF(m_topPillar->boundingRect().width()/2, m_topPillar->boundingRect().height() + 60));
    m_bottomPillar->setPos(QPointF(0,0) + QPoint(-m_bottomPillar->boundingRect().width()/2, 60));

    // 组合成一个整体
    addToGroup(m_topPillar);
    addToGroup(m_bottomPillar);

    // 随机化障碍物整体位置
    m_yPos = QRandomGenerator::global()->bounded(150);
    int xRandomizer = QRandomGenerator::global()->bounded(200);

    // 设置初始位置
    setPos(QPoint(0,0) + QPoint(260 + xRandomizer, m_yPos));

    // 创建水平移动动画
    m_xAnimation = new QPropertyAnimation(this, "x", this );
    m_xAnimation->setStartValue(260 + xRandomizer);
    m_xAnimation->setEndValue(-260);
    m_xAnimation->setEasingCurve(QEasingCurve::Linear);
    m_xAnimation->setDuration(1500);

    // 绑定动画结束时回调
    connect(m_xAnimation, &QPropertyAnimation::finished, this, [=]()
            {
                qDebug() << "Animation finished";
                // 从 scene 中移除自身, 并释放内存
                scene()->removeItem(this);
                delete this;
            });

    m_xAnimation->start();

}

PillarItem::~PillarItem()
{
    qDebug() << "Deleting Pillar";
    // 手动释放上下障碍物
    delete m_topPillar;
    delete m_bottomPillar;
}

qreal PillarItem::x() const
{
    return m_x;
}

void PillarItem::freezeInPlace()
{
    // 停止障碍物动画
    m_xAnimation->stop();
}

void PillarItem::setX(qreal newX)
{
    if (qFuzzyCompare(m_x, newX))
        return;
    // qDebug() << "Pillar positon : " << m_x;
    m_x = newX;

    // 当障碍物x坐标通过中心->通过一个障碍物
    if(m_x < 0 && !m_pastBird)
    {
        m_pastBird = true;
        QGraphicsScene * mScene = scene();
        Scene * myScene = dynamic_cast<Scene *> (mScene);
        if(myScene)
        {
            myScene->incrementScore();
        }
    }

    if(collidesWithBird())
    {
        emit collideFail();
    }
    // 根据动画帧更新 m_x ，然后用 (m_x, m_yPos) 来更新整个对象在场景中的位置
    setPos(QPointF(0,0) + QPointF(m_x, m_yPos));
    emit xChanged();
}

bool PillarItem::collidesWithBird()
{
    // 获取 上下障碍物的所有碰撞项
    QList<QGraphicsItem *> collidingItems = m_topPillar->collidingItems();
    collidingItems.append(m_bottomPillar->collidingItems());

    foreach(QGraphicsItem * item, collidingItems)
    {
        BirdItem * birdItem = dynamic_cast<BirdItem *>(item);
        if(birdItem)
            return true;
    }
    return false;
}
