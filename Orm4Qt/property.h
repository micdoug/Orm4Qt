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

#ifndef PROPERTY_H
#define PROPERTY_H

#include "reflect.h"

namespace Orm4Qt
{
    class Property : public Reflect
    {
    public:
        //Constructors and destructor
        explicit Property(const QHash<QString, QVariant> &tags = QHash<QString, QVariant>());
        Property(const Property &other);
        virtual ~Property();

        //Abstract methods for access and adjust value of properties
        virtual const QVariant value() const = 0;
        virtual void setValue(const QVariant &value) = 0;
        virtual int type() const = 0;
        virtual bool isPointer() const = 0;

        //Extending management tags methods to generate debug messages
#ifdef ORM4QT_DEBUG_OL
        virtual bool addTag(const QString &name, const QVariant &value) override;
        virtual bool replaceTag(const QString &name, const QVariant &value) override;
        virtual bool removeTag(const QString &name) override;
#endif
    };
}

#endif // PROPERTY_H
