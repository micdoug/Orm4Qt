#include <QSqlDatabase>
#include <QTextStream>
#include "sqliteprovider.h"

using namespace Orm4Qt;

SqliteProvider::SqliteProvider(const QString &dbFileName, const QString &connectionName) : SqlProvider(connectionName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(dbFileName);
}

SqliteProvider::SqliteProvider(const SqliteProvider &other) : SqlProvider(other.databaseConnectionName())
{}

SqliteProvider::~SqliteProvider()
{}

shared_ptr<QSqlQuery> SqliteProvider::generateInsert(Class *reflect, const QList<int> &fieldsno)
{
    //Create the string to store the sql command
    QString sqlstr;
    QTextStream sql(&sqlstr);
    QString columnsstr;
    QTextStream columns(&columnsstr);
    QString valuesstr;
    QTextStream values(&valuesstr);
    QMap<QString, QVariant> bindings;

    //Insert the prefix of the command
    sql << "INSERT INTO ";

    //Add table name
    if(!reflect->tags()["table"].isNull())
    {
        sql << reflect->tags()["table"].toString() << " (";
    }
    else
    {
        m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'table' was not supplied for the class '%1'.").arg(reflect->tags()["name"].toString())));
        return nullptr;
    }//End add table name

    shared_ptr<QSqlQuery> query(new QSqlQuery(QSqlDatabase::database(databaseConnectionName())));
    for(int index : fieldsno)
    {
        Property *prop = reflect->properties()[index];
        if(prop->tags()["column"].isNull())
        {
            m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'column' was not supplied for the property '%1' in the class '%2'.")
                                                            .arg(prop->tags()["name"].toString())
                                                            .arg(reflect->tags()["name"].toString())));
            return nullptr;
        }
        else
        {
            columns << prop->tags()["column"].toString() << ", ";
            values << prop->tags()["column"].toString().prepend(":") << ", ";
            bindings.insert(prop->tags()["column"].toString().prepend(":"), prop->value());
        }

    }
    //Insert version column
    QString versioncolumn = reflect->tags()["versioncolumn"].toString();
    if(!versioncolumn.isEmpty())
    {
        columns << versioncolumn << ", ";
        values << ":" << versioncolumn << ", ";
        bindings.insert(versioncolumn.prepend(":"), reflect->tags()["version"]);
    }
    sql << columnsstr.left(columnsstr.size()-2) << ") VALUES(";
    sql << valuesstr.left(valuesstr.size()-2) << ");";


    //Testing query
    if(query->prepare(sqlstr))
    {
        for(auto iter=bindings.begin(); iter!=bindings.end(); ++iter)
        {
            query->bindValue(iter.key(), iter.value());
        }
        return query;
    }
    else
    {
        m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::DatabaseError, QString("Invalid SQL statement for insert a row. See the sqlerror attached."), query->lastError()));
        return nullptr;
    }
}

