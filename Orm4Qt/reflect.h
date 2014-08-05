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

#ifndef REFLECT_H
#define REFLECT_H

#include <QHash>
#include <QString>
#include <QVariant>

namespace Orm4Qt
{
    class Reflect
    {
    public:
        //Constructors and destructors
        explicit Reflect(const QHash<QString, QVariant> &tags=QHash<QString, QVariant>());
        Reflect (const Reflect &other);
        virtual ~Reflect();

        //Management of tags
        virtual const QHash<QString, QVariant> &tags() const;
        virtual bool addTag(const QString &name, const QVariant &value);
        virtual bool replaceTag(const QString &name, const QVariant &value);
        virtual bool removeTag(const QString &name);
        virtual void addTagsFromString(const QString &tags);

    private:
        //Internal properties
        QHash<QString, QVariant> m_tags;
    };
}

#endif // REFLECT_H
