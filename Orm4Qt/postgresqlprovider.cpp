/*
 * Orm4Qt - An Object Relational Mapping Library for the Qt Framework
 * Copyright (c) 2014, Michael Dougras da Silva, All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */

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
        sql << reflect->tags()["table"].toString();
        sql << " (";
    }
    else
    {
        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'table' was not supplied for the class '%1'.").arg(reflect->tags()["name"].toString())));
        return nullptr;
    }//End add table name

    //Verify if autoid is used
    if(!reflect->tags()["autoid"].isNull())
    {
        sql << reflect->tags()["autoid"].toString() << " BIGSERIAL, ";
        pk << reflect->tags()["autoid"].toString() << ", ";
    }//End verify if autoid is used

    //Verify if version column is active
    if(!reflect->tags()["versioncolumn"].isNull())
    {
        sql << reflect->tags()["versioncolumn"].toString() << " INTEGER NOT NULL, ";
    }//End verify if version column is active

    //Get the autocolumn name when it is used
    QString autocolumn = reflect->tags()["autocolumn"].toString();

    //Iterating the properties
    for(Property *prop : reflect->properties())
    {
        //Verifying if the tag column is present
        if(prop->tags()["column"].isNull())
        {
            m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The tag 'column' was not supplied for the property '%1' in the class '%2'.")
                                                            .arg(prop->tags()["name"].toString())
                                                            .arg(reflect->tags()["name"].toString())));
            return nullptr;
        } //End verify tag column
        //Adding the definition of the column in the sql command
        else
        {
            sql << prop->tags()["column"].toString() << " ";

            //Defining the type of the column
            //Check if the column is the autocolumn
            if(prop->tags()["name"].toString() == autocolumn)
            {
                sql << "BIGSERIAL ";
            }
            else
            {
                switch(prop->type())
                {
                    case QMetaType::Bool:
                        sql << "BOOLEAN ";
                        break;
                    case QMetaType::UInt:
                    case QMetaType::Int:
                        sql << "INTEGER ";
                        break;
                    case QMetaType::Short:
                    case QMetaType::UShort:
                        sql << "SMALLINT ";
                        break;
                    case QMetaType::LongLong:
                    case QMetaType::ULongLong:
                    case QMetaType::Long:
                    case QMetaType::ULong:
                        sql << "BIGINT ";
                        break;
                    case QMetaType::Double:
                    case QMetaType::Float:
                        sql << "DECIMAL ";
                        break;
                    case QMetaType::QByteArray:
                        sql << "BLOB ";
                        break;
                    case QMetaType::QString:
                        if(!prop->tags()["maxlength"].isNull())
                        {
                            sql << QString("VARCHAR(%1) ").arg(prop->tags()["maxlength"].toInt());
                        }
                        else if(!prop->tags()["length"].isNull())
                        {
                            sql << QString("CHAR(%1) ").arg(prop->tags()["length"].toInt());
                        }
                        else
                        {
                            sql << "TEXT ";
                        }
                        break;
                    case QMetaType::QDateTime:
                        sql << "TIMESTAMPTZ ";
                        break;
                    case QMetaType::QTime:
                        sql << "TIME ";
                        break;
                    case QMetaType::QDate:
                        sql << "DATE ";
                        break;
                    case QMetaType::User:
                        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::ConfigurationError, QString("The type of the property '%1' in the class '%2' is not supported.")
                                                                    .arg(prop->tags()["name"].toString())
                                                                    .arg(reflect->tags()["name"].toString())));
                        return nullptr;
                    default:
                        sql << "TEXT ";
                } //End defining the type of the column
            }

            //Check if the column is a key
            if(prop->tags()["key"].toBool())
            {
                pk << prop->tags()["column"].toString() << ", ";
            } //End check if the column is a key

            //Check if the property is required or not
            if(!prop->tags()["required"].toBool() && (prop->isPointer() ||
                                                      prop->type() == QMetaType::QDate ||
                                                      prop->type() == QMetaType::QDateTime ||
                                                      prop->type() == QMetaType::QTime ||
                                                      prop->type() == QMetaType::QString ||
                                                      prop->type() == QMetaType::QByteArray))
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
    std::shared_ptr<QSqlQuery> query(new QSqlQuery(QSqlDatabase::database(databaseConnectionName())));
    //Check if the query is valid and can be exec
    if(query->exec(sqlstr))
    {
        return query;
    }
    //If it doesn't, generate an error
    else
    {
        m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::DatabaseError, QString("Invalid SQL statement for create table. See the sqlerror attached."), query->lastError()));
        return nullptr;
    }
}
