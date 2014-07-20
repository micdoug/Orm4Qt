#ifndef PROPERTYCONCRETE_H
#define PROPERTYCONCRETE_H

#include "property.h"
#include <functional>
#include <type_traits>

namespace Orm4Qt
{
    template<typename T>
    class PropertyConcrete : public Property
    {
    public:
        //Constructors and destructors
        explicit PropertyConcrete(std::function<T&()> func, const QHash<QString, QVariant> &tags = QHash<QString, QVariant>()):
            Property(tags), m_function(func)
        {}
        PropertyConcrete(const PropertyConcrete &other) :
            Property(other),m_function(other.m_function)
        {}
        virtual ~PropertyConcrete()
        {}

        //Reimplemented methods
        virtual const QVariant value() const override
        {
            QVariant v;
            v.setValue<T>(m_function());
            return v;
        }
        virtual void setValue(const QVariant &value) override
        {
            if(value.canConvert<T>())
            {
                m_function() = value.value<T>();
            }
            else
            {
                throw new std::bad_cast();
            }
        }
        virtual int type() const override
        {
            return qMetaTypeId<T>();
        }
        virtual bool isPointer() const override
        {
            return false;
        }

    private:
        std::function<T&()> m_function;
    };

    template<typename T>
    class PropertyConcrete<T*> : public Property
    {
    public:
        //Constructors and destructors
        explicit PropertyConcrete(std::function<T*&()> func, const QHash<QString, QVariant> &tags = QHash<QString, QVariant>()):
            Property(tags), m_function(func)
        {}
        explicit PropertyConcrete(const PropertyConcrete &other) :
            Property(other),m_function(other.m_function)
        {}
        virtual ~PropertyConcrete()
        {}

        //Reimplemented methods
        virtual const QVariant value() const override
        {
            QVariant v;
            if(m_function() != nullptr)
            {
                v.setValue<T>(*m_function());
            }
            return v;
        }
        virtual void setValue(const QVariant &value) override
        {
            if(value.isNull())
            {
                m_function() = nullptr;
            }
            else if(value.canConvert<T>())
            {
                if(m_function() != nullptr)
                    delete m_function();
                m_function() = new T(value.value<T>());
            }
            else
            {
                throw new std::bad_cast();
            }
        }
        virtual int type() const override
        {
            return qMetaTypeId<T>();
        }
        virtual bool isPointer() const override
        {
            return true;
        }

    private:
        std::function<T*&()> m_function;
    };
}

#endif // PROPERTYCONCRETE_H
