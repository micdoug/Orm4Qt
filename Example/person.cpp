#include "person.h"

Person::~Person()
{}

QString Person::name() const
{
    return m_name;
}

void Person::setName(const QString &name)
{
    m_name = name;
}
QString Person::email() const
{
    return m_email;
}

void Person::setEmail(const QString &email)
{
    m_email = email;
}
QDate Person::birthday() const
{
    return m_birthday;
}

void Person::setBirthday(const QDate &birthday)
{
    m_birthday = birthday;
}
double Person::balance() const
{
    return m_balance;
}

void Person::setBalance(double balance)
{
    m_balance = balance;
}
unsigned int Person::age() const
{
    return m_age;
}

void Person::setAge(unsigned int age)
{
    m_age = age;
}
QByteArray Person::resume() const
{
    return m_resume;
}

void Person::setResume(const QByteArray &resume)
{
    m_resume = resume;
}





