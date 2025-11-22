###
- This project is for personal learning and resume/portfolio purposes only.
- The source code was written/recreated by me as a learning exercise, based on course materials.
- It is not for commercial use, and the original content is not distributed.
- It serves as an example for learning C++, qt programming, and event handling.
- It is intended as a portfolio piece for C++/Qt application development

- 本项目仅供个人学习和简历展示使用  
- 源代码是我自己根据课程内容练习学习编写/复现  
- 不用于商业用途，也不对外传播原始内容
- 学习 Qt GUI 编程与事件处理的示例
- 作为 C++/Qt 桌面程序的作品集展示
###

Flappy Bird Mini-Game: C++17 / Qt6 (Widgets & Graphics View Framework) / CMake

Project Overview
 A recreation of the classic Flappy Bird game implemented using the Qt Graphics View framework. The game scene is built with QGraphicsScene, incorporating dynamic obstacles, gravity and collision detection mechanisms, while utilizing QTimer for the main game loop and animation updates.

Key Features & Technical Implementation:
 Scene Architecture Design: Game objects (bird, pillars, ground, etc.) are managed based on QGraphicsItem, with QGraphicsView handling rendering and scalable adaptation.

Modular Game Objects:
-BirdItem: A custom QGraphicsPixmapItem implementing gravity-based falling, jump logic, and animation effects.
-PillarItem: Obstacle class supporting timed movement, random height generation, and collision detection.
-Game Loop Control: Uses QTimer to manage frame updates and event processing, simulating physical motion and scene scrolling.
-Event Interaction: Responds to keyboard/mouse events to control the bird’s jump, with support for restart functionality and score tracking.
-Resource Management: Unified management of images and other resources via .qrc files, ensuring cross-platform compatibility and executable packaging.



Flappy Bird小游戏 : C++17 / Qt6 (Widgets & Graphics View Framework) / CMake

项目简介
 使用Qt Graphics View 框架实现经典Flappy Bird 游戏复刻版. 通过QGraphicsScene 构筑游戏场景,包括动态障碍物,重力与碰撞机制检测,并利用QTimer实现游戏主循环和动画刷新.

主要功能与技术实现： 
 -场景架构设计: 基于QGraphicsItem 管理游戏对象(bird,pillar,ground等), 通过QGraphicsView实现画面渲染与缩放适配.
 -游戏对象模块化: 
  BirdItem: 自定义 QGraphicsPixmapItem, 实现重力下落, 跳跃逻辑与动画效果.
  PillarItem: 障碍物类,支持定时移动,随机高度生成与碰撞检测.
 -游戏循环控制: 使用QTimer控制帧刷新与事件更新,模拟物理运动与场景滚动.
 -事件交互: 响应键盘/鼠标事件,控制bird角色跳跃,支持重新开始与分数统计功能.
 -资源管理: 通过 .qrc 统一管理图片等资源,实现跨平台和可执行文件打包.



