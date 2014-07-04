#include <QString>
#include <QtTest>

class TestsTest : public QObject
{
    Q_OBJECT

public:
    TestsTest();

private Q_SLOTS:
    void testCase1();
};

TestsTest::TestsTest()
{
}

void TestsTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(TestsTest)

#include "tst_teststest.moc"
