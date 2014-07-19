#include <QtTest>
#include <QDebug>
#include "testreflectclass.h"
#include "testreflectionsystem.h"
#include "repository.h"
#include "sqliteprovider.h"
#include "book.h"
#include "usuario.h"

#include <QFile>
#include <QTextStream>

using namespace std;

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

   Repository::registerProvider(new SqliteProvider("Teste.sqlite"));
   auto repository = Repository::createRepository();

//   if(repository->createTable<Usuario>())
//   {
//       qDebug() << "Tabela criada com sucesso!";
//   }
//   else
//   {
//       qDebug() << "Erro na criação da tabela.";
//       qDebug() << repository->lastError()->description();
//       if(repository->lastError()->errorType() == DatabaseError)
//       {
//           qDebug() << repository->lastError()->sqlError().text();
//       }
//   }

//   return 0;

   Usuario usuario;
   usuario.setNome("Michael");
   usuario.setEmail("michael");
   usuario.setInscrito(QDate::currentDate());
   repository->saveObject<Usuario>(usuario);

   QList<Usuario> usuarios;
   if(repository->select<Usuario>(usuarios))
   {
       for(int i=0; i<usuarios.count(); ++i)
       {
            Usuario& usuario = usuarios[i];
            qDebug() << QString("%1 - %2").arg(usuario.codigo()).arg(usuario.nome());
            if(repository->deleteObject<Usuario>(usuario))
            {
                qDebug() << "Usuario deletado";
            }
            else
            {
                qDebug() << "Erro na deleção";
                qDebug() << repository->lastError()->description();
                if(repository->lastError()->errorType() == DatabaseError)
                {
                    qDebug() << repository->lastError()->sqlError().text();
                }
            }
            if(repository->saveObject<Usuario>(usuario))
            {
                qDebug() << "Usuário recriado";
                qDebug() << QString("%1 - %2").arg(usuario.codigo()).arg(usuario.nome());
            }
            else
            {
                qDebug() << "Erro na recriação";
                qDebug() << repository->lastError()->description();
                if(repository->lastError()->errorType() == DatabaseError)
                {
                    qDebug() << repository->lastError()->sqlError().text();
                }
            }
            usuario.setNome("Geralda");
            if(repository->saveObject<Usuario>(usuario))
            {
                qDebug() << "Usuário mudado";
                qDebug() << QString("%1 - %2").arg(usuario.codigo()).arg(usuario.nome());
            }
            else
            {
                qDebug() << "Erro na recriação";
                qDebug() << repository->lastError()->description();
                if(repository->lastError()->errorType() == DatabaseError)
                {
                    qDebug() << repository->lastError()->sqlError().text();
                }
            }
       }
   }
   else
   {
       qDebug() << "Erro na criação da tabela.";
       qDebug() << repository->lastError()->description();
       if(repository->lastError()->errorType() == DatabaseError)
       {
           qDebug() << repository->lastError()->sqlError().text();
       }
   }

   return 0;

}
