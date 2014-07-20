#include "sqlprovider.h"
#include <QUuid>
#include <QSqlDatabase>

using namespace Orm4Qt;

/**
 * Constructor with initialization of the database connection name associated with this instance
 * of sql provider.
 * @param connectionName
 * The name of the database connection associated with this provider.
 */
SqlProvider::SqlProvider()
{
    while(true)
    {
        QUuid id = QUuid::createUuid();
        QSqlDatabase db = QSqlDatabase::database(id.toString());
        if(!db.isValid())
        {
            m_databaseConnectionName = id.toString();
            break;
        }
    }
}

/**
 * Close the database connection associated with this instance of sql provider.
 */
SqlProvider::~SqlProvider()
{
    {
        QSqlDatabase db = QSqlDatabase::database(m_databaseConnectionName, false);
        if(db.isValid())
        {
            if(db.isOpen())
            {
                db.close();
            }
        }
    }
    QSqlDatabase::removeDatabase(m_databaseConnectionName);
}

/**
 * Method for access the database connection name associated with this instance of the provider.
 * @return
 * The name of the connection associated with this instance of provider.
 */
QString SqlProvider::databaseConnectionName() const
{
    return m_databaseConnectionName;
}

/**
 * Method for access the object with the description of the last error generated by this instance of provider.
 * @return
 * The error descriptor.
 */
shared_ptr<OrmError> SqlProvider::lastError() const
{
    return m_lastError;
}

