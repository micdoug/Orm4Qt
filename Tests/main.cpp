#include <QtTest>
#include <QDebug>
#include "testreflectclass.h"
#include "testreflectionsystem.h"
#include "repository.h"
#include "sqliteprovider.h"
#include "book.h"
#include "usuario.h"

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

    QList<Usuario> usuarios;
    if(repository->select<Usuario>(usuarios, Where("Nome", WhereOp::StartsWith, {"M"}, WherePrecede::Begin).
                                   And(Where("Email", WhereOp::Contains, {"2"}, WherePrecede::End)).
                                   Or(Where("Codigo", WhereOp::In, {1, 2, 3}))))
    {
        for(Usuario user: usuarios)
        {
            qDebug() << QString("%1 \t %2 \t %3").arg(user.codigo())
                        .arg(user.nome()).arg(user.email());
            user.setPontos(15.75);
            if(repository->saveObject<Usuario>(user))
            {
                qDebug() << "Registro atualizado";
            }
            else
            {
                qDebug() << repository->lastError()->description();
                if(repository->lastError()->errorType() == ErrorType::DatabaseError)
                    qDebug() << repository->lastError()->sqlError().text();
            }
        }
    }
    else
    {
        qDebug() << repository->lastError()->description();
        if(repository->lastError()->errorType() == ErrorType::DatabaseError)
            qDebug() << repository->lastError()->sqlError().text();
    }

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
    }
    QList<Book> list;
    if(repository->select(list, Where("Name", WhereOp::StartsWith, {"t"})))
    {
        qDebug() << "Select Ok";
        Book book = list.first();
//        book.setPhoto(list.first().photo());
//        book.setName("teste");
//        book.setPublished(QDateTime::currentDateTime());
//        if(!repository->saveObject<Book>(book))
//        {
//            qDebug() << repository->lastError()->description();
//            if(repository->lastError()->errorType() == ErrorType::DatabaseError)
//                qDebug() << repository->lastError()->sqlError().text();
//        }
        book.setName("The new book");
        if(repository->saveObject<Book>(book))
        {
            qDebug() << "Ok";
        }
        else
        {
            qDebug() << repository->lastError()->description();
        }

    }
    else
    {
        qDebug() << repository->lastError()->description();
        qDebug() << repository->lastError()->sqlError().text();
    }*/



   return 0;
}
