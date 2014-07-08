#ifndef TESTREFLECTIONCLASS_H
#define TESTREFLECTIONCLASS_H

#include <QObject>
#include "reflect.h"
#include <QtTest>

using namespace Orm4Qt;

class TestReflectClass : public QObject
{
    Q_OBJECT
public:
    TestReflectClass();
private slots:
    void initTestCase();
    void addTags();
    void replaceTags();
    void removeTags();
    void addTagsFromString();
    void cleanupTestCase();
private:
    Reflect *m_reflect;
};

#endif // TESTREFLECTIONCLASS_H
