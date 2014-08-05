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
