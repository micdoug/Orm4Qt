#include "testreflectionsystem.h"

void TestReflectionSystem::initTestCase()
{
    m_person.setAddress("");
    m_person.setAge(new int(0));
    m_person.setEmail(new QString(""));
    m_person.setId(0);
    m_person.setName("");
    m_person.setPay(0.0);
}

void TestReflectionSystem::validatePropertiesNameAndType()
{
    shared_ptr<Orm4Qt::Class> ref = m_person.reflection();
    //Property m_id
    QVERIFY2(ref->properties()[0]->tags()["name"].toString()=="id", QString("Error on validating name of property '%1'. The name returned was '%2'.")
            .arg("id").arg(ref->properties()[0]->tags()["name"].toString()).toLocal8Bit().data());
    QVERIFY2(ref->properties()[0]->type() == QMetaType::Int && !ref->properties()[0]->isPointer(), QString("Error on validating the type of property '%1'.").arg("id").toLocal8Bit().data());
    //Property m_age
    QVERIFY2(ref->properties()[1]->tags()["name"].toString()=="m_age", QString("Error on validating name of property '%1'. The name returned was '%2'.")
            .arg("m_age").arg(ref->properties()[1]->tags()["name"].toString()).toLocal8Bit().data());
    QVERIFY2(ref->properties()[1]->type() == QMetaType::Int && ref->properties()[1]->isPointer(), QString("Error on validating the type of property '%1'.").arg("m_age").toLocal8Bit().data());
    //Property m_name
    QVERIFY2(ref->properties()[2]->tags()["name"].toString()=="m_name", QString("Error on validating name of property '%1'. The name returned was '%2'.")
            .arg("m_name").arg(ref->properties()[2]->tags()["name"].toString()).toLocal8Bit().data());
    QVERIFY2(ref->properties()[2]->type() == QMetaType::QString && !ref->properties()[2]->isPointer(), QString("Error on validating the type of property '%1'.").arg("m_name").toLocal8Bit().data());
    //Property m_email
    QVERIFY2(ref->properties()[3]->tags()["name"].toString()=="m_email", QString("Error on validating name of property '%1'. The name returned was '%2'.")
            .arg("m_email").arg(ref->properties()[3]->tags()["name"].toString()).toLocal8Bit().data());
    QVERIFY2(ref->properties()[3]->type() == QMetaType::QString && ref->properties()[3]->isPointer(), QString("Error on validating the type of property '%1'.").arg("m_email").toLocal8Bit().data());
    //Property m_pay
    QVERIFY2(ref->properties()[4]->tags()["name"].toString()=="m_pay", QString("Error on validating name of property '%1'. The name returned was '%2'.")
            .arg("m_pay").arg(ref->properties()[4]->tags()["name"].toString()).toLocal8Bit().data());
    QVERIFY2(ref->properties()[4]->type() == QMetaType::Double && !ref->properties()[4]->isPointer(), QString("Error on validating the type of property '%1'.").arg("m_pay").toLocal8Bit().data());
    //Property m_address
    QVERIFY2(ref->properties()[5]->tags()["name"].toString()=="m_address", QString("Error on validating name of property '%1'. The name returned was '%2'.")
            .arg("m_address").arg(ref->properties()[5]->tags()["name"].toString()).toLocal8Bit().data());
    QVERIFY2(ref->properties()[5]->type() == QMetaType::User && !ref->properties()[5]->isPointer() && QString(QMetaType::typeName(ref->properties()[5]->type())) == "std::string",
            QString("Error on validating the type of property '%1'.").arg("m_address").toLocal8Bit().data());
}

void TestReflectionSystem::getSetNonPointerValues()
{
    shared_ptr<Orm4Qt::Class> ref = m_person.reflection();
    //Int m_id property
    QVERIFY2(ref->properties()[0]->value().toInt() == 0, QString("Initial value of property '%1' wasn't validated.").
            arg(ref->properties()[0]->tags()["name"].toString()).toLocal8Bit().data());
    ref->properties()[0]->setValue(555);
    QVERIFY2(ref->properties()[0]->value().toInt() == 555 && m_person.id() == 555, QString("Value of property '%1' wasn't adjusted via reflection system.").
            arg(ref->properties()[0]->tags()["name"].toString()).toLocal8Bit().data());

    //QString m_name property
    QVERIFY2(ref->properties()[2]->value().toString() == "", QString("Initial value of property '%1' wasn't validated.").
            arg(ref->properties()[2]->tags()["name"].toString()).toLocal8Bit().data());
    ref->properties()[2]->setValue("Michael Dougras");
    QVERIFY2(ref->properties()[2]->value().toString() == "Michael Dougras" && m_person.name() == "Michael Dougras", QString("Value of property '%1' wasn't adjusted via reflection system.").
            arg(ref->properties()[2]->tags()["name"].toString()).toLocal8Bit().data());

    //Double m_pay property
    QVERIFY2(ref->properties()[4]->value().toDouble() == 0.0, QString("Initial value of property '%1' wasn't validated.").
            arg(ref->properties()[4]->tags()["name"].toString()).toLocal8Bit().data());
    ref->properties()[4]->setValue(7.5);
    QVERIFY2(ref->properties()[4]->value().toDouble() == 7.5 && m_person.pay()==7.5, QString("Value of property '%1' wasn't adjusted via reflection system.").
            arg(ref->properties()[4]->tags()["name"].toString()).toLocal8Bit().data());
}

void TestReflectionSystem::getSetPointerValues()
{
    shared_ptr<Orm4Qt::Class> ref = m_person.reflection();
    //Int *m_age property
    QVERIFY2(ref->properties()[1]->value().toInt() == 0, QString("Initial value of property '%1' wasn't validated.").
            arg(ref->properties()[1]->tags()["name"].toString()).toLocal8Bit().data());
    ref->properties()[1]->setValue(444);
    QVERIFY2(ref->properties()[1]->value().toInt() == 444 && *m_person.age() == 444, QString("Value of property '%1' wasn't adjusted via reflection system.").
            arg(ref->properties()[1]->tags()["name"].toString()).toLocal8Bit().data());
    //QString *m_email property
    QVERIFY2(ref->properties()[3]->value().toString() == "", QString("Initial value of property '%1' wasn't validated.").
            arg(ref->properties()[3]->tags()["name"].toString()).toLocal8Bit().data());
    ref->properties()[3]->setValue("example@email.com");
    QVERIFY2(ref->properties()[3]->value().toString() == "example@email.com" && *m_person.email() == "example@email.com",
            QString("Value of property '%1' wasn't adjusted via reflection system.").
            arg(ref->properties()[3]->tags()["name"].toString()).toLocal8Bit().data());
}

void TestReflectionSystem::getSetQVariantNonSupportedTypes()
{
    shared_ptr<Orm4Qt::Class> ref = m_person.reflection();
    //std::string m_address property
    QVERIFY2(ref->properties()[5]->value().value<std::string>() == "", QString("Initial value of property '%1' wasn't validated.").
            arg(ref->properties()[5]->tags()["name"].toString()).toLocal8Bit().data());
    ref->properties()[5]->setValue(QVariant::fromValue<std::string>("somewhere"));
    QVERIFY2(ref->properties()[5]->value().value<std::string>() == "somewhere", QString("Value of property '%1' wasn't adjusted via reflection system.").
            arg(ref->properties()[5]->tags()["name"].toString()).toLocal8Bit().data());
}


