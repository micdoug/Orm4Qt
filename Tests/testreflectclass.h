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

#ifndef TESTREFLECTIONCLASS_H
#define TESTREFLECTIONCLASS_H

#include <QObject>
#include "reflect.h"
#include <QtTest>

using namespace Orm4Qt;

class TestReflectClass : public QObject
{
    Q_OBJECT
public:
    TestReflectClass();
private slots:
    void initTestCase();
    void addTags();
    void replaceTags();
    void removeTags();
    void addTagsFromString();
    void cleanupTestCase();
private:
    Reflect *m_reflect;
};

#endif // TESTREFLECTIONCLASS_H
