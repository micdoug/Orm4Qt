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

#include "property.h"
#include "log.h"

using namespace Orm4Qt;

/**
 * \class Orm4Qt::Property
 * Class used for storing metadata about properties of classes. This class also
 * provide an interface for set and retrieve the current value stored in the property
 * being used
 */

/**
 * Constructor with optional initialization of the list of tags
 * @param tags
 * The list of tags to be used for initialization
 */
Property::Property(const QHash<QString, QVariant> &tags) : Reflect(tags)
{}

/**
 * Copy constructor
 * @param other
 * The instance to be cloned
 */
Property::Property(const Property &other) : Reflect(other)
{}

/**
 * Destructor
 */
Property::~Property()
{}

#ifdef ORM4QT_DEBUG_OL
bool Property::addTag(const QString &name, const QVariant &value)
{
    if(Reflect::addTag(name, value))
    {
        qCDebug(ORM4QT_OL) << QString("Added in the property the tag '%1' with value '%2'").arg(name).arg(value.toString());
        return true;
    }
    else
    {
        qCWarning(ORM4QT_OL) << QString("The tag '%1' already exists in the property with value '%2'. The new value '%3' will be ignored.").arg(name).arg(tags()[name].toString()).arg(value.toString());
        return false;
    }
}

bool Property::replaceTag(const QString &name, const QVariant &value)
{
    if(Reflect::replaceTag(name, value))
    {
        qCDebug(ORM4QT_OL) << QString("Replaced in the property the tag '%1' with the new value '%2'").arg(name).arg(value.toString());
        return true;
    }
    else
    {
        qCWarning(ORM4QT_OL) << QString("The tag '%1' doesn't exist in the property, so it can't be replaced with the new value '%2'.").arg(name).arg(value.toString());
        return false;
    }
}

bool Property::removeTag(const QString &name)
{
    if(Reflect::removeTag(name))
    {
        qCDebug(ORM4QT_OL) << QString("Removed in the property the tag '%1'.").arg(name);
        return true;
    }
    else
    {
        qCWarning(ORM4QT_OL) << QString("Can't remove the tag '%1' in the property, because it doesn't exist.").arg(name);
        return false;
    }
}
#endif

/**
 * \fn virtual const QVariant Property::value() const = 0
 * Retrieves the current value stored in the property
 * \return Current value stored
 */

/**
 * \fn virtual void Property::setValue(const QVariant &value) = 0
 * Set the current value to be stored in the property
 * @param value
 * The new value to be stored in the property
 */

/**
 * \fn virtual int Property::type() const = 0
 * Return the id of the QMetaType::Type associated with the property data type
 * \return Id of the property data type
 */

/**
 * \fn virtual bool Property::isPointer() const = 0
 * Return if the property is a pointer type or not.
 * \return If the property is a pointer type
 */

