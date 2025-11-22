#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_canvas = new PaintCanvas(this);
    setCentralWidget(m_canvas);

    //pen width
    QLabel * penWidthLabel = new QLabel("Pen Width", this);
    QSpinBox * penWidthSpinBox = new QSpinBox(this);
    penWidthSpinBox->setValue(2);
    penWidthSpinBox->setRange(1,15);

    // Pen Color
    QLabel * penColorLabel = new QLabel("Pen Color", this);
    m_penColorButton = new QPushButton(this);

    // Fill Color
    QLabel * fillColorLabel = new QLabel("Fill Color", this);
    m_fillColorButton = new QPushButton(this);

    // Fill on/off
    m_fillCheckBox = new QCheckBox("Fill Shape", this);

    // Tool Buttons
    QPushButton * rectButton = new QPushButton(this);
    rectButton->setIcon(QIcon(":/images/rectangle.png"));

    QPushButton * penButton = new QPushButton(this);
    penButton->setIcon(QIcon(":/images/pen.png"));

    QPushButton * ellipseButton = new QPushButton(this);
    ellipseButton->setIcon(QIcon(":/images/circle.png"));

    QPushButton * eraserButton = new QPushButton(this);
    eraserButton->setIcon(QIcon(":/images/eraser.png"));

    // connect status
    connect(rectButton, &QPushButton::clicked, [=]()
            {
        //set current tool to rect
        m_canvas->setTool(PaintCanvas::Rect);
        statusBar()->showMessage("Current tool : Rect");
    });

    connect(penButton, &QPushButton::clicked,[=]()
            {
        m_canvas->setTool((PaintCanvas::Pen));
        statusBar()->showMessage("Current tool : Pen");
    });

    connect(ellipseButton, &QPushButton::clicked, [=]()
                            {
        m_canvas->setTool(PaintCanvas::Ellipse);
        statusBar()->showMessage("Current Tool : Ellipse");
    });

    connect(eraserButton, &QPushButton::clicked, [=]()
            {
        m_canvas->setTool(PaintCanvas::Eraser);
        statusBar()->showMessage("Current Tool : Eraser");
    });

    // Slots connections
    connect(penWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(penWidthChanged(int)));
    connect(m_penColorButton,SIGNAL(clicked()), this, SLOT(changePenColor()));
    connect(m_fillColorButton, SIGNAL(clicked()), this, SLOT(changeFillColor()));
    connect(m_fillCheckBox, SIGNAL(clickeded()), this, SLOT(changeFillProperty()));

    // add Widget
    ui->mainToolBar->addWidget(penWidthLabel);
    ui->mainToolBar->addWidget(penWidthSpinBox);
    ui->mainToolBar->addWidget(penColorLabel);
    ui->mainToolBar->addWidget(m_penColorButton);
    ui->mainToolBar->addWidget(fillColorLabel);
    ui->mainToolBar->addWidget(m_fillColorButton);
    ui->mainToolBar->addWidget(m_fillCheckBox);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(penButton);
    ui->mainToolBar->addWidget(rectButton);
    ui->mainToolBar->addWidget(ellipseButton);
    ui->mainToolBar->addWidget(eraserButton);

    // init pen color and fill color
    QString css = QString("background-color : %1").arg(m_canvas->penColor().name());
    m_penColorButton->setStyleSheet(css);

    css = QString("background-color : %1").arg(m_canvas->fillColor().name());
    m_fillColorButton->setStyleSheet(css);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::penWidthChanged(int width)
{
    m_canvas->setPenWidth(width);
}

void MainWindow::changePenColor()
{
    QColor color = QColorDialog::getColor(m_canvas->penColor());

    if(color.isValid())
    {
        m_canvas->setPenColor(color);
        QString css = QString("background-color :　%1").arg(color.name());
        m_penColorButton->setStyleSheet(css);
    }
}

void MainWindow::changeFillColor()
{
    QColor color = QColorDialog::getColor(m_canvas->fillColor());

    if(color.isValid())
    {
        m_canvas->setFillColor(color);
        QString css = QString("background-color : %1").arg(color.name());
        m_fillColorButton->setStyleSheet(css);
    }
}

void MainWindow::changeFillProperty()
{
    m_canvas->setFill(m_fillCheckBox->isChecked());
}
