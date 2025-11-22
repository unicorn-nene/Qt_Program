#ifndef EXPENSE_H
#define EXPENSE_H

#include <QDate>
#include <QObject>
#include <QString>

class Expense : public QObject
{
    Q_OBJECT
public:
    explicit Expense(QObject *parent = nullptr);
    explicit Expense(const QDate &date, const QString &item, double amount, const QString &category, QObject *parent = nullptr);


    QDate date() const;
    QString item() const;
    double amount() const;
    QString category() const;

    void setDate(const QDate &newDate);
    void setItem(const QString &newItem);
    void setAmount(double newAmount);
    void setCategory(const QString &newCategory);

signals:
    void dateChanged(const QDate &date);
    void itemChanged(const QString &item);
    void amountChanged(double amount);
    void categoryChanged(const QString &category);

private:
    QDate m_date{};
    QString m_item{};
    double m_amount{};
    QString m_category{};
};

#endif // EXPENSE_H
