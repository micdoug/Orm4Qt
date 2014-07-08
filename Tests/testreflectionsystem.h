#ifndef TESTREFLECTIONSYSTEM_H
#define TESTREFLECTIONSYSTEM_H

#include "person.h"
#include <QtTest>

class TestReflectionSystem : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void validatePropertiesNameAndType();
    void getSetNonPointerValues();
    void getSetPointerValues();
    void getSetQVariantNonSupportedTypes();

private:
    Person m_person;
};

#endif // TESTREFLECTIONSYSTEM_H
