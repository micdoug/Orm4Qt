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

