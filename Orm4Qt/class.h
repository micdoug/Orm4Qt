#ifndef CLASS_H
#define CLASS_H

#include "reflect.h"
#include "property.h"
#include "propertyconcrete.h"
#include <QList>
#include "log.h"

namespace Orm4Qt
{
    enum Scope
    {
        Local = 0,
        Remote = 1
    };

    /**
     * Class used for storing metadata about classes. It provides an interface for access the properties of
     * the class
     */
    class Class : public Reflect
    {
    public:
        //Constructors and destructors
        /**
         * Constructor with optional initialization of the list of properties and the list of tags
         * @param properties
         * The list of properties used for initialization
         * @param tags
         * The list of tags used for initialization
         */
        explicit Class(const QList<Property*> &properties = QList<Property*>(), const QHash<QString, QVariant> &tags=QHash<QString, QVariant>())
            : Reflect(tags), m_properties(properties)
        {
            this->addTag("scope", Scope::Local);
        }
        /**
         * Copy constructor
         * @param other
         * The instance to be cloned
         */
        Class(const Class &other) : Reflect(other) //m_properties(other.properties())
        {}
        /**
         * Destructor
         */
        virtual ~Class()
        {
            qDeleteAll(m_properties.begin(), m_properties.end());
        }

        //Methods for management of properties
        /**
         * Access the list of properties associated with the class
         * @return
         * The list of properties associated with the class
         */
        virtual const QList<Property*> &properties() const
        {
            return static_cast<const QList<Property*>&>(m_properties);
        }
        /**
         * Add an instance of property reflection
         * @param property
         * The property to be added
         */
        virtual void addProperty(Property *property)
        {
            m_properties << property;
        }
        /**
         * Remove a property reflection instance
         * @param index
         * The index of the property to be removed
         * @return
         * True if the index is valid and the property was removed or false if the index is invalid and
         * the property wasn't removed.
         */
        virtual bool removeProperty(int index)
        {
            if(index > 0 && index < m_properties.count())
            {
                delete m_properties[index];
                m_properties.removeAt(index);
                return true;
            }
            else
            {
                return false;
            }
        }
        /**
         * A template method for add an instance of PropertyConcrete in the list of properties
         * @param property
         * The property to be added
         */
        template<typename T>
        void addPropertyConcrete(PropertyConcrete<T> *property)
        {
            this->addProperty(property);
        }
        //Reimplemented tags management tags for generate log statements
#ifdef ORM4QT_DEBUG_OL
        virtual bool addTag(const QString &name, const QVariant &value) override
        {
            if(Reflect::addTag(name, value))
            {
                qCDebug(ORM4QT_OL) << QString("Added in the class the tag '%1' with value '%2'").arg(name).arg(value.toString());
                return true;
            }
            else
            {
                qCWarning(ORM4QT_OL) << QString("The tag '%1' already exists in the class with value '%2'. The new value '%3' will be ignored.").arg(name).arg(tags()[name].toString()).arg(value.toString());
                return false;
            }
        }
        virtual bool replaceTag(const QString &name, const QVariant &value) override
        {
            if(Reflect::replaceTag(name, value))
            {
                qCDebug(ORM4QT_OL) << QString("Replaced in the class the tag '%1' with the new value '%2'").arg(name).arg(value.toString());
                return true;
            }
            else
            {
                qCWarning(ORM4QT_OL) << QString("The tag '%1' doesn't exist in the class, so it can't be replaced with the new value '%2'.").arg(name).arg(value.toString());
                return false;
            }
        }
        virtual bool removeTag(const QString &name) override
        {
            if(Reflect::removeTag(name))
            {
                qCDebug(ORM4QT_OL) << QString("Removed in the class the tag '%1'.").arg(name);
                return true;
            }
            else
            {
                qCWarning(ORM4QT_OL) << QString("Can't remove the tag '%1' in the class, because it doesn't exist.").arg(name);
                return false;
            }
        }
#endif
        void clearProperties()
        {
            qDeleteAll(m_properties.begin(), m_properties.end());
            m_properties.clear();
        }
    private:
        QList<Property*> m_properties;

    };
}

#endif // CLASS_H
