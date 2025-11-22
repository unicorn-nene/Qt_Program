#include "birditem.h"
#include <QTimer>
#include <QGraphicsScene>

/**
 * @brief BirdItem::BirdItem
 * 构造函数，初始化 BirdItem 对象
 * @param pixmap 初始化显示的鸟图片
 */
BirdItem::BirdItem(QPixmap pixmap)
    :m_wingPosition(WingPosition::Up)
    ,m_wingDirection(0)
{
    // 设置初始 pixmap
    setPixmap(pixmap);

    // 启动一个定时器 更新bird翅膀动画
    QTimer * birdWingsTimer = new QTimer(this);
    connect(birdWingsTimer, &QTimer::timeout,this, [=]()
            {
                updatePixmap();
            });

    birdWingsTimer->start(80);

    // dowm animation
    // 初始化下落动画
    m_groundPosition = scenePos().y() +290;

    m_yAnimation = new QPropertyAnimation(this, "y", this);
    m_yAnimation->setStartValue(scenePos().y());
    m_yAnimation->setEndValue(m_groundPosition);
    m_yAnimation->setEasingCurve(QEasingCurve::InQuad);
    m_yAnimation->setDuration(1000) ;

    // m_yAnimation->start();

    // rotation animation 初始化旋转动画
    m_rotationAnimation = new QPropertyAnimation(this, "rotation", this);
    // rotateTo(90, 1200, QEasingCurve::InQuad);
}

/**
 * @brief BirdItem::updatePixmap
 * 更新 bird 翅膀的动画帧, 根据当前翅膀位置切换 pixmap
 */
void BirdItem::updatePixmap()
{
    if(m_wingPosition == WingPosition::Middle)
    {
        //  中间位置 -> 上 / 下 位置
        if(m_wingDirection)
        {
            //up
            setPixmap(QPixmap(":/images/bird_blue_up.png"));
            m_wingPosition = WingPosition::Up;
            m_wingDirection = 0;
        }
        else
        {
            //Down
            setPixmap(QPixmap(":/images/bird_blue_down.png"));
            m_wingPosition = WingPosition::Down;
            m_wingDirection = 1;
        }}
    else    // 上/下位置 -> 中间位置
    {
        setPixmap(QPixmap(":/images/bird_blue_middle.png"));
        m_wingPosition = WingPosition::Middle;
    }
}

/**
 * @brief BirdItem::rotation
 * 获取 bird 的当前旋转角度
 * @return bird 的当前旋转角度
 */
qreal BirdItem::rotation() const
{
    return m_rotation;
}


/**
 * @brief BirdItem::setRotation
 * 设置 bird 旋转角度
 * @param newRotation
 */
void BirdItem::setRotation(qreal newRotation)
{
    if (qFuzzyCompare(m_rotation, newRotation))
        return;
    m_rotation = newRotation;

    //bird totation
    QPointF c = boundingRect().center();

    QTransform t;
    t.translate(c.x(), c.y());      // 设置转换的中心点(平移,旋转等)
    t.rotate(m_rotation);           // 设置操作矩阵
    t.translate(-c.x(), -c.y());    // 恢复转换中心
    setTransform(t);

    emit rotationChanged();
}

/**
 * @brief BirdItem::y
 * @return Bird 的 y 坐标(动画属性)
 */
qreal BirdItem::y() const
{
    return m_y;
}

/**
 * @brief BirdItem::shootUp
 * 执行 bird 飞行动作动画
 */
void BirdItem::shootUp()
{
    // 停止当前所有动画
    m_yAnimation->stop();
    m_rotationAnimation->stop();

    // 配置新 的 m_yAniamtion : 当前 y 坐标减去 scene 高度的 1/8
    qreal curPosY = y();

    m_yAnimation->setStartValue(curPosY);
    m_yAnimation->setEndValue(curPosY - scene()->sceneRect().height() / 8);
    m_yAnimation->setEasingCurve(QEasingCurve::OutQuad);
    m_yAnimation->setDuration(285);

    // 上升飞行动画结束后 开始下落动画
    connect(m_yAnimation, &QPropertyAnimation::finished, this, [=]()
            {
                fallToGroundIfNecessary();
            });

    m_yAnimation->start();

    // 调用旋转动画,调整bird抬头
    rotateTo(-20, 200, QEasingCurve::OutCubic);
}

/**
 * @brief BirdItem::startFlying
 * 启动 bird 自由飞行动画
 */
void BirdItem::startFlying()
{
    // 启动下落动画
    m_yAnimation->start();

    // 启动旋转动画
    rotateTo(-20, 200, QEasingCurve::OutCubic);
}

/**
 * @brief BirdItem::freezeInPlace
 * 暂停 bird 的当前动画
 */
void BirdItem::freezeInPlace()
{
    // 停止下落动画
    m_yAnimation->stop();
    // 停止旋转动画
    m_rotationAnimation->stop();
}

/**
 * @brief BirdItem::setY
 * 设置 bird 的 Y 属性(动画绑定属性)
 * @param newY 修改的 Y 值
 */
void BirdItem::setY(qreal newY)
{
    if (qFuzzyCompare(m_y, newY))
        return;
    moveBy(0, newY - m_y);
    m_y = newY;
    //setPos(QPointF(0, 0) + QPointF(0, m_y));

    emit yChanged();
}

/**
 * @brief BirdItem::rotateTo  使 bird 旋转 指定角度
 * @param end 目标旋转角度
 * @param duration 动画持续时间
 * @param curve 动画曲线类型
 */
void BirdItem::rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve)
{
    // 配置 m_rotationAnimation
    m_rotationAnimation->setStartValue(rotation());
    m_rotationAnimation->setEndValue(end);
    m_rotationAnimation->setEasingCurve(curve);
    m_rotationAnimation->setDuration(duration);

    m_rotationAnimation->start();
}

/**
 * @brief BirdItem::fallToGroundIfNecessary
 * 当 bird 未接触地面时, bird 保持自由下落动画
 */
void BirdItem::fallToGroundIfNecessary()
{
    // 判断当前 y 坐标 是否低于地面
    if( y() < m_groundPosition)
    {
        // 如果低于地面,bird 不再旋转,只保持头方向向下下落
        // 停止旋转动画
        m_rotationAnimation->stop();
        // m_yAnimation->stop();

        // 启动下落动画
        m_yAnimation->setStartValue(y());
        m_yAnimation->setEasingCurve(QEasingCurve::InQuad);
        m_yAnimation->setEndValue(m_groundPosition);
        m_yAnimation->setDuration(1200);
        m_yAnimation->start();

        rotateTo(90, 1100, QEasingCurve::InCubic);
    }
}
