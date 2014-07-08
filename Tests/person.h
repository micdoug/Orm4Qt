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
