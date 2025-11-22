#include "personmodel.h"

PersonModel::PersonModel(QObject *parent)
    :QAbstractTableModel{parent}
{
    //populate with initial data
    m_persons.append(new Person("Jamie Lannister", "red", 33, 2));
    m_persons.append(new Person("Marry Lane", "cyan", 26, 5));
    m_persons.append(new Person("Steve Moors", "yellow", 44, 3));
    m_persons.append(new Person("Victor Trunk", "dodgerBlue", 30, 2));
    m_persons.append(new Person("Ariel Geeny", "blue", 33, 4));
    m_persons.append(new Person("Knut Vikran", "lightblue", 26, 3));
}

PersonModel::~PersonModel()
{
    qDeleteAll(m_persons);
}

/**
 * @brief PersonModel::addPerson 向 Model 中添加一个已有的 Person 对象
 * @param person 要添加的 Person 实例指针
 */
void PersonModel::addPerson(Person *person)
{
    insertRows(m_persons.size(),1);

    setData(index(m_persons.size() - 1, 0), person->names(), Qt::EditRole);
    setData(index(m_persons.size() - 1, 1), person->favoriteColor(), Qt::EditRole);
    setData(index(m_persons.size() - 1, 2), person->socialScore(), Qt::EditRole);
}

/**
 * @brief PersonModel::addPerson  向 Model 中添加一个空的 Person 条目
 */
void PersonModel::addPerson()
{
    Person *person = new Person("Added Person", "yellowgreen", 45, 5 ,this);
    addPerson(person);
}

/**
 * @brief 向 Model中添加一条包含指定信息的 Person 记录
 * @param names         姓名
 * @param age           指向年龄的指针（允许为空）
 * @param socialScore   社交分数
 */
void PersonModel::addPerson(const QString &names, const int &age, const int &socialScore)
{
    Person *person = new Person(names, "yellowgreen", age, socialScore, this);
    addPerson(person);
}

/**
 * @brief PersonModel::removePerson 删除指定 index 的Person项
 * @param index 要删除的 index
 */
void PersonModel::removePerson(QModelIndex index)
{
    removeRows(index.row(), 1);
}

/**
 * @brief PersonModel::rowCount 返回 model 的行数
 * @param parent 父索引
 * @return int 当前 model 的行数
 */
int PersonModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_persons.size();
}

/**
 * @brief PersonModel::columnCount 返回 model 的列数
 * @param parent 父索引
 * @return int 当前 model 的列数
 */
int PersonModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 4;
}

/**
 * @brief PersonModel::data 返回 model 指定 index 处的数据
 * @param index      model的索引
 * @param role       指定的 Qt::Role
 * @return QVariant  对应 Role 的数据
 */
QVariant PersonModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_persons.size())
        return QVariant();

    Person *person = m_persons[index.row()];
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(index.column() == 0)
            return person->names();
        if(index.column() == 1)
            return person->age();
        if(index.column() == 2)
            return person->favoriteColor();
        if(index.column() == 3)
            return person->socialScore();
    }

    if(role == NamesRole)
        return person->names();
    if(role == FavoriteColorRole)
        return person->favoriteColor();
    if(role == AgeRole)
        return person->age();
    if(role == SocialScoreRole)
        return person->socialScore();

    if(role == Qt::EditRole)
    {
        qDebug() << "Data() method called with eidt role";
        return person->names();
    }

    if(role == Qt::ToolTipRole)
    {
        return person->names();
    }

    return QVariant();
}

/**
 * @brief PersonModel::setData 设置 Model 指定 index 的数据
 * @param index     修改的索引
 * @param value     要修改的值
 * @param role      要修改的 Role
 * @return bool     返回是否修改成功
 */
bool PersonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    Person *person = m_persons[index.row()];
    bool somethingChanged = false;

    switch (role)
    {
    case Qt::EditRole:
    {
        if(index.column() == 0)
        {
            if(person->names()!=value.toString())
            {
                person->setNames(value.toString());
                somethingChanged = true;
            }
        }
        if(index.column() == 1)
        {
            if(person->age() != value.toInt())
            {
                person->setAge(value.toInt());
                somethingChanged = true;
            }
        }
        if(index.column() == 2)
        {
            if(person->favoriteColor()!=value.toString())
            {
                person->setFavoriteColor(value.toString());
                somethingChanged = true;
            }
        }
        if(index.column() == 3)
        {
            if(person->socialScore() != value.toInt())
            {
                person->setSocialScore(value.toInt());
                somethingChanged = true;
            }
        }
    }
    break;

    case NamesRole:
    {
        qDebug() << "Names role changing names, index " << index.row();
        if(person->names()!= value.toString())
        {
            person->setNames(value.toString());
            somethingChanged = true;
        }
    }
    break;

    case AgeRole:
    {
        if(person->age() != value.toInt())
        {
            person->setAge(value.toInt());
            somethingChanged = true;
        }
    }
    break;

    case FavoriteColorRole:
    {
        if(person->favoriteColor() != value.toString())
        {
            person->setFavoriteColor(value.toString());
            somethingChanged = true;
        }
    }
    break;
    }

    if(somethingChanged)
    {
        // When reimplementing the setData() function, this signal must to be explicitly
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

/**
 * @brief PersonModel::headerData 返回 Model 表头的数据
 * @param section       表头的行号或列号
 * @param orientation   行标题/列标题
 * @param role          指定的 Qt::Role
 * @return Qvariant     对应表头的数据
 */
QVariant PersonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return QString("Names");
        case 1:
            return QString("Age");
        case 2:
            return QString("Favorite Color");
        case 3:
            return QString("Social Socre");
        }
    }

    return QString("Person %1").arg(section);
}

/**
 * @brief PersonModel::flags 返回指定索引的 Item 标志flags
 * @param index             指定的 index
 * @return Qt::ItemFlags    item 的标志
 */
Qt::ItemFlags PersonModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return QAbstractTableModel::flags(index);

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

/**
 * @brief PersonModel::insertRows 再指定位置插入行
 * @param row       插入的起始行号
 * @param count     插入的行数
 * @param parent    父索引
 * @return bool     插入是否成功
 */
bool PersonModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginInsertRows(QModelIndex(), row, row + count - 1);
    for(int i = 0;i < count; ++i)
    {
        m_persons.insert(row, new Person());
    }
    endInsertRows();

    return true;
}

/**
 * @brief PersonModel::removeRows 在指定位置删除行
 * @param row       删除的起始行号
 * @param count     删除的行数
 * @param parent    父索引
 * @return bool     删除是否成功
 */
bool PersonModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i)
    {
        m_persons.removeAt(row);
    }
    endRemoveRows();

    return true;
}

/**
 * @brief PersonModel::roleNames 返回 Model 中自定义 Role 的名称映射
 * @return QHash<int, QByteArray> <Role ID, Role Names>
 */
QHash<int, QByteArray> PersonModel::roleNames() const
{
    QHash<int, QByteArray> roles{};

    roles[NamesRole] = "names";
    roles[FavoriteColorRole] = "favoritecolor";
    roles[AgeRole] = "age";
    roles[SocialScoreRole] = "socialscore";

    return roles;
}

/**
 * @brief PersonModel::hasChildren 判断指定 index 下是否有子项
 * @param parent   要检查的父索引
 * @return bool    是否有子项
 */
bool PersonModel::hasChildren(const QModelIndex &parent) const
{
    if(parent.column() == 0)
        return false;

    return true;
}
