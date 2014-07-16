#include <QtTest>
#include <QDebug>
#include "testreflectclass.h"
#include "testreflectionsystem.h"
#include "repository.h"
#include "sqliteprovider.h"
#include "book.h"

#include <QFile>

using namespace std;

void teste(Book b)
{
    qDebug() << b.reflection()->tags()["id"];
}

int main(int argc, char **argv)
{
   Q_UNUSED(argc);
   Q_UNUSED(argv);

   /*//Testing the Reflect class
   TestReflectClass *treflect = new TestReflectClass();
   QTest::qExec(treflect);
   delete treflect;

   //Testing the class and property reflection system
   TestReflectionSystem *treflectionsystem = new TestReflectionSystem();
   QTest::qExec(treflectionsystem);
   delete treflectionsystem;*/

   Repository::registerProvider(new SqliteProvider("Teste.db"));

   auto repository = Repository::createRepository();
   //Book b;
   /*if(repository->createTable<Book>())
   {
       qDebug() << "Table created.";
   }
   else
   {
       qDebug() << repository->lastError()->description();
       qDebug() << repository->lastError()->sqlError().text();
   }*/
   /*QFile file("E:/Usuarios/michael/Pictures/Unix.png");
   if(file.open(QIODevice::ReadOnly))
   {
       b.setPhoto(file.readAll());
       b.setName("The C++ Programming Language");
       b.setEdition(4);
       b.setPublished(QDateTime::currentDateTime());
       if(repository->saveObject<Book>(b))
       {
           qDebug() << "Object saved";
           qDebug() << b.reflection()->tags()["id"];
       }
       else
       {
           qDebug() << repository->lastError()->description();
           qDebug() << repository->lastError()->sqlError().text();
       }
   }
   else
   {
       qDebug() << "Cannot open the file.";
   }*/
   QList<Book> list;
   if(repository->select(list, Where("autoid", WhereOp::Equals, {2})))
   {
       qDebug() << "Select Ok";
       Book book = list.first();
       book.setName("The android operation system");
       qDebug() << book.name();
       book.reflection()->properties()[0]->setValue("teste");
       qDebug() << book.name();
       //repository->saveObject<Book>(book);
   }
   else
   {
       qDebug() << repository->lastError()->description();
       qDebug() << repository->lastError()->sqlError().text();
   }

   return 0;
}
