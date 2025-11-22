#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "expense.h"
#include "expensemodel.h"
#include "expenseproxymodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_addExpenseButton_clicked();

    void on_removeExpenseButton_clicked();

    void on_filterLine_textChanged(const QString &text);

    void on_filterColumnCombobox_currentTextChanged(const QString &text);

private:
    Ui::Widget *ui;

    ExpenseModel *m_model{};
    ExpenseProxyModel *m_proxyModel{};
};
#endif // WIDGET_H
