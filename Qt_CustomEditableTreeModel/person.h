#ifndef PERSON_H
#define PERSON_H
#include <memory>
#include <vector>
#include <QVariant>

/**
 * @brief The Person class
 *
 * 一个用于树形结构（Tree Model）的节点类，每个 Person 对象代表树中的一个节点。
 *
 * 该类常用于配合 Qt 的 Model/View 框架（例如 QAbstractItemModel 的自定义实现），
 * 用来表示层次化的数据结构。
 *
 * 特点：
 * - 使用 unique_ptr 管理子节点，自动负责子节点的生命周期。
 * - 使用原始指针 (raw pointer) 保存父节点引用（非拥有关系 non-owning pointer）。
 * - 每个节点包含一个 QVector<QVariant>，用于存放多列数据（列字段）。
 */
class Person
{
public:
    Person(const QVector<QVariant> &data, Person *parent = nullptr);
    ~Person() = default;

    void appendChild(std::unique_ptr<Person> child);
    Person* child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row()const;
    Person* parentPerson();
    void printTree(int indent = 0) const;

    bool insertChildren(int position, int count, int column);
    bool removeChildren(int position, int count);
    bool setData(int column, const QVariant& value);

private:
    std::vector<std::unique_ptr<Person>> m_children{};  // 当前节点的子节点集合, 由 unique_ptr 自动管理声明周期
    QVector<QVariant> m_itemData{};                     // 当前节点的列数据(每个 QVariant 对应一列)
    Person *m_parent{};                                 // 父节点指针(non-owning 指针,仅用于访问父节点, 不负责释放)
};

#endif // PERSON_H