shared_ptr<QSqlQuery> SqliteProvider::generateUpdate(Class *reflect, const QList<int> &fieldsno)
{
    //Objects to store the sql command
    QString sqlstr;
    QTextStream sql(&sqlstr);

    //Begin the command
    sql << "UPDATE ";

    //Insert the table name
    if(!reflect->tags()["table"].isNull())
    {
        sql << reflect->tags()["table"].toString() << " SET ";
    }
    else
    {
        m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'table' was not supplied for the class '%1'.").arg(reflect->tags()["name"].toString())));
        return nullptr;
    }//End insert the table name

    //Iterating over the fields
    QMap<QString, QVariant> bindings;
    for(int index: fieldsno)
    {
        Property *prop = reflect->properties()[index];
        //Add the set statements
        if(!prop->tags()["column"].isNull())
        {
            sql << prop->tags()["column"].toString()
                << " = "
                << prop->tags()["column"].toString().prepend(":")
                << ", ";
            QVariant value = prop->value();
            bindings.insert(prop->tags()["column"].toString().prepend(":"), value);
        }
        else
        {
            m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'column' was not supplied for the property '%1' in the class '%2'.")
                                                            .arg(prop->tags()["name"].toString())
                                                            .arg(reflect->tags()["name"].toString())));
            return nullptr;
        }//End add set statements
    }//End iterate over the fields

    //Add the new version
    if(!reflect->tags()["newversion"].isNull())
    {
        sql << reflect->tags()["versioncolumn"].toString()
            << " = "
            << reflect->tags()["versioncolumn"].toString().prepend(":new")
            << ", ";
        bindings.insert(reflect->tags()["versioncolumn"].toString().prepend(":new"), reflect->tags()["newversion"]);
    }

    //Include the where clause
    sqlstr = sqlstr.left(sqlstr.size()-2) + " ";
    sql << "WHERE ";
    //Add the autoid
    if(!reflect->tags()["autoid"].isNull())
    {
        sql << reflect->tags()["autoid"].toString()
            << " = "
            << reflect->tags()["autoid"].toString().prepend(":")
            << " AND ";
        bindings.insert(reflect->tags()["autoid"].toString().prepend(":"), reflect->tags()["id"]);
    }

    //Add the version check
    if(!reflect->tags()["versioncolumn"].isNull())
    {
        sql << reflect->tags()["versioncolumn"].toString()
            << " = "
            << reflect->tags()["versioncolumn"].toString().prepend(":")
            << " AND ";
        bindings.insert(reflect->tags()["versioncolumn"].toString().prepend(":"), reflect->tags()["version"]);
    }

    QString autocolumn = reflect->tags()["autocolumn"].toString();

    //Iterating over the keys
    for(Property *prop: reflect->properties())
    {
        if(prop->tags()["key"].toBool() || (!autocolumn.isNull() && prop->tags()["name"].toString()==autocolumn))
        {
            if(prop->tags()["column"].isNull())
            {
                m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'column' was not supplied for the property '%1' in the class '%2'.")
                                                                .arg(prop->tags()["name"].toString())
                                                                .arg(reflect->tags()["name"].toString())));
            }
            else
            {
                sql << prop->tags()["column"].toString()
                    << " = "
                    << prop->tags()["column"].toString().prepend(":")
                    << " AND ";
                bindings.insert(prop->tags()["column"].toString().prepend(":"), prop->value());
            }
        }
    }//End iterating over the keys

    sqlstr = sqlstr.left(sqlstr.size()-4);
    //Create the query object
    shared_ptr<QSqlQuery> query(new QSqlQuery(QSqlDatabase::database(databaseConnectionName())));
    //Testing query
    if(query->prepare(sqlstr))
    {
        for(auto iter=bindings.begin(); iter!=bindings.end(); ++iter)
        {
            query->bindValue(iter.key(), iter.value());
        }
        return query;
    }
    else
    {
        m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::DatabaseError, QString("Invalid SQL statement for update a row. See the sqlerror attached."), query->lastError()));
        return nullptr;
    }
}

shared_ptr<QSqlQuery> SqliteProvider::generateDelete(Class *reflect)
{
    return nullptr;
}

