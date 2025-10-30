#include "scene.h"
#include "pillaritem.h"
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

/**
 * @brief Scene::Scene
 * @param parent
 */
Scene::Scene(QObject *parent)
    : QGraphicsScene{parent}
    ,m_gameOn(false)
    ,m_score(0)
    ,m_bestScore(0)
{
    setUpPillarTimer();

}

/**
 * @brief Scene::~Scene
 */
Scene::~Scene()
{
    // 析构时确保清理UI资源
    hideGameOverGraphics();
    cleanPillars();
}

/**
 * @brief Scene::addBird
 * 添加 bird 对象到 scene 中
 */
void Scene::addBird()
{
    // 添加 bird 到场景中央
    m_birdPtr = new BirdItem(QPixmap(":/images/bird_blue_up.png"));
    m_birdPtr->setTransformOriginPoint(m_birdPtr->boundingRect().center());
    m_birdPtr->setPos(QPointF(0,0) - QPointF(m_birdPtr->boundingRect().width()/2.0f, m_birdPtr->boundingRect().height()/2.0f));
     //m_birdPtr->setZValue(1);
    addItem(m_birdPtr);
}

/**
 * @brief Scene::startGame
 * 启动 Game
 */
void Scene::startGame()
{
    // bird start flying
    m_birdPtr->startFlying();

    // Pillars generate
    if(!m_pillarTimer->isActive())
    {
        cleanPillars();
        setGameOn(true);
        setScore(0);
        hideGameOverGraphics();
        m_pillarTimer->start(1000); // 每秒生成一个柱子障碍物
    }
}

/**
 * @brief Scene::keyPressEvent
 * 按键检测, 触发 bird 飞行
 * @param event QKeyEvent 键盘事件
 */
void Scene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
        if(m_gameOn)
            m_birdPtr->shootUp();
    }

    QGraphicsScene::keyPressEvent(event);
}

/**
 * @brief Scene::mousePressEvent
 * 鼠标检测, 触发 Bird 飞行
 * @param event QGraphicsSceneMouseEvent鼠标事件
 */
void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(m_gameOn)
            m_birdPtr->shootUp();
    }

    QGraphicsScene::mousePressEvent(event);
}

/**
 * @brief Scene::showGameOverGraphics
 * 当游戏结束时, 显示结束图像
 */
void Scene::showGameOverGraphics()
{
    // 添加 gameOver 图片对象并设置在中心位置
    m_gameOverPix = new QGraphicsPixmapItem(QPixmap(":/images/game_over_red.png"));
    addItem(m_gameOverPix);
    m_gameOverPix->setPos(QPointF(0.0, 0.0) - QPointF(m_gameOverPix->boundingRect().width() / 2.0f, m_gameOverPix->boundingRect().height()/2.0f));

    // 设置得分文本并设置在 GameOver 下方
    m_scoreTextItem = new QGraphicsTextItem();
    QString htmlString = "<p> Score : " + QString::number(m_score) + "</p>" + "<p> Beat Score : " + QString::number(m_bestScore) + "</p>";
    QFont mFont("Consoles", 30, QFont::Bold);
    m_scoreTextItem->setHtml(htmlString);
    m_scoreTextItem->setFont(mFont);
    m_scoreTextItem->setDefaultTextColor(Qt::green);

    addItem(m_scoreTextItem);
    m_scoreTextItem->setPos(QPointF(0.0f, 0.0f) - QPointF(m_scoreTextItem->boundingRect().width() / 2.0f, -m_gameOverPix->boundingRect().height() / 2.0f));
}

/**
 * @brief Scene::hideGameOverGraphics
 * 隐藏 GameOver 结算图像
 */
void Scene::hideGameOverGraphics()
{
    // 如果 GameOver 图片对象存在,则移除对象并清理指针内存
    if(m_gameOverPix)
    {
        removeItem(m_gameOverPix);
        delete m_gameOverPix;
        m_gameOverPix = nullptr;
    }

    // 如果得分文本存在,则移除对象并清理指针内存
    if(m_scoreTextItem)
    {
        removeItem(m_scoreTextItem);
        delete m_scoreTextItem;
        m_scoreTextItem = nullptr;
    }
}

/**
 * @brief Scene::setUpPillarTimer
 * 设置 游戏中的障碍物
 */
void Scene::setUpPillarTimer()
{
    // 设置定时器
    m_pillarTimer = new QTimer(this);
    // 当定时结束时,检测是否 bird 与 柱子障碍物碰撞
    connect(m_pillarTimer, &QTimer::timeout, this, [=]()
            {
                PillarItem * pillarItem = new PillarItem();
        // 如果产生碰撞,游戏结束(停止生成柱子障碍物, 冻结bird 和柱子障碍物动画,停止游戏,展示游戏结束结算图像)
                connect(pillarItem, &PillarItem::collideFail, this, [=]()
                        {
                            m_pillarTimer->stop();
                            freezeBirdAndPillarsInPlace();
                            setGameOn(false);
                            showGameOverGraphics();
                        });
                addItem(pillarItem);
            });

    m_pillarTimer->start(1000);
}

/**
 * @brief Scene::freezeBirdAndPillarsInPlace
 * 暂停游戏场景
 */
void Scene::freezeBirdAndPillarsInPlace()
{
    // Freeze bird
    m_birdPtr->freezeInPlace();

    // Freeze pillars
    QList<QGraphicsItem *> sceneItems = items();

    // 遍历 scene 中的 item, 冻结所有柱子的item
    foreach(QGraphicsItem *item, sceneItems)
    {
        PillarItem * pillar = dynamic_cast<PillarItem *> (item);
        if(pillar)
        {
            pillar->freezeInPlace();
        }
    }
}

bool Scene::gameOn() const
{
    return m_gameOn;
}

void Scene::setGameOn(bool newGameOn)
{
    m_gameOn = newGameOn;
}

/**
 * @brief Scene::cleanPillars
 * 清理 障碍物 对象
 */
void Scene::cleanPillars()
{
    // 遍历所有 scene 中的 item, 移除所有是 pillarItem 的 item
    QList<QGraphicsItem *> sceneItems = items();
    foreach(QGraphicsItem * item, sceneItems)
    {
        PillarItem * pillar = dynamic_cast<PillarItem *> (item);
        if(pillar)
        {
            removeItem(pillar);
            delete pillar;
        }
    }
}

int Scene::score() const
{
    return m_score;
}

void Scene::setScore(int newScore)
{
    m_score = newScore;
}

/**
 * @brief Scene::incrementScore
 * 计算游戏得分, 没通过一个柱子,增加分数
 */
void Scene::incrementScore()
{
    m_score++;
    if(m_score > m_bestScore)
        m_bestScore = m_score;

    qDebug() << "Score : " << m_score << " Best Score : " << m_bestScore;
}
