#include "person.h"

Person::Person(QObject *parent)
    :QObject(parent)
{
}

Person::Person(const QString &name, const QString &favoritecolor, const int &age, const int &socialScore, QObject *parent)
    :QObject(parent)
    ,m_names(name)
    ,m_favoriteColor(favoritecolor)
    ,m_age(age)
    ,m_socialScore(socialScore)
{

}

QString Person::names() const
{
    return m_names;
}

void Person::setNames(const QString &newNames)
{
    if(m_names == newNames)
        return;
    m_names = newNames;
    emit namesChanged(m_names);
}

QString Person::favoriteColor() const
{
    return m_favoriteColor;
}

void Person::setFavoriteColor(const QString &newFavoriteColor)
{
    if (m_favoriteColor == newFavoriteColor)
        return;
    m_favoriteColor = newFavoriteColor;
    emit favoriteColorChanged(m_favoriteColor);
}

int Person::age() const
{
    return m_age;
}

void Person::setAge(int newAge)
{
    if (m_age == newAge)
        return;
    m_age = newAge;
    emit ageChanged(m_age );
}

int Person::socialScore() const
{
    return m_socialScore;
}

void Person::setSocialScore(int newSocialScore)
{
    if(m_socialScore == newSocialScore)
        return;
    m_socialScore = newSocialScore;
}
