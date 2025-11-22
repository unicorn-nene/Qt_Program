#include "personmodel.h"
#include <QFile>

PersonModel::PersonModel(QObject *parent)
    : QAbstractItemModel{parent}
{
    QVector<QVariant> rootData;
    rootData << "Names" << "Proffession";
    m_rootPerson = std::make_unique<Person>(rootData);

    readFile();
}

/**
 * @brief 根据行号、列号和父索引创建子项的 QModelIndex(QModelIndex 是 Qt 视图层使用的“索引”对象)
 * @param row    子项所在的行号
 * @param column 子项所在的列号
 * @param parent 父项的模型索引（如果无效，表示根节点）
 * @return 对应子项的 QModelIndex（如果不存在，返回无效索引）
 */
QModelIndex PersonModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    // Go form the language of QAbstractItemModel to the language of our Person tree structure
    // 将 QAbstractItemModel 的父索引转换为我们自己的树节点指针
    Person *parentPerson = parent.isValid() ? static_cast<Person *> (parent.internalPointer()) : m_rootPerson.get();

    // Get the child at a given row
    // 获取指定行的子节点
    Person *childPerson = parentPerson->child(row);
    if(childPerson)
    {
        // createIndex 会在内部记录：行、列和该节点对应的指针
        // 创建新的对应row, col, 以及数据的索引
        return createIndex(row, column, childPerson);
    }

    return QModelIndex();
}

/**
 * @brief PersonModel::parent
 * 将树结构中的子项（child）映射回它的父项（parent），
 * 并返回该父项在模型中的 QModelIndex
 * @param child 子项的 QModelIndex
 * @return QModelIndex 父项的模型索引。如果子项是根节点或无效，则返回空索引
 */
QModelIndex PersonModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
        return QModelIndex();

    // 从 QModelIndex 中取出绑定的 Person 对象
    Person *childPerson = static_cast<Person *>(child.internalPointer());
    // 获取该子节点对应的父节点
    Person *parentPerson = childPerson->parentPerson();

    if(parentPerson == m_rootPerson.get())
        return QModelIndex();

    //  创建并返回父节点的索引（列固定为0）
    return createIndex(parentPerson->row(), 0, parentPerson);
}

/**
 * @brief 返回给定父索引下的子节点数量（即行数）
 * @param parent 要查询的父节点索引。如果是无效索引（invalid），表示查询的是根节点。
 * @return 子节点的数量（行数）
 */
int PersonModel::rowCount(const QModelIndex &parent) const
{
    // Make sure we process children only for items in the first column
    // 只有第一列的项可以有子节点（多列树模型的常规约定）
    if(parent.column() > 0)
        return 0;

    // Go from a QModelIndex to a row Person poniter
    // 如果 parent 有效，就取出对应的 Person 节点；
    // 否则使用根节点 m_rootPerson
    Person * parentPerson = parent.isValid() ? static_cast<Person *>(parent.internalPointer()) : m_rootPerson.get();

    // 返回该节点的子节点数量
    return parentPerson->childCount();
}

int PersonModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2; // Names and Proffession
}

QVariant PersonModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    Person *person = static_cast<Person *>(index.internalPointer());
    return person->data(index.column());
}

bool PersonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole)
        return false;

    Person *person = static_cast<Person *>(index.internalPointer());
    bool result = person->setData(index.column(), value);

    if(result)
    {
        emit dataChanged(index, index, {role});
    }

    return result;
}

bool PersonModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    Person *parentItem = parent.isValid() ? static_cast<Person *>(parent.internalPointer()) : m_rootPerson.get();

    bool success;
    beginInsertRows(parent, position, position + rows - 1);

    success = parentItem->insertChildren(position, rows, 2); // 这里的 2 表示 两列数据 "姓名" 和 "职业'

    endInsertRows();

    if(success)
    {
        saveFile();
    }

    return success;
}

bool PersonModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    Person *parentItem = parent.isValid() ? static_cast<Person *>(parent.internalPointer()) : m_rootPerson.get();

    bool success;

    beginRemoveRows(parent, position, position + rows - 1);

    success = parentItem->removeChildren(position, rows);

    endRemoveRows();

    if(success)
    {
        saveFile();
    }

    return success;
}

QVariant PersonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == 0)
            return tr("Names");
        else if(section == 1)
            return tr("Proffession");
    }

    return QVariant();
}

Qt::ItemFlags PersonModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

