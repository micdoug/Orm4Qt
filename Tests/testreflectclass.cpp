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

#include "testreflectclass.h"

TestReflectClass::TestReflectClass()
{
}

void TestReflectClass::initTestCase()
{
    this->m_reflect = new Reflect();
}

void TestReflectClass::addTags()
{
    //Test of adding different data types
    QVERIFY2(m_reflect->addTag("tag1", 1), QString("Error on adding tag of type int. Tag with name '%1'.").arg("tag1").toLocal8Bit().data());
    QVERIFY2(m_reflect->tags().contains("tag1"), QString("Error, the tag '%1' wasn't added.").arg("tag1").toLocal8Bit().data());
    QVERIFY2(m_reflect->addTag("tag2", 1.5), QString("Error on adding tag of type double. Tag with name '%1'.").arg("tag2").toLocal8Bit().data());
    QVERIFY2(m_reflect->tags().contains("tag2"), QString("Error, the tag '%1' wasn't added.").arg("tag2").toLocal8Bit().data());
    QVERIFY2(m_reflect->addTag("tag3", true), QString("Error on adding tag of type bool. Tag with name '%1'.").arg("tag3").toLocal8Bit().data());
    QVERIFY2(m_reflect->tags().contains("tag3"), QString("Error, the tag '%1' wasn't added.").arg("tag3").toLocal8Bit().data());
    QVERIFY2(m_reflect->addTag("tag4", "test"), QString("Error on adding tag of type QString. Tag with name '%1'.").arg("tag4").toLocal8Bit().data());
    QVERIFY2(m_reflect->tags().contains("tag4"), QString("Error, the tag '%1' wasn't added.").arg("tag4").toLocal8Bit().data());

    //Test of trying add a tag that already exists
    QVERIFY2(!m_reflect->addTag("tag1", "test1"), QString("Error, a tag that already exists was added.").toLocal8Bit().data());
    QVERIFY2(m_reflect->tags()["tag1"].toInt() == 1, QString("Error, the tag with name '%1' was incorrect modified.").toLocal8Bit().data());
}

void TestReflectClass::replaceTags()
{
    //Test of replacing different data types
    QVERIFY2(m_reflect->replaceTag("tag1", "test"), QString("Error on replacing tag of type int with type QString. Tag with name '%1'.").arg("tag1").toLocal8Bit().data());
    QVERIFY2(m_reflect->tags()["tag1"].toString()=="test", QString("Error, the tag '%1' wasn't replaced.").arg("tag1").toLocal8Bit().data());
    QVERIFY2(m_reflect->replaceTag("tag2", true), QString("Error on replacing tag of type double with type bool. Tag with name '%1'.").arg("tag2").toLocal8Bit().data());
    QVERIFY2(m_reflect->tags()["tag2"].toBool(), QString("Error, the tag '%1' wasn't replaced.").arg("tag2").toLocal8Bit().data());
    QVERIFY2(m_reflect->replaceTag("tag3", 1.5), QString("Error on replacing tag of type bool with type double. Tag with name '%1'.").arg("tag3").toLocal8Bit().data());
    QVERIFY2(m_reflect->tags()["tag3"].toDouble()==1.5, QString("Error, the tag '%1' wasn't replaced.").arg("tag3").toLocal8Bit().data());
    QVERIFY2(m_reflect->replaceTag("tag4", 1), QString("Error on replacing tag of type QString with type int. Tag with name '%1'.").arg("tag4").toLocal8Bit().data());
    QVERIFY2(m_reflect->tags()["tag4"].toInt() == 1, QString("Error, the tag '%1' wasn't replaced.").arg("tag4").toLocal8Bit().data());

    //Test of replacing a tag that doesn't exits
    QVERIFY2(!m_reflect->replaceTag("tag5", false), QString("Error, the tag '%1' doesn't exist but was replaced.").arg("tag5").toLocal8Bit().data());
}

void TestReflectClass::removeTags()
{
    QVERIFY2(m_reflect->removeTag("tag1"), QString("Error on removing the '%1'.").arg("tag1").toLocal8Bit().data());
    QVERIFY2(!m_reflect->tags().contains("tag1"), QString("The tag '%1' wasn't removed.").arg("tag1").toLocal8Bit().data());
    QVERIFY2(m_reflect->removeTag("tag2"), QString("Error on removing the '%1'.").arg("tag2").toLocal8Bit().data());
    QVERIFY2(!m_reflect->tags().contains("tag2"), QString("The tag '%1' wasn't removed.").arg("tag2").toLocal8Bit().data());
    QVERIFY2(m_reflect->removeTag("tag3"), QString("Error on removing the '%1'.").arg("tag3").toLocal8Bit().data());
    QVERIFY2(!m_reflect->tags().contains("tag3"), QString("The tag '%1' wasn't removed.").arg("tag3").toLocal8Bit().data());
    QVERIFY2(m_reflect->removeTag("tag4"), QString("Error on removing the '%1'.").arg("tag4").toLocal8Bit().data());
    QVERIFY2(!m_reflect->tags().contains("tag4"), QString("The tag '%1' wasn't removed.").arg("tag4").toLocal8Bit().data());
}

void TestReflectClass::addTagsFromString()
{
    m_reflect->addTagsFromString(QString("tag1=1, tag2=true, tag3=2.5, tag4 invalid"));
    QVERIFY2(m_reflect->tags().contains("tag1"), QString("The tag '%1' wasn't added.").arg("tag1").toLocal8Bit().data());
    QVERIFY2(m_reflect->tags().contains("tag2"), QString("The tag '%1' wasn't added.").arg("tag2").toLocal8Bit().data());
    QVERIFY2(m_reflect->tags().contains("tag3"), QString("The tag '%1' wasn't added.").arg("tag3").toLocal8Bit().data());
    QVERIFY2(!m_reflect->tags().contains("tag4"), QString("The invalid tag '%1' was added.").arg("tag4").toLocal8Bit().data());
}

void TestReflectClass::cleanupTestCase()
{
    delete this->m_reflect;
}
