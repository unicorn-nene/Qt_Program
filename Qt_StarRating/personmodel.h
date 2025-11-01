#ifndef PERSONMODEL_H
#define PERSONMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include "person.h"

class PersonModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum PersonRoles
    {
        NamesRole = Qt::UserRole + 1,
        FavoriteColorRole,
        AgeRole,
        SocialScoreRole
    };

    PersonModel(QObject *parent = nullptr);
    ~PersonModel();

    void addPerson(Person *person);
    void addPerson();
    void addPerson(const QString &names, const int age);
    void removePerson(QModelIndex index);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    // QAbstractItemModel interface
    // Methods to make the model editable
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Person *> m_persons{};
};

#endif // PERSONMODEL_H
