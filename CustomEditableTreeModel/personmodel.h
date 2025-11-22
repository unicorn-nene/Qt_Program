 #ifndef PERSONMODEL_H
#define PERSONMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <memory>
#include "person.h"


class PersonModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit PersonModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // make the model editable
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int position, int rows, const QModelIndex &parent) override;
    bool removeRows(int position, int rows, const QModelIndex &parent) override;

private:
    // read data
    void readFile();
    QVector<QVariant> parsePersonData(const QString &line);

    // Write data
    bool saveFile();
    void writeOPersonToStream(QTextStream &out, Person *person, int indent = 0) const;

private:
    //Member variables
    std::unique_ptr<Person> m_rootPerson{};


};

#endif // PERSONMODEL_H
