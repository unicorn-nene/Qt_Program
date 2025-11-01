#include "personmodel.h"

/**
 * @brief PersonModel::PersonModel
 * @param parent
 */
PersonModel::PersonModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    //populate with initial data
    m_persons.append(new Person("Jamie Lannister", "red", 33, 2));
    m_persons.append(new Person("Marry Lane", "cyan", 26, 3));
    m_persons.append(new Person("Steve Moors", "dodagerblue", 44, 4));
    m_persons.append(new Person("Victor Trunk", "dodgerBlue", 30, 4));
    m_persons.append(new Person("Ariel Geeny", "blue",33, 5));
    m_persons.append(new Person("Knut Viken","lightblue",26, 5));

     qDebug() << "PersonModel init";
}

PersonModel::~PersonModel()
{
    qDebug() << "PersonModel destroy";
    qDeleteAll(m_persons);
}

/**
* @brief 向 Model 中添加一个已有的 Person 对象
* @param person 需要插入的 Person 实例对象
*/
void PersonModel::addPerson(Person *person)
{
    insertRows(m_persons.size(), 1, QModelIndex());
    setData(index(m_persons.size() - 1, 0), person->names(), NamesRole);
    setData(index(m_persons.size() - 1, 1), person->favoriteColor(), FavoriteColorRole);
    setData(index(m_persons.size() - 1, 2), person->age(), AgeRole);
    setData(index(m_persons.size() - 1, 3), person->socialScore(), SocialScoreRole);
}

/**
 * @brief 添加一个默认构造的 Person 对象
 * @details 主要用于测试或者默认添加条目
 */
void PersonModel::addPerson()
{
    Person *person = new Person("Added person", "yellowgreen", 45, 3, this);
    addPerson(person);
}

/**
 * @brief PersonModel::addPerson
 * @param names
 * @param age
 */
void PersonModel::addPerson(const QString &names, const int age)
{
    Person *person = new Person(names, "yellowgreen", age, 3, this);
    addPerson(person);
}

/**
 * @brief 移除指定索引所在的 Person 位置
 * @param index 视图中用户选中的条目索引
 */
void PersonModel::removePerson(QModelIndex index)
{
    if(index.isValid())
    {
        removeRows(index.row(), 1, QModelIndex());
    }
}

/**
 * @brief 返回 model 当前的行数
 * @param parent 父节点索引(未使用)
 * @return 行数
 */
int PersonModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_persons.size();
}

/**
 * @brief 返回 model 当前的列数
 * @param parent 父节点索引(未使用)
 * @return 列数
 */
int PersonModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

/**
 * @brief 根据 index 和 Role 获取对应的数据
 * @param index 当前访问的单元格位置/索引
 * @param role 访问的数据的类型
 * @return QVariant 封装的对应字段内容
 */
QVariant PersonModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_persons.size())
        return QVariant();

    Person *person = m_persons[index.row()];

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            // names
            return person->names();
        }
        if(index.column() == 1)
        {
            // ages
            return person->age();
        }
        if(index.column() == 2)
        {
            // favoriteColor
            return person->favoriteColor();
        }
        if(index.column() == 3)
        {
            // socialScore
            return person->socialScore();
        }
    }

    if(role == Qt::ToolTipRole || role == NamesRole)
    {
        return person->names();
    }

    if(role == FavoriteColorRole)
    {
        return person->favoriteColor();
    }

    if(role == AgeRole)
    {
        return person->age();
    }

    if(role == SocialScoreRole)
    {
        return person->socialScore();
    }

    return QVariant();
}


// ==================== Methods to make the model editable ======================
/**
 * @brief 将编辑后的数据从 view 写回 Model
 * @param index 对应的数据项(model) 索引
 * @param value 用户修改后的数据
 * @param role 数据角色
 * @return 成功写入返回 ture, 失败返回 false
 */
bool PersonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    Person *person = m_persons[index.row()];
    bool somethingChanged{false};

    switch(role)
    {
    case Qt::EditRole: // role == Qt::EditRole 说明这是编辑操作, person 的所有数据项都是可编辑内容.
    {
        // change the names
        if(person->names() != value.toString())
        {
            person->setNames(value.toString());
            somethingChanged = true;
        }
        // change the age
        if(person->age() != value.toInt())
        {
            person->setAge(value.toInt());
            somethingChanged = true;
        }
        // change the favoriteColor
        if(person->favoriteColor() != value.toString())
        {
            person->setFavoriteColor(value.toString());
            somethingChanged = true;
        }
        // change the SocialSocre
        if(person->socialScore() != value.toInt())
        {
            person->setSocialScore(value.toInt());
            somethingChanged = true;
        }
        break;
    }

    case NamesRole:
    {
        if(person->names()!= value.toString())
        {
            person->setNames(value.toString());
            somethingChanged = true;
        }
        break;
    }

    case AgeRole:
    {
        if(person->age() != value.toInt())
        {
            person->setAge(value.toInt());
            somethingChanged = true;
        }
        break;
    }

    case FavoriteColorRole:
    {
        if(person->favoriteColor() != value.toString())
        {
            person->setFavoriteColor(value.toString());
            somethingChanged = true;
        }
        break;
    }

    case SocialScoreRole:
    {
        if(person->socialScore() != value.toInt())
        {
            person->setSocialScore(value.toInt());
            somethingChanged = true;
        }
        break;
    }

    }

    if(somethingChanged == true)
    {
        emit dataChanged(index, index, {Qt::DisplayRole | Qt::EditRole});
        return true;
    }

    return false;
}

/**
 * @brief 当 View 想要显示表头 (Header) 时, 会调用此函数来获取对应的标题文字
 * @param section 表头的序号(行号或列号)
 * @param orientation 水平(列头)或垂直(行头)
 * @param role 默认为 Qt::DispalyRole
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
            break;
        case 1:
            return QString("Age");
            break;
        case 2:
            return QString("Favorite Color");
            break;
        case 3:
            return QString("Social Score");
            break;
        }
    }

    // vertical headers
    return QString("Person %1").arg(section);
}


/**
 * @brief 返回指定模型项的交互标志/Role
 * @param index 对应的数据项(Model)索引
 * @return 可用的交互行为组合
 */
Qt::ItemFlags PersonModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return QAbstractTableModel::flags(index);
    }

    // 给每一个合法项额外添加 Qt::ItemsIsEditable 标志, 表示可编辑.
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

/**
 * @brief 在指定位置插入指定数量的行
 * @param row 要插入的起始行的位置
 * @param count 要插入的行数
 * @param parent 未使用
 * @return 插入是否成功
 */
bool PersonModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginInsertRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i)
    {
        m_persons.insert(row, new Person()); // 从 row 位置开始插入新对象, 插入 count 个
    }
    endInsertRows();

    return true;
}

/**
 * @brief 删除指定行数的数据
 * @param row 要删除的起始行
 * @param count 要删除的行数
 * @param parent 未使用(用于树结构)
 * @return 删除操作是否成功
 */
bool PersonModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i)
    {
        Person *person = m_persons.takeAt(row);
        delete person;
    }

    endRemoveRows();

    return true;
}

/**
 * @brief 返回 model 支持的所有自定义 Role 与名称的映射
 * @return
 */
QHash<int, QByteArray> PersonModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[NamesRole] = "names";
    roles[FavoriteColorRole] = "favoritecolor";
    roles[AgeRole] = "age";
    roles[SocialScoreRole] = "socialScore";

    return roles;
}
