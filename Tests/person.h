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

#ifndef PERSON_H
#define PERSON_H

#include "annotations.h"
#include <QString>
#include <string>

Q_DECLARE_METATYPE(std::string)

class Person
{
public:
    int id() const;
    void setId(int id);

    int *age() const;
    void setAge(int *age);

    QString name() const;
    void setName(const QString &name);

    QString *email() const;
    void setEmail(QString *email);

    double pay() const;
    void setPay(double pay);

    std::string address() const;
    void setAddress(const std::string &address);

private:
    int m_id;
    int *m_age = nullptr;
    QString m_name;
    QString *m_email = nullptr;
    double m_pay;
    std::string m_address;

    ORM4QT_BEGIN
    CLASS(name="Person", table="People")
    PROPERTY(m_id, column="id", name="id")
    PROPERTY(m_age, column="age")
    PROPERTY(m_name, column="name")
    PROPERTY(m_email, column="email")
    PROPERTY(m_pay, column="pay")
    PROPERTY(m_address, column="address")
    ORM4QT_END
};

#endif // PERSON_H
