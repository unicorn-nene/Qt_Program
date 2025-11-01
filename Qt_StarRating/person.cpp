#include "person.h"
#include <QDebug>

Person::Person(QObject *parent)
    : QObject{parent}
{}

Person::Person(const QString &names, const QString favoriteColor, const int &age, const int &socialScore, QObject *parent)
    :m_names(names)
    ,m_favoriteColor(favoriteColor)
    ,m_age(age)
    ,m_socialScore(socialScore)
    ,QObject(parent)
{
    qDebug() << "person init";
}

Person::~Person()
{
  qDebug() << "person destory";
}

QString Person::names() const
{
    return m_names;
}

void Person::setNames(const QString &newNames)
{
    m_names = newNames;
}

QString Person::favoriteColor() const
{
    return m_favoriteColor;
}

void Person::setFavoriteColor(const QString &newFavoriteColor)
{
    m_favoriteColor = newFavoriteColor;
}

int Person::age() const
{
    return m_age;
}

void Person::setAge(int newAge)
{
    m_age = newAge;
}

int Person::socialScore() const
{
    return m_socialScore;
}

void Person::setSocialScore(int newSocialScore)
{
    m_socialScore = newSocialScore;
}
