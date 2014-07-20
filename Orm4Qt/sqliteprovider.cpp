#include <QSqlDatabase>
#include <QTextStream>
#include "sqliteprovider.h"

using namespace Orm4Qt;

SqliteProvider::SqliteProvider(const QString &dbFileName) : StandardSqlProvider()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", databaseConnectionName());
    db.setDatabaseName(dbFileName);
}

SqliteProvider::~SqliteProvider()
{}

shared_ptr<QSqlQuery> SqliteProvider::generateCreateTable(Class *reflect)
{
    //Creating the strings to hold the sql commands
    QString sqlstr;
    QTextStream sql(&sqlstr);
    QString pkstr;
    QTextStream pk(&pkstr);
    sql << "CREATE TABLE ";
    pk << "PRIMARY KEY (";

    //Add table name
    if(reflect->tags().contains("table"))
    {
        sql << reflect->tags()["table"].toString().prepend("'").append("'");
        sql << " (";
    }
    else
    {
        m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'table' was not supplied for the class '%1'.").arg(reflect->tags()["name"].toString())));
        return nullptr;
    }//End add table name

    //Verify if autoid is used
    bool autoid = false;
    if(!reflect->tags()["autoid"].isNull())
    {
        sql << reflect->tags()["autoid"].toString().prepend("'").append("'") << " INTEGER PRIMARY KEY AUTOINCREMENT, ";
        autoid = true;
    }//End verify if autoid is used

    //Verify if version column is active
    if(!reflect->tags()["versioncolumn"].isNull())
    {
        sql << reflect->tags()["versioncolumn"].toString().prepend("'").append("'") << " INTEGER NOT NULL, ";
    }//End verify if version column is active

    //Verify if autocolumn is used
    QString autocolumn;
    if(!autoid)
    {
        autocolumn = reflect->tags()["autocolumn"].toString();
        autoid = true;
    }
    else if(!reflect->tags()["autocolumn"].isNull())
    {
        m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("Error in the definition of the table for the class '%1'. You can't use autoid and autocolumn at the same time.").arg(reflect->tags()["name"].toString())));
        return nullptr;
    }//End verify if autocolumn is used

    //Iterating the properties
    for(Property *prop : reflect->properties())
    {
        //Verifying if the tag column is present
        if(prop->tags()["column"].isNull())
        {
            m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'column' was not supplied for the property '%1' in the class '%2'.")
                                                            .arg(prop->tags()["name"].toString())
                                                            .arg(reflect->tags()["name"].toString())));
            return nullptr;
        } //End verify tag column
        //Adding the definition of the column in the sql command
        else
        {
            sql << prop->tags()["column"].toString().prepend("'").append("'") << " ";
            //Defining the type of the column
            switch(prop->type())
            {
                case QMetaType::Int:
                case QMetaType::UInt:
                case QMetaType::Short:
                case QMetaType::UShort:
                case QMetaType::Long:
                case QMetaType::ULong:
                case QMetaType::LongLong:
                case QMetaType::ULongLong:
                    sql << "INTEGER ";
                    break;
                case QMetaType::Float:
                case QMetaType::Double:
                    sql << "REAL ";
                    break;
                case QMetaType::QByteArray:
                    sql << "BLOB ";
                    break;
                case QMetaType::User:
                    m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The type of the property '%1' in the class '%2' is not supported.")
                                                                .arg(prop->tags()["name"].toString())
                                                                .arg(reflect->tags()["name"].toString())));
                    return nullptr;
                default:
                    sql << "TEXT ";
            } //End defining the type of the column

            //Check if the column is the autocolumn
            if(prop->tags()["name"].toString() == autocolumn)
            {
                sql << "PRIMARY KEY AUTOINCREMENT ";
            }
            //Check if the column is a key
            else if(prop->tags()["key"].toBool())
            {
                //If it was defined a autoincrement column, we can't add primary keys
                if(autoid)
                {
                    m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("Error in the property '%1'' in the class '%2'. Sqlite doesn't support "
                                                                                                           "composite primary key when autoincrement is used.")
                                                                    .arg(prop->tags()["name"].toString())
                                                                    .arg(reflect->tags()["name"].toString())));
                    return nullptr;
                }
                pk << prop->tags()["column"].toString().prepend("'").append("'") << ", ";
            } //End check if the column is a key

            //Check if the property is required or not
            if(!prop->tags()["required"].toBool() && prop->isPointer())
            {
                sql << "NULL ";
            }
            else
            {
                sql << "NOT NULL ";
            } //End check if the property is required or not

            //Check if the property is unique
            if(prop->tags()["unique"].toBool())
            {
                sql << "UNIQUE, ";
            }
            else
            {
                sql << ", ";
            } // End check if the property is unique

        }
    } // End iterating the properties

    //Check if it was added primary keys
    if(pk.string()->endsWith("("))
    {
        //Remove the string ", " from the sql command and close the command with ");"
        sqlstr = sqlstr.left(sqlstr.size()-2) + ");";
    }
    //If it was added primary keys
    else
    {
        //Close the pk command and merge the sql and pk command
        sql << pkstr.left(pkstr.size()-2) << "));";
    }

    //Create a query object using the database connection with the name supplied
    shared_ptr<QSqlQuery> query(new QSqlQuery(QSqlDatabase::database(databaseConnectionName())));
    //Check if the query is valid and can be exec
    if(query->prepare(sqlstr))
    {
        return query;
    }
    //If it doesn't, generate an error
    else
    {
        m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::DatabaseError, QString("Invalid SQL statement for create table. See the sqlerror attached."), query->lastError()));
        return nullptr;
    }
}
