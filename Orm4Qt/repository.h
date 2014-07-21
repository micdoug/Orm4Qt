#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "sqlprovider.h"
#include "ormerror.h"
#include "class.h"
#include "log.h"
#include <QStringList>
#include <QSqlField>
#include <QSqlDriver>
#include <memory>
#include <functional>

namespace Orm4Qt
{
    class Repository
    {
        //Begin static area
    public:
        /**
         * Create a repository instance using the provider registered.
         * @return
         * An instance of a repository.
         */
        static std::shared_ptr<Repository> createRepository()
        {
            return std::shared_ptr<Repository>(new Repository(sm_createProvider()));
        }
        /**
         * Register a provider function that will be used to create the sql providers for the new repositories
         * returned by createRepository().
         * @param provider
         * The function object that creates providers.
         */
        static void registerProvider(std::function<SqlProvider*()> provider)
        {
            Repository::sm_createProvider = provider;
        }
    protected:
        //The prototype provider
        static std::function<SqlProvider*()> sm_createProvider;
        //End static Area

    public:
        /**
         * Constructor with provider initialization
         * @param provider
         * Provider to be used as sql generator.
         */
        Repository(SqlProvider *provider)
        {
            m_provider = std::shared_ptr<SqlProvider>(provider);
        }
        /**
         * Copy constructor
         * @param other
         * Object used as a template for copy.
         */
        Repository(const Repository &other) : Repository(other.m_provider.get())
        {}
        /**
         * Virtual destructor.
         */
        virtual ~Repository()
        {}
        /**
         * Access the last error ocurred.
         * @return
         */
        std::shared_ptr<OrmError> lastError() const
        {
            return m_lastError;
        }

        /**
         * Save an object in the database using the sqlprovider.
         * @param object
         * Object to be saved in the database
         * @param fields
         * List of field names to be persisted. If the list is empty, all the fields will be stored.
         * @return
         * If the operation was executed with success or not.
         */
        template<class T>
        bool saveObject(T &object, const QStringList &fields = QStringList())
        {
            if(!openConnection())
                return false;

            //Inserting an object in the database
            if(object.reflection()->tags()["scope"].toInt() == Scope::Local)
            {
                return insertObject(object.reflection().get(), fields);
            }
            //Updating an object in the database
            else
            {
                return updateObject(object.reflection().get(), fields);
            }
        }

        /**
         * Remove an object from the database.
         * @param object
         * Object to be removed.
         * @return
         * If the operation was executed with success or not.
         */
        template<class T>
        bool deleteObject(T &object)
        {
            //Try to open the connection with the database
            if(!openConnection())
                return false;

            //Check the scope of the object
            if(object.reflection()->tags()["scope"].toInt() == Scope::Local)
            {
                m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::InvalidRequest, QString("This object doesn't exist in the database context.")));
                return false;
            }//Check the scope of the object

            //Create the object query
            std::shared_ptr<QSqlQuery> query = m_provider->generateDelete(object.reflection().get());

            //Check errors in the provider
            if(query == nullptr)
            {
                m_lastError = m_provider->lastError();
                return false;
            }//End check errors in the provider

#ifdef ORM4QT_DEBUG_SL
            //Debug the sql generated
            qCDebug(ORM4QT_SL) << getLastExecutedQuery(*query.get());
#endif

