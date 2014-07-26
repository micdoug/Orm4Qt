#include "standardsqlprovider.h"

using namespace Orm4Qt;

StandardSqlProvider::StandardSqlProvider() : SqlProvider()
{}

StandardSqlProvider::~StandardSqlProvider()
{}

std::shared_ptr<QSqlQuery> StandardSqlProvider::generateInsert(Class *reflect, const QList<int> &fieldsno)
{
    //Check if there are fields to insert
    if(fieldsno.isEmpty())
    {
        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::InvalidRequest, QString("There aren't fields to insert. Check if the name of the fields informed are valid.")));
        return nullptr;

    }//End Check if there are fields to insert

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
        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'table' was not supplied for the class '%1'.").arg(reflect->tags()["name"].toString())));
        return nullptr;
    }//End add table name

    //Add properties in the sql statemtent
    for(int index : fieldsno)
    {
        Property *prop = reflect->properties()[index];
        if(prop->tags()["column"].isNull())
        {
            m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'column' was not supplied for the property '%1' in the class '%2'.")
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
    }//End add properties in the sql statement

    //Insert version column
    if(!reflect->tags()["versioncolumn"].isNull())
    {
        QString versioncolumn = reflect->tags()["versioncolumn"].toString();
        columns << versioncolumn << ", ";
        values << ":" << versioncolumn << ", ";
        bindings.insert(versioncolumn.prepend(":"), reflect->tags()["version"]);
    }//End insert version column

    //Combine sql statement with columns and values
    sql << columnsstr.left(columnsstr.size()-2) << ") VALUES(";
    sql << valuesstr.left(valuesstr.size()-2) << ");";
    //End combine sql statement with columns and values

    //Create and validate query
    std::shared_ptr<QSqlQuery> query(new QSqlQuery(QSqlDatabase::database(databaseConnectionName())));
    if(query->prepare(sqlstr))
    {
        //Bind values of the insert
        for(auto iter=bindings.begin(); iter!=bindings.end(); ++iter)
        {
            query->bindValue(iter.key(), iter.value());
        }//End bind values of the insert

        //Return the query object
        return query;
    }
    else
    {
        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::DatabaseError, QString("Invalid SQL statement for insert a row. See the sqlerror attached."), query->lastError()));
        return nullptr;
    }//End create and validate query
}

std::shared_ptr<QSqlQuery> StandardSqlProvider::generateUpdate(Class *reflect, const QList<int> &fieldsno)
{
    //Check if there are fields to update
    if(fieldsno.isEmpty())
    {
        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::InvalidRequest, QString("There aren't fields to update. Check if the name of the fields informed are valid. In the update operation the autoid, autocolumn and primary keys properties are ignored.")));
        return nullptr;

    }//End Check if there are fields to update

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
        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'table' was not supplied for the class '%1'.").arg(reflect->tags()["name"].toString())));
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
            m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'column' was not supplied for the property '%1' in the class '%2'.")
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
    }//End add the new version

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

    //QString autocolumn = reflect->tags()["autocolumn"].toString();

    //Iterating over the keys
    for(Property *prop: reflect->properties())
    {
        if(prop->tags()["key"].toBool()) // || (!autocolumn.isNull() && prop->tags()["name"].toString()==autocolumn))
        {
            if(prop->tags()["column"].isNull())
            {
                m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'column' was not supplied for the property '%1' in the class '%2'.")
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
    std::shared_ptr<QSqlQuery> query(new QSqlQuery(QSqlDatabase::database(databaseConnectionName())));
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
        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::DatabaseError, QString("Invalid SQL statement for update a row. See the sqlerror attached."), query->lastError()));
        return nullptr;
    }
}

