#ifndef PROPERTYCONCRETESTDSTRING_H
#define PROPERTYCONCRETESTDSTRING_H

#include "propertyconcrete.h"
#include <string>
#include <functional>
#include <type_traits>
#include <QString>

namespace Orm4Qt
{
    template<>
    class PropertyConcrete<std::string> : public Property
    {
    public:
        //Constructors and destructors
        explicit PropertyConcrete(std::function<std::string&()> func, const QHash<QString, QVariant> &tags = QHash<QString, QVariant>()):
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
            v.setValue<QString>(QString::fromStdString(m_function()));
            return v;
        }
        virtual void setValue(const QVariant &value) override
        {
            if(value.canConvert<QString>())
            {
                m_function() = value.value<QString>().toStdString();
            }
            else
            {
                throw new std::bad_cast();
            }
        }
        virtual int type() const override
        {
            return qMetaTypeId<QString>();
        }
        virtual bool isPointer() const override
        {
            return false;
        }

    private:
        std::function<std::string&()> m_function;
    };

    template<>
    class PropertyConcrete<std::string*> : public Property
    {
    public:
        //Constructors and destructors
        explicit PropertyConcrete(std::function<std::string*&()> func, const QHash<QString, QVariant> &tags = QHash<QString, QVariant>()):
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
                v.setValue<QString>(QString::fromStdString(*m_function()));
            }
            return v;
        }
        virtual void setValue(const QVariant &value) override
        {
            if(value.isNull())
            {
                m_function() = nullptr;
            }
            else if(value.canConvert<QString>())
            {
                if(m_function() != nullptr)
                    delete m_function();
                m_function() = new std::string(value.value<QString>().toStdString());
            }
            else
            {
                throw new std::bad_cast();
            }
        }
        virtual int type() const override
        {
            return qMetaTypeId<QString>();
        }
        virtual bool isPointer() const override
        {
            return true;
        }

    private:
        std::function<std::string*&()> m_function;
    };
}

#endif // PROPERTYCONCRETESTDSTRING_H
