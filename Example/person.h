#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QDate>
#include <QByteArray>
#include "annotations.h"

class Person
{
public:
    Person() = default;
    Person(const Person &other) = default;
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
