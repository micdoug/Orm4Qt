#ifndef CLASS_H
#define CLASS_H

#include "reflect.h"
#include "property.h"
#include "propertyconcrete.h"
#include <QList>

namespace Orm4Qt
{
    class Class : public Reflect
    {
    public:
        //Constructors and destructors
        explicit Class(const QList<Property*> &properties = QList<Property*>(), const QHash<QString, QVariant> &tags=QHash<QString, QVariant>())
            : Reflect(tags), m_properties(properties)
        {}
        Class(const Class &other) : Reflect(other), m_properties(other.properties())
        {}
        virtual ~Class()
        {
            qDeleteAll(m_properties.begin(), m_properties.end());
        }

        //Methods for management of properties
        virtual const QList<Property*> &properties() const
        {
            return static_cast<const QList<Property*>&>(m_properties);
        }
        virtual void addProperty(Property *property)
        {
            m_properties << property;
        }
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
        template<typename T>
        void addPropertyConcrete(PropertyConcrete<T> *property)
        {
            this->addProperty(property);
        }
    private:
        QList<Property*> m_properties;

    };
}

#endif // CLASS_H
