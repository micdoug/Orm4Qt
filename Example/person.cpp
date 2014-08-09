/*
 * Orm4Qt - An Object Relational Mapping Library for the Qt Framework
 * Copyright (c) 2014, Michael Dougras da Silva, All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */

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





