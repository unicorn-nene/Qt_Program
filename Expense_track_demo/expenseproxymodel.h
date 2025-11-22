#ifndef EXPENSEPROXYMODEL_H
#define EXPENSEPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class ExpenseProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ExpenseProxyModel(QObject *parent = nullptr);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

#endif // EXPENSEPROXYMODEL_H
