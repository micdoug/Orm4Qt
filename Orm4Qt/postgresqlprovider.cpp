#include "postgresqlprovider.h"

using namespace Orm4Qt;


PostgreSqlProvider::PostgreSqlProvider(const QString &user, const QString &password, const QString &database,
                                       const QString &host, int port) :
    StandardSqlProvider()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", databaseConnectionName());
    db.setDatabaseName(database);
    db.setHostName(host);
    db.setUserName(user);
    db.setPassword(password);
    db.setPort(port);
}

PostgreSqlProvider::~PostgreSqlProvider()
{}

std::shared_ptr<QSqlQuery> PostgreSqlProvider::generateCreateTable(Class *reflect)
{
    return nullptr;
}
