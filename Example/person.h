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

#include <QString>
#include <QDate>
#include <QByteArray>
#include "annotations.h"

class Person
{
public:
    //Virtual destructor
    virtual ~Person();

    //Getters and setters
    QString name() const;
    void setName(const QString &name);
    QString email() const;
    void setEmail(const QString &email);
    QDate birthday() const;
    void setBirthday(const QDate &birthday);
    double balance() const;
    void setBalance(double balance);
    unsigned int age() const;
    void setAge(unsigned int age);
    QByteArray resume() const;
    void setResume(const QByteArray &resume);

private:
    //Private attributes
    QString m_name;
    QString m_email;
    QDate m_birthday;
    double m_balance;
    unsigned int m_age;
    QByteArray m_resume;

    //Annotations
    ORM4QT_BEGIN
        CLASS(name="Person", table="people", autoid="id", versioncolumn="version")
        PROPERTY(m_name, name="name", column="name", required=true, maxlength=200, unique=true, display="Name")
        PROPERTY(m_email, name="email", column="email", required=false, maxlenght=100, display="Email")
        PROPERTY(m_birthday, name="birthday", column="birthday", required=true, display="Birthday")
        PROPERTY(m_balance, name="balance", column="balance", display="Balance")
        PROPERTY(m_age, name="age", column="age", display="Age")
        PROPERTY(m_resume, name="resume", column="resume", required=false, display="Resumé")
    ORM4QT_END
};

#endif // PERSON_H
