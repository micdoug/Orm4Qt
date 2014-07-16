#include "sqlprovider.h"

using namespace Orm4Qt;

SqlProvider::SqlProvider(const QString &connectionName) :
    m_databaseConnectionName(connectionName)
{}

SqlProvider::SqlProvider(const SqlProvider &other) : SqlProvider(other.m_databaseConnectionName)
{}

SqlProvider::~SqlProvider()
{}

QString SqlProvider::databaseConnectionName() const
{
    return m_databaseConnectionName;
}

void SqlProvider::setDatabaseConnectionName(const QString &databaseConnectionName)
{
    m_databaseConnectionName = databaseConnectionName;
}

shared_ptr<OrmError> SqlProvider::lastError() const
{
    return m_lastError;
}