            //Exec the query
            if(query->exec())
            {
                //Check the number of rows affected
                if(query->numRowsAffected() < 1)
                {
                    m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::VersionError, QString("The delete failed, because the version number of the object or the values of the primary keys changed after you retrivied this object. You need to check this changes before update."), query->lastError()));
                    return false;
                }//End check the number of rows affected
                //Adjust the scope of the object to local
                else
                {
                    if(!object.reflection()->replaceTag("scope", Scope::Local))
                        object.reflection()->addTag("scope", Scope::Local);
                    return true;
                }//End adjust the scope of the object to local
            }
            else
            {
                m_lastError = std::shared_ptr<OrmError>(new OrmError(DatabaseError, QString("An error ocurred during the deletion of an object in the database. See the sqlerror attached."), query->lastError()));
                return false;
            }//End exec the query
        }

        /**
         * Select a list of objects in the database based on the conditions tested and the fields required
         * @param list
         * The list to be filled with the objects selected.
         * @param where
         * The condition used in the select statement. If not specified, no condition will be used.
         * @param fields
         * The list of fields to be retrieved. If not specified, all fields will be retrivied.
         * @return
         * True if the statement was executed with success or false in case of an error. The error can be retrieved using the method lastError().
         */
        template<class T>
        bool select(QList<T> &list, const Where &where = Where(), const QStringList &fields = QStringList(),
                    const QList<QPair<QString, OrderBy>> orderby = QList<QPair<QString, OrderBy>>(), int offset=-1, int limit=-1)
        {
            //Try to open connection with the database
            if(!openConnection())
                return false;

            //Create an object of the type to be selected to obtain the reflection object
            T temp;

            //Try to create the query
            QList<int> indexes = fieldIndexes(temp.reflection().get(), fields);
            std::shared_ptr<QSqlQuery> query = m_provider->generateSelect(temp.reflection().get(), where, indexes, orderby, offset, limit);
            if(query == nullptr)
            {
                m_lastError = m_provider->lastError();
                return false;
            }
#ifdef ORM4QT_DEBUG_SL
            //Debug the sql generated
            qCDebug(ORM4QT_SL) << getLastExecutedQuery(*query.get());
#endif
            //Try execute the query
            if(query->exec())
            {
                //Reading the result
                while(query->next())
                {
                    list.append(T());
                    std::shared_ptr<Class> reflect = list.last().reflection();
                    //Reading the properties
                    int count = 0;
                    for(int index: indexes)
                    {
                        QVariant value = query->value(count);
                        reflect->properties()[index]->setValue(value);
                        ++count;
                    }
                    //Reading the autoid
                    if(!reflect->tags()["autoid"].isNull())
                    {
                        if(reflect->addTag("id", query->value(count)))
                            reflect->replaceTag("id", query->value(count));
                        ++count;
                    }
                    //Reading the version
                    if(!reflect->tags()["versioncolumn"].isNull())
                    {
                        if(reflect->addTag("version", query->value(count)))
                            reflect->replaceTag("version", query->value(count));
                        ++count;
                    }
                    //Adjust the scope of the object
                    reflect->replaceTag("scope", Scope::Remote);
                }
                return true;
            }
            else
            {
                m_lastError = std::shared_ptr<OrmError>(new OrmError(DatabaseError, QString("An error ocurred during the select of records in the database. See the sqlerror attached."), query->lastError()));
                return false;
            }

        }

        template<class T>
        /**
         * Create a table for the object.
         * @return
         * If the operation was executed with success or not.
         */
        bool createTable()
        {
            if(!openConnection())
                return false;
            //Create an object of the type required
            T object;
            //Create the query object
            std::shared_ptr<QSqlQuery> query = m_provider->generateCreateTable(object.reflection().get());
            //If the query is invalid an error was generated by the provider
            if(query == nullptr)
            {
                //Adjust the last error and return false
                m_lastError = m_provider->lastError();
                return false;
            }
#ifdef ORM4QT_DEBUG_SL
            //Debug the sql generated
            qCDebug(ORM4QT_SL) << getLastExecutedQuery(*query.get());
#endif

            //The provider executed the create table statement
            return true;
        }

        template<class T>
        bool select(QList<T> &list, const QStringList &fields, const QList<QPair<QString, OrderBy>> orderby = QList<QPair<QString, OrderBy>>(), int limit=-1, int offset = 0)
        {
            return select(list, Where(), fields, orderby, offset, limit);
        }

        template<class T>
        bool select(QList<T> &list, const Where &where, const QList<QPair<QString, OrderBy>> orderby, int limit=-1, int offset = 0)
        {
            return select(list, where, QStringList(), orderby, offset, limit);
        }

        template<class T>
        bool select(QList<T> &list, const QList<QPair<QString, OrderBy>> orderby, int limit=-1, int offset = 0)
        {
            return select(list, Where(), QStringList(), orderby, offset, limit);
        }

        template<class T>
        bool select(QList<T> &list, int limit, int offset=0)
        {
            return select(list, Where(), QStringList(), QList<QPair<QString, OrderBy>>(), offset, limit);
        }

        /**
         * Open a transaction in the database connection.
         * @return
         * If the operation was executed with success or not.
         */
        bool beginTransaction()
        {
            if(!openConnection())
                return false;

            QSqlDatabase db = QSqlDatabase::database(m_provider->databaseConnectionName());
            if(db.transaction())
            {
                return true;
            }
            else
            {
                m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::DatabaseError, QString("An error occurred while opening the transaction. See the sql error attached."), db.lastError()));
                return false;
            }
        }

        /**
         * Commits the changes of the last transaction opened.
         * @return
         * If the operation was executed with success or not.
         */
        bool commit()
        {
            if(!openConnection())
                return false;

            QSqlDatabase db = QSqlDatabase::database(m_provider->databaseConnectionName());
            if(db.commit())
            {
                return true;
            }
            else
            {
                m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::DatabaseError, QString("An error occurred while commit the transaction. See the sql error attached."), db.lastError()));
                return false;
            }
        }

        /**
         * Aborts the changes of the last transaction opened.
         * @return
         * If the operation was executed with success or not.
         */
        bool rollback()
        {
            if(!openConnection())
                return false;

            QSqlDatabase db = QSqlDatabase::database(m_provider->databaseConnectionName());
            if(db.rollback())
            {
                return true;
            }
            else
            {
                m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::DatabaseError, QString("An error occurred while rollback the transaction. See the sql error attached."), db.lastError()));
                return false;
            }
        }

    private:
        //Store the sql provider used to create the query objects
        std::shared_ptr<SqlProvider> m_provider;
        //Store the description of the last error.
        std::shared_ptr<OrmError> m_lastError;

        /**
         * Generate the properties indexes of reflection objects based on a list of field names.
         * If the list is empty, returns all the properties in the reflection object.
         * @param reflect
         * The reflection object with the properties used
         * @param fields
         * The list of field names
         * @return
         * A list of field indexes
         */
        QList<int> fieldIndexes(Class *reflect, const QStringList &fields)
        {
            QList<int> indexes;
            if(fields.isEmpty())
            {
                for(int i=0; i<reflect->properties().count(); ++i)
                {
                    indexes << i;
                }
            }
            else
            {
                for(int i=0; i<reflect->properties().count(); ++i)
                {
                    Property *prop = reflect->properties()[i];
                    if(fields.contains(prop->tags()["name"].toString()))
                        indexes << i;
                }
            }
            return indexes;
        }

        /**
         * Try to open the connection with the database
         * @return
         * If the connection is open or not
         */
        bool openConnection()
        {
            QSqlDatabase db = QSqlDatabase::database(m_provider->databaseConnectionName());
            if(db.isOpen())
            {
                return true;
            }
            else
            {
                m_lastError = std::shared_ptr<OrmError>(new OrmError(DatabaseError, QString("An error ocurred on openning connection with the database. See the sqlerror attached."), db.lastError()));
                return false;
            }
        }

        /**
         * Method used to debug the sql commands generated.
         * Reference: http://stackoverflow.com/questions/5777409/how-to-get-last-prepared-and-executed-query-using-qsqlquery
         * @param query
         * The query object used to execute the query
         * @return
         * A string with the sql command executed.
         */
        QString getLastExecutedQuery(const QSqlQuery& query)
        {
            QString sql = query.executedQuery();
            int nbBindValues = query.boundValues().size();
            for(int i = 0, j = 0; j < nbBindValues;)
            {
                int s = sql.indexOf(QLatin1Char('\''), i);
                i = sql.indexOf(QLatin1Char('?'), i);
                if (i < 1)
                {
                    break;
                }
                if(s < i && s > 0)
                {
                    i = sql.indexOf(QLatin1Char('\''), s + 1) + 1;
                    if(i < 2)
                    {
                        break;
                    }
                }
                else
                {
                    const QVariant &var = query.boundValue(j);
                    QSqlField field(QLatin1String(""), var.type());
                    if (var.isNull())
                    {
                        field.clear();
                    }
                    else
                    {
                        field.setValue(var);
                    }
                    QString formatV = query.driver()->formatValue(field);
                    sql.replace(i, 1, formatV);
                    i += formatV.length();
                    ++j;
                }
            }
            return sql;
        }

        /**
         * Insert an object into the database using the provider to execute sql statements.
         * @param object
         * The object to be persisted
         * @param fields
         * Optional list of field names to be inserted
         * @return
         * True if the operation was successfully executed or false in case of an error. The description
         * of the error can be accessed via the method lastError().
         */
        bool insertObject(Class *object, QStringList fields)
        {
            //Get the indexes of the fields to be inserted
            QList<int> indexes = fieldIndexes(object, fields);

            //Check if the object use the autocolumn
            if(!object->tags()["autocolumn"].isNull())
            {
                QString autocolumn = object->tags()["autocolumn"].toString();
                for(int index: indexes)
                {
                    Property *prop = object->properties()[index];
                    if(prop->tags()["name"].toString()==autocolumn)
                    {
                        indexes.removeOne(index);
                    }
                }
            }

            //Check if is necessary to generate a version number
            if(!object->tags()["versioncolumn"].isNull())
            {
                if(object->addTag("version", 0))
                    object->replaceTag("version", 0);
            }

            //Creating query object
            std::shared_ptr<QSqlQuery> query = m_provider->generateInsert(object, indexes);
            //Checking errors in provider
            if(query == nullptr)
            {
                m_lastError = m_provider->lastError();
                return false;
            }
#ifdef ORM4QT_DEBUG_SL
            //Debug the sql generated
            qCDebug(ORM4QT_SL) << getLastExecutedQuery(*query.get());
#endif

            //Executing the sql statement
            if(query->exec())
            {
                //Verifying if is necessary to obtain an autogenerated id
                if(!object->tags()["autoid"].isNull())
                {
                    if(!object->addTag("id", query->lastInsertId()))
                        object->replaceTag("id", query->lastInsertId());
                }
                else if(!object->tags()["autocolumn"].isNull())
                {
                    QString column = object->tags()["autocolumn"].toString();
                    for(Property *prop: object->properties())
                    {
                        if(prop->tags()["name"].toString() == column)
                        {
                            prop->setValue(query->lastInsertId());
                            break;
                        }
                    }
                }//end check if is necessary to obtain an autogenerated id

                //Adjust the scope of the object
                object->replaceTag("scope", Scope::Remote);
                return true;
            }
            else
            {
                m_lastError = std::shared_ptr<OrmError>(new OrmError(DatabaseError, QString("An error ocurred during the insertion of an object in the database. See the sqlerror attached."), query->lastError()));
                return false;
            }
        }

        /**
         * @brief updateObject
         * @param object
         * @param fields
         * @return
         */
        bool updateObject(Class *object, QStringList fields)
        {
            //Check if the object use the versioncolumn
            if(!object->tags()["versioncolumn"].isNull())
            {
                //Generate the new version number
                QVariant v(object->tags()["version"].value<qlonglong>()+1);
                //Store the new version in a tag
                if(!object->addTag("newversion", v))
                    object->replaceTag("newversion", v);
            }//End Check if the object use the versioncolumn

            //Get the indexes of the fields to be updated
            QList<int> indexes = fieldIndexes(object, fields);

            //Check if the object use the autocolumn
            if(!object->tags()["autocolumn"].isNull())
            {
                QString autocolumn = object->tags()["autocolumn"].toString();
                for(int index: indexes)
                {
                    Property *prop = object->properties()[index];
                    if(prop->tags()["name"].toString()==autocolumn)
                    {
                        indexes.removeOne(index);
                    }
                }
            }

            //Remove all the keys from the update list
            for(int index: indexes)
            {
                Property *prop = object->properties()[index];
                if(prop->tags()["key"].toBool())
                    indexes.removeOne(index);
            }//End remove all the keys from the update list

            //Create a new query object
            std::shared_ptr<QSqlQuery> query = m_provider->generateUpdate(object, indexes);

            //Checking errors in provider
            if(query == nullptr)
            {
                m_lastError = m_provider->lastError();
                return false;
            }

#ifdef ORM4QT_DEBUG_SL
            //Debug the sql generated
            qCDebug(ORM4QT_SL) << getLastExecutedQuery(*query.get());
#endif

            //Executing the update statement
            if(query->exec())
            {
                //Check if the query changed rows in the database
                if(query->numRowsAffected() < 1)
                {
                    object->removeTag("newversion");
                    m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::VersionError, QString("The update failed, because the version number of the object or the values of the primary keys changed after you retrivied this object. You need to check this changes before update."), query->lastError()));
                    return false;
                }
                else
                {
                    //If is necessary adjust the new version number
                    if(!object->tags()["versioncolumn"].isNull())
                        object->replaceTag("version", object->tags()["newversion"]);

                    object->removeTag("newversion");
                    return true;
                }
            }
            //In case of an error
            else
            {
                object->removeTag("newversion");
                m_lastError = std::shared_ptr<OrmError>(new OrmError(ErrorType::DatabaseError, QString("An error ocurred during the update of an object in the database. See the sqlerror attached."), query->lastError()));
                return false;
            }
        }

    };

    //Initializing static members
    std::function<SqlProvider*()> Repository::sm_createProvider = [](){ return nullptr; };

}

#endif // REPOSITORY_H
