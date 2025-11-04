#include "widget.h"
#include "./ui_widget.h"
#include "personmodel.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //create an object of the model
    PersonModel * model = new PersonModel(this);
    ui->treeView->setModel(model);
}

Widget::~Widget()
{
    delete ui;
}

/**
 * First get the currently selected item's index and the tree model.
 * Then insert a new row after the currently selected item
 */


void Widget::on_addRowButton_clicked()
{
    // 获取当前在 treeView 中选中的索引（当前选中的节点）
    QModelIndex  index = ui->treeView->selectionModel()->currentIndex();
    // 获取 treeView 所使用的数据模型（即 PersonModel）
    QAbstractItemModel *model = ui->treeView->model();

    if(!model->insertRow(index.row() + 1, index.parent()))
        return;

    // Loop through the columns to put in initial data
    // 遍历新插入行的每一列，为其设置初始数据. 即设置姓名和职业
    for(int column = 0 ; column < model->columnCount(index.parent()); ++column)
    {
        // 获取新插入行（index.row() + 1）中指定列（column）的索引
        QModelIndex child = model->index(index.row() + 1, column, index.parent());
        model->setData(child, QVariant("[Empty Cell]"), Qt::EditRole);
    }
}


void Widget::on_removeButton_clicked()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();

    // 删除当前选中的 column
    model->removeRows(index.row(), 1, index.parent());
}


void Widget::on_addChildButton_clicked()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();

    if(!model->insertRow(0, index))
        return;

    // Loop through the columns to put in initial data
    for(int column = 0; column < model->columnCount(index.parent()); ++column)
    {
        QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant("[Empty Cell]"), Qt::EditRole);
    }
    // 让新插入的行设置为当前选中项
    // - QItemSelectionModel::ClearAndSelect: 清除原有选中项，只选中新项
    ui->treeView->selectionModel()->setCurrentIndex(model->index(0, 0 , index),
                                                    QItemSelectionModel::ClearAndSelect);
}


