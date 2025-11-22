#ifndef PERSON_H
#define PERSON_H

#include <QObject>

class Person : public QObject
{
    Q_OBJECT
public:
    explicit Person(QObject *parent = nullptr);
    explicit Person(const QString &name, const QString &favoritecolor, const int &age, const int &socialScore, QObject *parent = nullptr);

    QString names() const;
    void setNames(const QString &newNames);

    QString favoriteColor() const;
    void setFavoriteColor(const QString &newFavoriteColor);

    int age() const;
    void setAge(int newAge);

    int socialScore() const;
    void setSocialScore(int newSocialScore);

signals:
    void namesChanged(QString names);
    void favoriteColorChanged(QString favoriteColor);
    void ageChanged(int age);

private:
    QString m_names{};
    QString m_favoriteColor{};

    int m_age{};
    int m_socialScore{};
};

#endif // PERSON_H