shared_ptr<QSqlQuery> SqliteProvider::generateSelect(Class *reflect, const Where &where, const QList<int> &fieldsno)
{
    //Objects to store the sql command
    QString sqlstr;
    QTextStream sql(&sqlstr);

    //Begin the statement
    sql << "SELECT ";

    //Iterating the properties
    for(int index: fieldsno)
    {
        Property *prop = reflect->properties()[index];
        //Check if the property has the column tag
        if(prop->tags()["column"].isNull())
        {
            m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'column' was not supplied for the property '%1' in the class '%2'.")
                                                            .arg(prop->tags()["name"].toString())
                                                            .arg(reflect->tags()["name"].toString())));
            return nullptr;
        }
        else
        {
            //Add the column in the select
            sql << prop->tags()["column"].toString() << ", ";
        }//End check if the property has the column tag
    }
    //Insert the autoid column
    if(!reflect->tags()["autoid"].isNull())
    {
        sql << reflect->tags()["autoid"].toString() << ", ";
    }
    //Insert the version column
    if(!reflect->tags()["versioncolumn"].isNull())
    {
        sql << reflect->tags()["versioncolumn"].toString() << ", ";
    }

    //Remove the last ", " from the command
    sqlstr = sqlstr.left(sqlstr.size()-2);

    //Insert the table name
    sql << " FROM ";
    if(reflect->tags()["table"].isNull())
    {
        m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'table' was not supplied for the class '%1'.").arg(reflect->tags()["name"].toString())));
        return nullptr;
    }
    else
    {
        sql << reflect->tags()["table"].toString() << " ";
    }//End insert the table name

    //Check if where is used
    QMap<QString, QVariant> bindings; //The list of bindings used int the where clause
    if(where.operation() != WhereOp::NoOperation)
    {
        //Init the where clause
        QString wherestr = "WHERE ";
        const Where *whereptr = &where;
        //Iterating over the where clauses joined
        while(whereptr)
        {
            QString field; //Store the name of the field used in the comparison

            //Check if the field is the autoid
            if(whereptr->field()=="autoid")
            {
                field = reflect->tags()["autoid"].toString();
            }
            //Check if the field is one of the properties in the object
            else
            {
                //Iterating over the properties  to be selected
                for(int index: fieldsno)
                {
                    Property *prop = reflect->properties()[index];
                    if(prop->tags()["name"].toString()==whereptr->field())
                    {
                        field = prop->tags()["column"].toString();
                        break;
                    }
                } //End iterating over the properties to be selected
            }

            //Check if the field name is valid
            if(field.isNull())
            {
                //If the field is invalid ignore this clause and check the next
                whereptr = whereptr->joined();
                continue;
            }
            //Field is valid
            else
            {
                //Check if is necessary to open a block of precedence
                if(whereptr->precede()==WherePrecede::Begin)
                    wherestr += "(";

                //Include the name of the field in the where clause
                wherestr += field + " ";
            }

            //Prepare the field to be used as a placeholder
            field.prepend(":");
            //Char used to diff multiple arguments
            char diff = 'a';
            //Check the type of operation
            switch(whereptr->operation())
            {
                case WhereOp::Equals:
                    wherestr += QString("= %1 ").arg(field);
                    bindings.insert(field, whereptr->arguments().first());
                    break;
                case WhereOp::NotEquals:
                    wherestr += QString("<> %1 ").arg(field);
                    bindings.insert(field, whereptr->arguments().first());
                    break;
                case WhereOp::GreaterThan:
                    wherestr += QString("> %1 ").arg(field);
                    bindings.insert(field, whereptr->arguments().first());
                    break;
                case WhereOp::GreaterEqual:
                    wherestr += QString(">= %1 ").arg(field);;
                    bindings.insert(field, whereptr->arguments().first());
                    break;
                case WhereOp::LessThan:
                    wherestr += QString("< %1 ").arg(field);
                    bindings.insert(field, whereptr->arguments().first());
                    break;
                case WhereOp::LessEqual:
                    wherestr += QString("<= %1 ").arg(field);
                    bindings.insert(field, whereptr->arguments().first());
                    break;
                case WhereOp::Contains:
                    wherestr += QString("LIKE '%%1%' ").arg(whereptr->arguments().first().toString());
                    break;
                case WhereOp::EndsWith:
                    wherestr += QString("LIKE '%1%2' ").arg("%").arg(whereptr->arguments().first().toString());
                    break;
                case WhereOp::StartsWith:
                    wherestr += QString("LIKE '%1%2' ").arg(whereptr->arguments().first().toString()).arg("%");
                    break;
                case WhereOp::Like:
                    wherestr += QString("LIKE '%1'").arg(whereptr->arguments().first().toString());
                    break;
                case WhereOp::In:
                    wherestr += QString("IN (");
                    for(QVariant value: whereptr->arguments())
                    {
                        wherestr += QString("%1, ").arg(field+diff);
                        bindings.insert(field+diff, value);
                        ++diff;
                    }
                    wherestr = wherestr.left(wherestr.size()-2)+") ";
                    break;
                case WhereOp::NotIn:
                    wherestr += QString("NOT IN (");
                    for(QVariant value: whereptr->arguments())
                    {
                        wherestr += QString("%1, ").arg(field+diff);
                        bindings.insert(field+diff, value);
                        ++diff;
                    }
                    wherestr = wherestr.left(wherestr.size()-2)+") ";
                    break;
                default:
                    m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::OperationNotSupported, QString("This type of comparison is not supported by this sql provider.")));
                    return nullptr;
            } //End check the type of operation

            //Check if is necessary to close a block of precedence
            if(whereptr->precede()==WherePrecede::End)
                wherestr = wherestr.trimmed() + ") ";

            //Check if this where is joined
            switch(whereptr->join())
            {
                case WhereJoin::And:
                    wherestr += "AND ";
                    break;
                case WhereJoin::Or:
                    wherestr += "OR ";
                    break;
                case WhereJoin::NoJoin:
                    whereptr = nullptr;
                    continue;
            } //Check if this where is joined

            //Adjust the next where clause to analize
            whereptr = whereptr->joined();
        } //End iterating over the where clauses joined

        //Include the where clause in the sql statement
        sql << wherestr;
    } //End check if where is used

    //Create a new query object
    shared_ptr<QSqlQuery> query(new QSqlQuery(QSqlDatabase::database(databaseConnectionName())));

    //Check if the sql statement is valid
    if(query->prepare(sqlstr))
    {
        //Bind the values registered in the where clause
        for(auto iter=bindings.begin(); iter!=bindings.end(); ++iter)
        {
            query->bindValue(iter.key(), iter.value());
        } //End bind the values registered in the where clause

        //Return the query
        return query;
    }
    //If the sql statement is invalid raise an error
    else
    {
        m_lastError = shared_ptr<OrmError>(new OrmError(ErrorType::DatabaseError, QString("Invalid SQL statement for select. See the sqlerror attached."), query->lastError()));
        return nullptr;
    }

}

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

SqlProvider *SqliteProvider::clone()
{
    return new SqliteProvider(*this);
}
