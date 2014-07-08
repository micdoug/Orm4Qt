#include <QtTest>
#include <QDebug>
#include "testreflectclass.h"
#include "testreflectionsystem.h"

using namespace std;

int main(int argc, char **argv)
{
   Q_UNUSED(argc);
   Q_UNUSED(argv);

   //Testing the Reflect class
   TestReflectClass *treflect = new TestReflectClass();
   QTest::qExec(treflect);
   delete treflect;

   //Testing the class and property reflection system
   TestReflectionSystem *treflectionsystem = new TestReflectionSystem();
   QTest::qExec(treflectionsystem);
   delete treflectionsystem;
}
