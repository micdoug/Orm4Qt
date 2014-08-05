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

#ifndef TESTREFLECTIONSYSTEM_H
#define TESTREFLECTIONSYSTEM_H

#include "person.h"
#include <QtTest>

class TestReflectionSystem : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void validatePropertiesNameAndType();
    void getSetNonPointerValues();
    void getSetPointerValues();
    void getSetQVariantNonSupportedTypes();

private:
    Person m_person;
};

#endif // TESTREFLECTIONSYSTEM_H