std::shared_ptr<QSqlQuery> StandardSqlProvider::generateDelete(Class *reflect)
{
    //Objects to store the sql command
    QString sqlstr;
    QTextStream sql(&sqlstr);

    //Begin the sql command
    sql << "DELETE FROM ";

    //Add the table name
    if(!reflect->tags()["table"].isNull())
    {
        sql << reflect->tags()["table"].toString() << " WHERE ";
    }
    else
    {
        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'table' was not supplied for the class '%1'.").arg(reflect->tags()["name"].toString())));
        return nullptr;
    }//End add the table name

    //Object to store the bindings
    QMap<QString, QVariant> bindings;

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

    //QString autocolumn = reflect->tags()["autocolumn"].toString();

    //Iterating over the keys
    for(Property *prop: reflect->properties())
    {
        if(prop->tags()["key"].toBool()) // || (!autocolumn.isNull() && prop->tags()["name"].toString()==autocolumn))
        {
            if(prop->tags()["column"].isNull())
            {
                m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'column' was not supplied for the property '%1' in the class '%2'.")
                                                                .arg(prop->tags()["name"].toString())
                                                                .arg(reflect->tags()["name"].toString())));
            }
            else
            {
                sql << prop->tags()["column"].toString()
                    << " = "
                    << prop->tags()["column"].toString().prepend(":")
                    << " AND ";
                QVariant v = prop->value();
                bindings.insert(prop->tags()["column"].toString().prepend(":"), prop->value());
            }
        }
    }//End iterating over the keys

    sqlstr = sqlstr.left(sqlstr.size()-4);
    //Create the query object
    std::shared_ptr<QSqlQuery> query(new QSqlQuery(QSqlDatabase::database(databaseConnectionName())));
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
        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::DatabaseError, QString("Invalid SQL statement for delete a row. See the sqlerror attached."), query->lastError()));
        return nullptr;
    }
}

std::shared_ptr<QSqlQuery> StandardSqlProvider::generateSelect(Class *reflect, const Where &where, const QList<int> &fieldsno, const QList<QPair<QString, OrderBy> > orderby, int offset, int limit)
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
            m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'column' was not supplied for the property '%1' in the class '%2'.")
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
        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'table' was not supplied for the class '%1'.").arg(reflect->tags()["name"].toString())));
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
                    m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::OperationNotSupported, QString("This type of comparison is not supported by this sql provider.")));
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

    //Check if order by is used
    if(!orderby.isEmpty())
    {
        sql << " ORDER BY ";
        for(QPair<QString,OrderBy> order: orderby)
        {
            for(Property *prop: reflect->properties())
            {
                if(prop->tags()["name"].toString() == order.first)
                {
                    if(!prop->tags()["column"].isNull())
                    {
                        sql << prop->tags()["column"].toString() << " ";
                        if(order.second == OrderBy::Asc)
                            sql << "ASC, ";
                        else
                            sql << "DESC, ";
                    }
                    else
                    {
                        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'column' was not supplied for the property '%1' in the class '%2'.")
                                                                        .arg(prop->tags()["name"].toString())
                                                                        .arg(reflect->tags()["name"].toString())));
                        return nullptr;
                    }
                    break;
                }
            }
            //Check if the orderby refers to the autoid
            if(!reflect->tags()["autoid"].isNull() && order.first == "autoid")
            {
                sql << reflect->tags()["autoid"].toString() << " ";
                if(order.second == OrderBy::Asc)
                    sql << "ASC, ";
                else
                    sql << "DESC, ";
            }//Check if the orderby refers to the autoid
        }

        //Remove the last ", " from the command
        sqlstr = sqlstr.left(sqlstr.size()-2);
    }//End check if order by is used

    //Check if the offset and the limit are used
    if(offset > -1 && limit > 0)
    {
        sql << " LIMIT " << limit << " OFFSET " << offset;
    }//End check if the offset and the limit are used

    //Create a new query object
    std::shared_ptr<QSqlQuery> query(new QSqlQuery(QSqlDatabase::database(databaseConnectionName())));

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
        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::DatabaseError, QString("Invalid SQL statement for select. See the sqlerror attached."), query->lastError()));
        return nullptr;
    }

}
