Flappy Bird小游戏 : C++17 / Qt6 (Widgets & Graphics View Framework) / CMake

 项目简介: 使用Qt Graphics View 框架实现经典Flappy Bird 游戏复刻版. 通过QGraphicsScene 构筑游戏场景,包括动态障碍物,重力与碰撞机制检测,并利用QTimer实现游戏主循环和动画刷新.

主要功能与技术实现： 
 -场景架构设计: 基于QGraphicsItem 管理游戏对象(bird,pillar,ground等), 通过QGraphicsView实现画面渲染与缩放适配.
 -游戏对象模块化: 
  BirdItem: 自定义 QGraphicsPixmapItem, 实现重力下落, 跳跃逻辑与动画效果.
  PillarItem: 障碍物类,支持定时移动,随机高度生成与碰撞检测.
 -游戏循环控制: 使用QTimer控制帧刷新与事件更新,模拟物理运动与场景滚动.
 -事件交互: 响应键盘/鼠标事件,控制bird角色跳跃,支持重新开始与分数统计功能.
 -资源管理: 通过 .qrc 统一管理图片等资源,实现跨平台和可执行文件打包.
