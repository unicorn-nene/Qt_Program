###
- This project is for personal learning and resume/portfolio purposes only.
- The source code was written/recreated by me as a learning exercise, based on course materials.
- It is not for commercial use, and the original content is not distributed.
- It serves as an example for learning C++, qt programming, and event handling.
- It is intended as a portfolio piece for C++ desktop application development

- 本项目仅供个人学习和简历展示使用  
- 源代码是我自己根据课程内容练习学习编写/复现  
- 不用于商业用途，也不对外传播原始内容
- 学习 C++ 和 qt 编程与事件处理的示例
- 作为 C++ 桌面程序的作品集展示
###

PaintCanvas - Qt Drawing Application

Overview

PaintCanvas is a simple desktop drawing application developed using Qt 6 and C++. 
It allows users to draw freehand lines, rectangles, ellipses, and erase parts of the drawing.
Users can customize the pen width, pen color, and fill color for shapes.
The application is designed as a lightweight canvas, suitable for educational or demonstration purposes.

Features:
 - Drawing Tools: Freehand pen, Rectangle, Ellipse, Eraser.
 - Customizable properties: Pen color, Fill color, Pen width, Fill on/off for shapes.
 - Resizable canvas: The drawing area expands dynamically when the window is resized.
 - Undo-like behavior for shapes and eraser: Temporary white overlay is used to update shapes dynamically while drawing.
 - Simple and intuitive toolbar with icons for tool selection and color/width customization.

Implementation Details
 - Main Classes:PaintCanvas (inherits from QWidget): Handles painting, mouse events, drawing logic, and canvas resizing.
 - MainWindow (inherits from QMainWindow): Provides the main window, toolbar, and UI elements for tool selection and color/width customization.

Key Concepts:
 - Mouse press, move, and release events are used to control drawing.
 - QPainter is used for rendering on a QImage which acts as the canvas.
 - Dynamic resizing of QImage ensures no loss of existing drawings.
 - Tools and properties are updated via signals and slots mechanism in Qt.

UI:
Toolbar contains controls for pen width, pen color, fill color, fill toggle, and tool buttons with icons.
Status bar shows current selected tool.



项目概述

PaintCanvas 是一款使用 Qt 6 与 C++ 开发的简易桌面绘图程序。用户可以在画布上绘制自由线条、矩形、椭圆，并使用橡皮擦擦除部分内容。
程序支持自定义画笔宽度、画笔颜色和形状填充颜色。设计轻量，适合作为学习、演示或简历项目展示。

功能特性:
 - 绘图工具：自由画笔, 矩形, 椭圆, 橡皮擦
 - 可定制属性：画笔颜色,填充颜色,画笔宽度,是否填充形状
 - 可动态调整画布大小：窗口大小变化时，画布自动扩展，不丢失已有内容。
 - 绘图和橡皮擦的动态更新：绘制过程中使用临时覆盖矩形实现“预览”效果。
 - 直观工具栏：工具选择按钮、颜色选择、画笔宽度调整。

实现细节
主要类：
 - PaintCanvas（继承自 QWidget）：处理绘图逻辑、鼠标事件、画布渲染和大小调整。
 - MainWindow（继承自 QMainWindow）：提供主窗口、工具栏以及工具和颜色/宽度的界面控件。

核心概念：
 - 利用鼠标按下、移动、释放事件控制绘图。
 - 使用 QPainter 在 QImage 上绘制，QImage 作为画布。
 - 画布可动态扩展，保证原有绘图内容不丢失。
 - 工具与属性通过 Qt 信号槽机制更新。

界面：
工具栏包含画笔宽度、画笔颜色、填充颜色、填充开关以及工具按钮。
状态栏显示当前选中的工具。

Screenshots:
<img width="2560" height="1440" alt="example1" src="https://github.com/user-attachments/assets/6704c928-36e9-46bb-8ce0-ef7cf1882e61" />
<img width="2560" height="1440" alt="example2" src="https://github.com/user-attachments/assets/fd676ae6-32b4-4e60-8ac1-deb992b200cc" />