/**
 * @brief PersonModel::readFile
 * 从 文件中读取层级结构文本
 */
void PersonModel::readFile()
{
    QString filename = "data/familytree1.txt";
    QFile inputFile(filename);

    if(!inputFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open the file: " << filename;
        return;
    }
    else
    {
        qDebug() << "File opened successfuly for reading!";
    }

    QTextStream in(&inputFile);

    Person *lastParent = m_rootPerson.get();    // 当前的父节点(最初是根节点)
    Person *lastPerson = nullptr;               // 当前节点
    int lastIndentation{0};                     // 上一次的缩进层级

    while(!in.atEnd())
    {
        QString line = in.readLine();
        int currentIndentation = line.count('\t');  // 计算当前行的缩进数量

        QVector<QVariant> data = parsePersonData(line.trimmed());

        // 比较 当前行缩进与上次缩进的差值
        int indentDiff = currentIndentation - lastIndentation;

        // 当前行(节点)与上一行(节点)同级, 挂在在同一父节点下
        if(indentDiff == 0)
        {
            //Add the child under the current parent
            auto person = std::make_unique<Person>(data, lastParent);
            lastPerson = person.get();
            lastParent->appendChild(std::move(person));
        }
        // 缩进更深, 说明进入了新的子层级, 上一个节点成为新的父节点
        else if(indentDiff > 0)
        {
            // Go deeper in the hierarchy
            lastParent = lastPerson;
            auto person = std::make_unique<Person> (data, lastParent);
            lastPerson = person.get();
            lastParent->appendChild(std::move(person));
        }
        // 缩进更少, 表示需要返回上层(父节点)
        else
        {
            // diff in indentation is nagative: Navigate back in the tree structure to figure out the right parent
            for(int i = 0 ; i < -indentDiff; ++i)
            {
                lastParent = lastParent->parentPerson();
            }

            auto person = std::make_unique<Person>(data, lastParent);
            lastPerson = person.get();
            lastParent->appendChild(std::move(person));
        }
        // 更新缩进层级
        lastIndentation = currentIndentation;
    }

    inputFile.close();
}

/**
 * @brief PersonModel::parsePersonData
 * 解析一行文本, 提取 "姓名" 和 "职业"信息.
 * @param line 要解析的一行文本 "Samuel Grayson(Barber)"
 * @return QVector<QVariant> 包含两列数据:
 *          [0] 姓名 "Samuel Grayson"
 *          [1] 职业 "Barber"
 */
QVector<QVariant> PersonModel::parsePersonData(const QString &line)
{
    // line = "Samuel Grayson(Barber)"
    // 拆分姓名和职业
    QStringList parts = line.split('(');
    QVector<QVariant> data;
    // data[0](姓名部分)= "SamuelGrayson"
    data << parts[0].trimmed();

    if(parts.size() > 1)
    {
        // data[1](职业部分) = Barber)
        QString proffesion = parts[1];
        proffesion.chop(1);     // 移除最后一个字符,也就是'(', 现在是 data[1] = "Barber"
        data << proffesion;
    }
    else // if(parts.size() <= 1) 即只有 data[0]名字, 就说明没有职业信息
    {
        data << QString();
    }

    // data 是 QVector<QVariant>
    // data[0] = "SamuelGrayson"
    // data[1] = "Barber"
    return data;
}

bool PersonModel::saveFile()
{
    QString filename = "data/familytree1.txt";
    QFile outputFile(filename);

    if(!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open file for writting:" << filename << "-" << outputFile.errorString();
        return false;
    }

    QTextStream out(&outputFile);

    // Write all children of the root item
    for(int i = 0; i < m_rootPerson->childCount(); ++i)
    {
        writeOPersonToStream(out, m_rootPerson->child(i), 0);
    }

    out.flush();
    outputFile.close();

    return true;
}

void PersonModel::writeOPersonToStream(QTextStream &out, Person *person, int indent) const
{
    if(!person)
        return;

    // Create indentation with tabs
    QString indentation;
    for(int i = 0; i < indent; ++i)
    {
        indentation += "\t";
    }

    //Format the line: name(proffession)
    QString name = person->data(0).toString();
    QString proffession = person->data(1).toString();
    QString line = indentation + name;

    if(!proffession.isEmpty())
    {
        line += "(" + proffession + ")";
    }
    out << line << '\n';

    // write the children recursively
    for(int i = 0; i < person->childCount(); ++i)
    {
        writeOPersonToStream(out, person->child(i), indent + 1);
    }
}
