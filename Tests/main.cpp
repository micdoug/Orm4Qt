#include <QtTest>
#include <QDebug>
#include "testreflectclass.h"
#include "testreflectionsystem.h"
#include "repository.h"
#include "sqliteprovider.h"
#include "postgresqlprovider.h"
#include "book.h"
#include "usuario.h"
#include <string>

#include <QFile>
#include <QTextStream>

//using namespace std;

QTextStream cin(stdin, QIODevice::ReadOnly);
QTextStream cout(stdout, QIODevice::WriteOnly);

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

   //Repository::registerProvider([]() { return new PostgreSqlProvider("postgres", "userlinux007#", "teste");});
   auto repository = new Repository(new PostgreSqlProvider("postgres", "userlinux007#", "teste"));

   Usuario usuario;
   usuario.setNome("Geralda Queiroz Silva");
   usuario.setEmail("geralda@gmail.com");
   usuario.setPontos(10.50);
   usuario.setInscrito(QDate::currentDate());

   if(repository->saveObject<Usuario>(usuario))
   {
       qDebug() << QString("%1 - %2 - %3").
                   arg(usuario.codigo()).
                   arg(usuario.nome()).
                   arg(usuario.email());
   }
   else
   {
       qDebug() << repository->lastError()->description();
       if(repository->lastError()->errorType() == DatabaseError)
       {
           qDebug() << repository->lastError()->sqlError().text();
       }
   }

   QList<Usuario> list;
   if(repository->select<Usuario>(list, {{"Nome", Asc}, {"Email", Desc}}, 10))
   {
       for(Usuario u: list)
       {
           qDebug() << QString("%1 \t %2").arg(u.codigo()).arg(u.nome());
       }
   }
   else
   {
       qDebug() << repository->lastError()->description();
       if(repository->lastError()->errorType() == DatabaseError)
       {
           qDebug() << repository->lastError()->sqlError().text();
       }
   }

   repository->createTable<Usuario>();
   repository->deleteObject<Usuario>(usuario);

   delete repository;
   return 0;
}
