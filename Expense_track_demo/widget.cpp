#include "widget.h"
#include "./ui_widget.h"
#include <QInputDialog>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_model = new ExpenseModel(this);

    // Set the proxy model
    m_proxyModel = new ExpenseProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    // set up table view
    ui->tableView->setModel(m_proxyModel);
    ui->tableView->resizeColumnsToContents();   // Resizes all columns based on the size hints of the delegate used to render each item in the columns
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);     // Sets the constraints on how the header can be resized to those described by the given mode
    ui->tableView->setSortingEnabled(true);

    // Initialize filter column combo box
    ui->filterColumnCombobox->addItems({"All Columns", "Date", "Item", "Amount", "Category"});
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief Widget::on_addExpenseButton_clicked
 * 当用户点击“Add Expense”（添加支出）按钮时触发。
 */
void Widget::on_addExpenseButton_clicked()
{
    bool ok;
    QString item = QInputDialog::getText(this, "Add Expense", "Item name:", QLineEdit::Normal, "Coffee", &ok);
    if(ok && !item.isEmpty())
    {
        double amount = QInputDialog::getDouble(this, "Add Expense", "Amount:", 0.00, 0.00, 1000000.00, 2, &ok);

        if(ok)
        {
            QString category = QInputDialog::getText(this, "Add Expense", "Category:", QLineEdit::Normal, "Food", &ok);

            if(ok && !category.isEmpty())
            {
                Expense *expense = new Expense(QDate::currentDate(), item, amount, category, m_model);
                m_model->addExpense(expense);
            }
        }
    }
}

/**
 * @brief Widget::on_removeExpenseButton_clicked 当用户点击“Remove Expense”（删除支出）按钮时触发
 */
void Widget::on_removeExpenseButton_clicked()
{
    QModelIndex proxyIndex = ui->tableView->currentIndex();
    if(proxyIndex.isValid())
    {
        // 将代理模型索引转换为源模型索引
        QModelIndex sourceIndex = m_proxyModel->mapToSource(proxyIndex);
        m_model->removeExpense(sourceIndex);
    }
    else
    {
        QMessageBox::warning(this, "Remove Expense", "Please select an expense to remove");
    }
}

/**
 * @brief Widget::on_filterLine_textChanged 当用户在过滤文本框中输入文字时触发
 * @param text 户输入的过滤文本
 */
void Widget::on_filterLine_textChanged(const QString &text)
{
    // 当前选择的列索引（-1 表示搜索所有列）
    int column = ui->filterColumnCombobox->currentIndex() - 1; // -1 because first item is "All columns"
    if(column == -1)
    {
        // 设置为 -1 表示对所有列进行过滤
        m_proxyModel->setFilterKeyColumn(-1); // Search all columns
    }
    else
    {
        // 只在选定列中匹配
        m_proxyModel->setFilterKeyColumn(column);
    }

     // 设置过滤关键字
    m_proxyModel->setFilterFixedString(text);
}

/**
 * @brief Widget::on_filterColumnCombobox_currentTextChanged 当过滤列下拉框选择改变时触发
 * @param arg1 当前选中的下拉框文本
 */
void Widget::on_filterColumnCombobox_currentTextChanged(const QString &text)
{
    // 根据当前输入框内容立即更新过滤结果
    on_filterLine_textChanged(ui->filterLine->text());
}

