#include "widget.h"
#include "./ui_widget.h"
#include <QGraphicsPixmapItem>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 创建场景
    m_scene = new Scene(this);
    // scene 尺寸，比如 -250,-300 到 250,300
    m_scene->setSceneRect(-250, -300, 500, 600);

    //add sky, set to centeral
    QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem(QPixmap(":/images/sky.png"));
    m_scene->addItem(pixItem);
    pixItem->setPos(QPointF(0,0) - QPointF(pixItem->boundingRect().width() / 2.0f,
                                            pixItem->boundingRect().height() / 2.0f));


    // 添加到 scene
    m_scene->addItem(pixItem);

    // 绑定 scene
    ui->graphicsView->setScene(m_scene);

    m_scene->addBird();
}


Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    m_scene->startGame();
}

