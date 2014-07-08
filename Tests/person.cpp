#include "person.h"

int Person::id() const
{
    return m_id;
}

void Person::setId(int id)
{
    m_id = id;
}
int *Person::age() const
{
    return m_age;
}

void Person::setAge(int *age)
{
    m_age = age;
}
QString Person::name() const
{
    return m_name;
}

void Person::setName(const QString &name)
{
    m_name = name;
}
QString *Person::email() const
{
    return m_email;
}

void Person::setEmail(QString *email)
{
    m_email = email;
}
double Person::pay() const
{
    return m_pay;
}

void Person::setPay(double pay)
{
    m_pay = pay;
}

std::string Person::address() const
{
    return m_address;
}

void Person::setAddress(const std::string &address)
{
    m_address = address;
}

