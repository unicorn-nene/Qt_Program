#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include "birditem.h"

// 游戏场景: 负责整体游戏逻辑的控制
// -管理小鸟与柱子
// -处理游戏状态(开始,结束,得分)
// -响应键盘和鼠标操作(事件)
class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    ~Scene() override;
    void addBird();                 // 添加一直 bird 到场景中心

    void startGame();               // 开始游戏 : 重置分数,隐藏 GameOver 图形,启动柱子障碍物计数器
    bool gameOn() const;
    void setGameOn(bool newGameOn);

    // 分数管理
    void incrementScore();          // 分数增加, 并更新记录最高分
    int score() const;
    void setScore(int newScore);

    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event) override;                      // 键盘事件: 按下 space 让 bird 飞行一次
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;     // 鼠标事件: 按下鼠标左键让 brid 飞行一次

private:
    void showGameOverGraphics();        // 显示 GameOver 图形和得分文本
    void hideGameOverGraphics();        // 隐藏并删除 GameOver 图形

    void setUpPillarTimer();            // 初始化并设置柱子障碍物定时器(定时生成柱子障碍物)
    void freezeBirdAndPillarsInPlace(); // 暂停场景中所有动态元素(brid 和 柱子障碍物)
    void cleanPillars();                // 清楚场景中的所有柱子障碍物

private:
    //游戏对象
    QTimer *m_pillarTimer{}; // 定时生成柱子障碍物的定时器
    BirdItem *m_birdPtr{};   // 小鸟对象指针

    // 游戏状态
    bool m_gameOn{};        // 当前是否在游戏中
    int m_score{};          // 当前得分
    int m_bestScore{};      // 历史最高分

    // 界面元素
    QGraphicsPixmapItem *m_gameOverPix{};   // GameOver 图片
    QGraphicsTextItem *m_scoreTextItem{};   // 显示分数的文本
};

#endif // SCENE_H
