#include <QString>
#include <QtTest>
#include "reflect.h"

using namespace Orm4Qt;

class TestsTest : public QObject
{
    Q_OBJECT

public:
    TestsTest();

private Q_SLOTS:
    void reflectInsertingTags();
    void reflectReplacingTags();
    void reflectRemovingTags();

private:
    Reflect m_ref;
};

TestsTest::TestsTest(){}

void TestsTest::reflectInsertingTags()
{
    QVERIFY2(m_ref.addTag("table", "person"), "Error on inserting tag 'table'.");
    QVERIFY2(m_ref.addTag("primary", true), "Error on inserting tag 'primary'");
    QVERIFY2(m_ref.addTag("count", 12000), "Error on inserting tag 'count'.");
    QVERIFY2(m_ref.addTag("money", 14.25), "Error on inserting tag 'money'.");

    QVERIFY2(m_ref.tags()["table"].toString() == "person", "Tag 'table' wasn't inserted.");
    QVERIFY2(m_ref.tags()["primary"].toBool(), "Tag 'primary' wasn't inserted.");
    QVERIFY2(m_ref.tags()["count"].toInt() == 12000, "Tag 'count' wasn't inserted.");
    QVERIFY2(m_ref.tags()["money"].toDouble() == 14.25, "Tag 'money' wasn't inserted.");

    QVERIFY2(!m_ref.addTag("table", ""), "Error on checking if tag exists before adding.");
}

void TestsTest::reflectReplacingTags()
{
    QVERIFY2(m_ref.replaceTag("table", "other"), "Error on replacing tag 'table'");
    QVERIFY2(m_ref.replaceTag("count", 111), "Error on replacing tag 'count'.");

    QVERIFY2(m_ref.tags()["table"].toString() == "other", "Tag 'table' wasn't replaced.");
    QVERIFY2(m_ref.tags()["count"].toInt() == 111, "Tag 'count' wasn't replaced.");

    QVERIFY2(!m_ref.replaceTag("nonexisting", ""), "Error on checking if tag exists before replacing.");
}

void TestsTest::reflectRemovingTags()
{
    QVERIFY2(m_ref.removeTag("table"), "Error on removing tag 'table'.");
    QVERIFY2(m_ref.removeTag("money"), "Error on removing tag 'money'.");

    QVERIFY2(!m_ref.tags().contains("table"), "Tag 'table' wasn't removed.");
    QVERIFY2(!m_ref.tags().contains("money"), "Tag 'table' wasn't removed.");

    QVERIFY2(!m_ref.removeTag("nonexisting"), "Error on checking if tag exists before removing.");
}



QTEST_APPLESS_MAIN(TestsTest)

#include "tst_teststest.moc"
