#include "person.h"

/**
* @brief 构造函数
* @param data   节点的列数据（QVector 中每个元素对应一列）
* @param parent 父节点指针（非拥有指针，默认为 nullptr 表示根节点）
*/
Person::Person(const QVector<QVariant> &data, Person *parent)
    :m_itemData(data)
    ,m_parent(parent)
{

}

/**
* @brief 默认析构函数
* 子节点通过 unique_ptr 自动销毁，无需手动管理。
*/
void Person::appendChild(std::unique_ptr<Person> child)
{
    m_children.push_back(std::move(child));
}

/**
* @brief 添加一个子节点
* @param child 待添加的子节点（由 unique_ptr 持有）
*
* 注意：此函数会自动建立父子关系。
*/
Person *Person::child(int row)
{
    // make sure you don't go over bounds
    if(row < 0 || row >= static_cast<int>(m_children.size()))
        return nullptr;

    return m_children[row].get();
}

/**
 * @brief 取当前节点的子节点数量
 * @return 子节点数量
 */
int Person::childCount() const
{
    return static_cast<int>(m_children.size());
}

/**
 * @brief 获取当前节点的列数量
 * @return 节点的列数量
 */
int Person::columnCount() const
{
    return m_itemData.size();
}

/**
 * @brief 获取当前节点指定列的数据
 * @param column 列号
 * @return QVariant 类型的数据；若列号无效则返回空 QVariant
 */
QVariant Person::data(int column) const
{
    return m_itemData.value(column);
}

/**
 * @brief 获取当前节点在父节点中的行号
 * @return 节点在父节点子列表中的位置（从 0 开始）
 */
int Person::row() const
{
    if(m_parent)
    {
        // Find the pointer equal to this pointer in the list of our parent's children
        auto it = std::find_if(m_parent->m_children.begin(), m_parent->m_children.end(),
                               [this](const auto & child){ return child.get() == this; });

        // Find the location
        if(it != m_parent->m_children.end())
        {
            // we have found our iterator. We just need to return the index
            return std::distance(m_parent->m_children.begin(), it);
        }
    }

    return 0;
}

Person *Person::parentPerson()
{
    return m_parent;
}

/**
 * @brief 打印当前节点及其所有子节点（用于调试）
 * @param indent 当前节点的缩进空格数，表示节点在树中的层级
 *
 * 这个函数递归地打印整棵树的结构：
 * 1. 当前节点先打印自身的数据，每列数据用 " | " 分隔
 * 2. 然后递归调用 printTree 打印每个子节点，缩进增加，形成树状结构
 *
 * 主要用于调试，直观显示树的层级和每个节点的数据。
 */
void Person::printTree(int indent) const
{
    // 根据 indent 参数生成缩进字符串
    QString indentStr = QString(indent, ' ');

    //Print the current node
    QStringList dataStrings;
    for(const QVariant& item : m_itemData)
    {
        dataStrings << item.toString();
    }

    // 输出当前节点，前面加缩进和 "+-" 符号表示层级
    qDebug().noquote() << indentStr << "+-" <<dataStrings.join(" | ");

    // Print children
    for(const auto & child : m_children)
    {
        child->printTree(indent + 4);
    }
}

bool Person::insertChildren(int position, int count, int column)
{
    if(position <0 || position > static_cast<int>(m_children.size()))
        return false;

    for(int row  = 0; row < count; ++row)
    {
        QVector<QVariant> data(column);
        auto person = std::make_unique<Person>(data, this);
        m_children.insert(m_children.begin() + position, std::move(person)); // 把一个 unique_ptr 放入 vector，必须使用移动语义
    }

    return true;
}

bool Person::removeChildren(int position, int count)
{
    if(position < 0 || position + count > static_cast<int>(m_children.size()))
        return false;

    m_children.erase(m_children.begin() + position, m_children.begin() + position + count);
    return true;
}

bool Person::setData(int column, const QVariant &value)
{
    if(column < 0 || column > m_itemData.size())
        return false;

    m_itemData[column] = value;
    return true;
}
