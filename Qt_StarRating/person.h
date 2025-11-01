#ifndef PERSON_H
#define PERSON_H

#include <QObject>

class Person : public QObject
{
    Q_OBJECT
public:
    explicit Person(QObject *parent = nullptr);
    Person(const QString &names, const QString favoriteColor, const int &age, const int &socialScore, QObject *parent = nullptr);
    ~Person();

    QString names() const;
    void setNames(const QString &newNames);

    QString favoriteColor() const;
    void setFavoriteColor(const QString &newFavoriteColor);

    int age() const;
    void setAge(int newAge);

    int socialScore() const;
    void setSocialScore(int newSocialScore);

private:
    QString m_names{};
    QString m_favoriteColor{};
    int m_age{};
    int m_socialScore{1};
};

#endif // PERSON_H
